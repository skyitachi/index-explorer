//
// Created by Shiping Yao on 2023/1/4.
//

#include <sys/fcntl.h>
#include <unistd.h>
#include "MetaBlockReader.h"

namespace explorer {
namespace duckdb {

static void ReadFieldHeader(Deserializer& deserializer) {
  deserializer.Read<uint32_t>();
  deserializer.Read<uint64_t>();
}


File::File(const std::string &file_path) {
  int fd = ::open(file_path.c_str(), O_RDONLY);
  if (fd < 0) {
    ::exit(-1);
  }
  fd_ = fd;
  offset_ = 0;
}

idx_t File::read(data_ptr_t data, idx_t size) {
  auto st = ::pread(fd_, data, size, offset_);
  if (st < 0) {
    throw IOException(strerror(errno));
  }
  offset_ += st;
  return st;
}

idx_t File::read(data_ptr_t data, idx_t size, idx_t offset) {
  auto st = ::pread(fd_, data, size, offset);
  if (st < 0) {
    throw IOException(strerror(errno));
  }
  return st;
}

void File::ReadData(data_ptr_t buffer, idx_t read_size) {
  read(buffer, read_size);
}

const char MainHeader::MAGIC_BYTES[] = "DUCK";

void MainHeader::CheckMagicBytes(File &file) {
  data_t magic_bytes[MAGIC_BYTE_SIZE];
  file.read(magic_bytes, MAGIC_BYTE_SIZE, MAGIC_BYTE_OFFSET);
  if (memcmp(magic_bytes, MAGIC_BYTES, MAGIC_BYTE_SIZE) != 0) {
    throw IOException("unexpect magic bytes");
  }
}

DatabaseHeader DatabaseHeader::ReadDatabaseHeader(Deserializer &deserializer) {
  DatabaseHeader header{};
  header.iteration = deserializer.Read<uint64_t>();
  header.meta_block = deserializer.Read<block_id_t>();
  header.free_list = deserializer.Read<block_id_t>();
  header.block_count = deserializer.Read<uint64_t>();
  return header;
}

SchemaBlock SchemaBlock::Read(Deserializer& deserializer) {
  SchemaBlock schema_block{};
  schema_block.checksum = deserializer.Read<uint64_t>();
  schema_block.next_block = deserializer.Read<block_id_t>();
  schema_block.schema_count = deserializer.Read<uint32_t>();

  for(int i = 0; i < schema_block.schema_count; i++) {
    schema_block.schemas.push_back(std::move(Schema::Read(deserializer)));
  }
  return schema_block;
}

ExtraTypeInfo ExtraTypeInfo::Read(Deserializer &deserializer) {
  auto type = deserializer.Read<ExtraTypeInfoType>();
  ExtraTypeInfo extra_info{};
  switch (type) {
    case ExtraTypeInfoType::INVALID_TYPE_INFO: {
      auto alias = deserializer.Read<std::string>();
      if (!alias.empty()) {
        return extra_info;
      }
      return extra_info;
    }
    case ExtraTypeInfoType::GENERIC_TYPE_INFO: {
      extra_info.type = type;
    } break;
    case ExtraTypeInfoType::DECIMAL_TYPE_INFO: {
      deserializer.Read<uint8_t>();
      deserializer.Read<uint8_t>();
      extra_info.type = type;
      break;
    }
    case ExtraTypeInfoType::STRING_TYPE_INFO:
      deserializer.Read<std::string>();
      extra_info.type = type;
      break;
    case ExtraTypeInfoType::LIST_TYPE_INFO:
      deserializer.Read<LogicalTypeId>();
      extra_info.type = type;
      break;
    case ExtraTypeInfoType::STRUCT_TYPE_INFO: {
      auto child_types_size = deserializer.Read<uint32_t>();
      for(uint32_t i = 0; i < child_types_size; i++) {
        deserializer.Read<std::string>();
        // TODO: 这里有递归
      }
      extra_info.type = type;
      break;

    }
//    case ExtraTypeInfoType::USER_TYPE_INFO:
//      extra_info = UserTypeInfo::Deserialize(reader);
//      break;
//    case ExtraTypeInfoType::ENUM_TYPE_INFO: {
//      auto enum_size = deserializer.Read<uint32_t>();
//      auto enum_internal_type = EnumVectorDictType(enum_size);
//      switch (enum_internal_type) {
//        case PhysicalType::UINT8:
//          extra_info = EnumTypeInfoTemplated<uint8_t>::Deserialize(reader, enum_size);
//          break;
//        case PhysicalType::UINT16:
//          extra_info = EnumTypeInfoTemplated<uint16_t>::Deserialize(reader, enum_size);
//          break;
//        case PhysicalType::UINT32:
//          extra_info = EnumTypeInfoTemplated<uint32_t>::Deserialize(reader, enum_size);
//          break;
//        default:
//          return extra_info;
//      }
//    } break;
//    case ExtraTypeInfoType::AGGREGATE_STATE_TYPE_INFO:
//      extra_info = AggregateStateTypeInfo::Deserialize(reader);
//      break;
    default:
      return extra_info;
  }
  auto alias = deserializer.Read<std::string>();
  extra_info.alias = alias;
  return extra_info;
}

TypeInfo TypeInfo::ReadTypeInfo(Deserializer &deserializer) {
  ReadFieldHeader(deserializer);
  TypeInfo info{};
  info.schema = deserializer.Read<std::string>();
  info.name = deserializer.Read<std::string>();
  ReadFieldHeader(deserializer);
  info.type = deserializer.Read<LogicalTypeId>();
}

Schema Schema::Read(Deserializer &deserializer) {
  Schema schema{};
  // field1
  ReadFieldHeader(deserializer);
  schema.schema_name = deserializer.Read<std::string>();
  // field2
  ReadFieldHeader(deserializer);
  schema.enum_count = deserializer.Read<uint32_t>();
  schema.seq_count = deserializer.Read<uint32_t>();
  schema.table_count = deserializer.Read<uint32_t>();
  return schema;
}

}
}
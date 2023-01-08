//
// Created by Shiping Yao on 2023/1/4.
//

#ifndef INDEX_EXPLORER_METABLOCKREADER_H
#define INDEX_EXPLORER_METABLOCKREADER_H

#include <cstdint>
#include <string>
#include <unistd.h>
#include "serializer.h"

namespace explorer {
  namespace duckdb {

    class File : public Deserializer {
    public:
      explicit File(int fd) : fd_(fd), offset_(0) {}

      File(int fd, idx_t off) : fd_(fd), offset_(off) {}

      explicit File(const std::string &file_path);

      ~File() {
        if (fd_ > 0) {
          ::close(fd_);
        }
      }

      int FD() {
        return fd_;
      }

      idx_t read(data_ptr_t data, idx_t size);
      idx_t read(data_ptr_t data, idx_t size, idx_t offset);
      void ReadData(data_ptr_t buffer, idx_t read_size) override;

      void seek(idx_t offset) {
        offset_ = offset;
      }

    private:
      int fd_;
      idx_t offset_;

    };

    struct Storage {
      //! The size of a hard disk sector, only really needed for Direct IO
      constexpr static int SECTOR_SIZE = 4096;
      //! Block header size for blocks written to the storage
      constexpr static int BLOCK_HEADER_SIZE = sizeof(uint64_t);
      // Size of a memory slot managed by the StorageManager. This is the quantum of allocation for Blocks on DuckDB. We
      // default to 256KB. (1 << 18)
      constexpr static int BLOCK_ALLOC_SIZE = 262144;
      //! The actual memory space that is available within the blocks
      constexpr static int BLOCK_SIZE = BLOCK_ALLOC_SIZE - BLOCK_HEADER_SIZE;
      //! The size of the headers. This should be small and written more or less atomically by the hard disk. We default
      //! to the page size, which is 4KB. (1 << 12)
      constexpr static int FILE_HEADER_SIZE = 4096;

      static constexpr uint64_t BLOCK_START = FILE_HEADER_SIZE * 3;
    };


    struct MainHeader {
      static constexpr idx_t MAGIC_BYTE_SIZE = 4;
      static constexpr idx_t MAGIC_BYTE_OFFSET = Storage::BLOCK_HEADER_SIZE;
      static constexpr idx_t FLAG_COUNT = 4;

      static const char MAGIC_BYTES[];

      static void CheckMagicBytes(File &file);
    };

    struct DatabaseHeader {
      uint64_t iteration;
      //! A pointer to the initial meta block
      block_id_t meta_block;
      //! A pointer to the block containing the free list
      block_id_t free_list;
      //! The number of blocks that is in the file as of this database header. If the file is larger than BLOCK_SIZE *
      //! block_count any blocks appearing AFTER block_count are implicitly part of the free_list.
      uint64_t block_count;

      static DatabaseHeader ReadDatabaseHeader(Deserializer &);
    };

    enum class LogicalTypeId : uint8_t {
      INVALID = 0,
      SQLNULL = 1, /* NULL type, used for constant NULL */
      UNKNOWN = 2, /* unknown type, used for parameter expressions */
      ANY = 3,     /* ANY type, used for functions that accept any type as parameter */
      USER = 4, /* A User Defined Type (e.g., ENUMs before the binder) */
      BOOLEAN = 10,
      TINYINT = 11,
      SMALLINT = 12,
      INTEGER = 13,
      BIGINT = 14,
      DATE = 15,
      TIME = 16,
      TIMESTAMP_SEC = 17,
      TIMESTAMP_MS = 18,
      TIMESTAMP = 19, //! us
      TIMESTAMP_NS = 20,
      DECIMAL = 21,
      FLOAT = 22,
      DOUBLE = 23,
      CHAR = 24,
      VARCHAR = 25,
      BLOB = 26,
      INTERVAL = 27,
      UTINYINT = 28,
      USMALLINT = 29,
      UINTEGER = 30,
      UBIGINT = 31,
      TIMESTAMP_TZ = 32,
      TIME_TZ = 34,
      JSON = 35,

      HUGEINT = 50,
      POINTER = 51,
      // HASH = 52, // deprecated, uses UBIGINT instead
      VALIDITY = 53,
      UUID = 54,

      STRUCT = 100,
      LIST = 101,
      MAP = 102,
      TABLE = 103,
      ENUM = 104,
      AGGREGATE_STATE = 105,
      LAMBDA = 106,
      UNION = 107
    };

    enum class ExtraTypeInfoType : uint8_t {
      INVALID_TYPE_INFO = 0,
      GENERIC_TYPE_INFO = 1,
      DECIMAL_TYPE_INFO = 2,
      STRING_TYPE_INFO = 3,
      LIST_TYPE_INFO = 4,
      STRUCT_TYPE_INFO = 5,
      ENUM_TYPE_INFO = 6,
      USER_TYPE_INFO = 7,
      AGGREGATE_STATE_TYPE_INFO = 8
    };

    struct ExtraTypeInfo {
      ExtraTypeInfoType type;
      std::string alias;

      static ExtraTypeInfo Read(Deserializer&);
    };
    struct TypeInfo {
      std::string schema;
      std::string name;
      LogicalTypeId type;

      static TypeInfo ReadTypeInfo(Deserializer&);
    };

    struct Schema {
      uint32_t max_field_count;
      uint64_t total_size;
      std::string schema_name;
      uint32_t enum_count;
      uint32_t seq_count;
      uint32_t table_count;
      uint32_t view_count;
      uint32_t macro_count;
      uint32_t table_macro_count;
      uint32_t table_index_count;

      std::vector<TypeInfo> types;

      static Schema Read(Deserializer&);
    };

    struct SchemaBlock {
      uint64_t checksum;
      block_id_t next_block;
      uint32_t schema_count;
      std::vector<Schema> schemas;
      static SchemaBlock Read(Deserializer&);
    };



  }


  class MetaBlockReader {

  public:
    void ReadData(data_ptr_t *dst, idx_t size);
  };
}


#endif //INDEX_EXPLORER_METABLOCKREADER_H

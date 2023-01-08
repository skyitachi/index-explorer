//
// Created by Shiping Yao on 2023/1/4.
//

#include <iostream>
#include "MetaBlockReader.h"


using namespace explorer;
using namespace duckdb;

int main(int argc, char** argv) {
  if (argc != 2) {
    perror("unexpect args: ./duckdb file_name");
    exit(1);
  }
  duckdb::File file(argv[1]);
  duckdb::MainHeader::CheckMagicBytes(file);

  file.seek(duckdb::Storage::FILE_HEADER_SIZE + sizeof(block_id_t));

  DatabaseHeader header = DatabaseHeader::ReadDatabaseHeader(file);
  std::cout << "iteration: " << header.iteration
            << ", meta_block_id: " << header.meta_block
            << ", free_list: " << header.free_list
            << ", block_count: " << header.block_count << std::endl;

  file.seek(duckdb::Storage::FILE_HEADER_SIZE * 2 + sizeof(block_id_t));
  DatabaseHeader h2 = DatabaseHeader::ReadDatabaseHeader(file);
  std::cout << "iteration: " << h2.iteration
            << ", meta_block_id: " << h2.meta_block
            << ", free_list: " << h2.free_list
            << ", block_count: " << h2.block_count << std::endl;

  file.seek(Storage::BLOCK_START + header.meta_block * Storage::BLOCK_ALLOC_SIZE);
  SchemaBlock schema_block = SchemaBlock::Read(file);

  std::cout << "schema_count: " << schema_block.schema_count << std::endl;
  for(const auto &schema: schema_block.schemas) {
    std::cout <<  ", schema_name: " << schema.schema_name <<
                  ", table_count: " << schema.table_count << std::endl;
  }

  return 0;
}

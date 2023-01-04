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

    class File: public Deserializer {
    public:
      explicit File(int fd): fd_(fd), offset_(0) {}
      File(int fd, idx_t off): fd_(fd), offset_(off) {}
      explicit File(const std::string& file_path);
      ~File() {
        if (fd_ > 0) {
          ::close(fd_);
        }
      }
      int FD() {
        return fd_;
      }

      idx_t Read(data_ptr_t data, idx_t size);
      idx_t Read(data_ptr_t data, idx_t size, idx_t offset);
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
    };

    struct MainHeader {
      static constexpr idx_t MAGIC_BYTE_SIZE = 4;
      static constexpr idx_t MAGIC_BYTE_OFFSET = Storage::BLOCK_HEADER_SIZE;
      static constexpr idx_t FLAG_COUNT = 4;

      static const char MAGIC_BYTES[];

      static void CheckMagicBytes(File& file);
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

      static DatabaseHeader ReadDatabaseHeader(Deserializer&);
    };


  }


  class MetaBlockReader {

  public:
    void ReadData(data_ptr_t* dst, idx_t size);
  };
}


#endif //INDEX_EXPLORER_METABLOCKREADER_H

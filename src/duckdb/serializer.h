//
// Created by Shiping Yao on 2023/1/4.
//

#ifndef INDEX_EXPLORER_SERIALIZER_H
#define INDEX_EXPLORER_SERIALIZER_H

#include <vector>
#include <memory>
#include <string>

#include "storage_info.h"

namespace explorer {
  namespace duckdb {

    class Deserializer {
    private:
      uint64_t version = 0L;

    public:
      virtual ~Deserializer() {
      }

      //! Sets the version of the serialization that readers are expected to use
      //! The version is mostly the most recent one, unless reading old data or streaming from
      //! an older version
      void SetVersion(uint64_t v) {
        this->version = v;
      }

      //! Returns the version of serialization that readers are expected to use
      uint64_t GetVersion() {
        return version;
      }

      //! Reads [read_size] bytes into the buffer
      virtual void ReadData(data_ptr_t buffer, idx_t read_size) = 0;

      template<class T>
      T Read() {
        T value;
        ReadData((data_ptr_t) &value, sizeof(T));
        return value;
      }

      template<class T, typename... ARGS>
      void ReadList(std::vector <std::unique_ptr<T>> &list, ARGS &&...args) {
        auto select_count = Read<uint32_t>();
        for (uint32_t i = 0; i < select_count; i++) {
          auto child = T::Deserialize(*this, std::forward<ARGS>(args)...);
          list.push_back(move(child));
        }
      }

      template<class T, class RETURN_TYPE = T, typename... ARGS>
      std::unique_ptr <RETURN_TYPE> ReadOptional(ARGS &&...args) {
        auto has_entry = Read<bool>();
        if (has_entry) {
          return T::Deserialize(*this, std::forward<ARGS>(args)...);
        }
        return nullptr;
      }

      void ReadStringVector(std::vector <std::string> &list);
    };
  }
}
#endif //INDEX_EXPLORER_SERIALIZER_H

#ifndef BBOLT_INCLUDE_DB_H
#define BBOLT_INCLUDE_DB_H

#include <cstring>

namespace bbolt {

class DB {
public:
	bool Open(const std::string& path);
private:
};

}
#endif

#include <cstdio>
#include <eoskeygen/core/file.h>

namespace eoskeygen {

bool readLines(const std::string& filename, strlist_t& lines) {

	FILE *fd;
	char buf[1024];

	fd = fopen(filename.c_str(), "r");
	if (!fd) {
		return false;
	}

	while(fgets(buf, sizeof(buf), fd) != NULL) {
		std::string line(buf);
		lines.push_back(trim(line));
	}

	fclose(fd);
	return true;
}

} // namespace eoskeygen

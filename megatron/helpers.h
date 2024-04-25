#ifndef HELPERS_H
#define HELPERS_H

#include <string>
#include <vector>

namespace helpers {
    std::string capitalize(std::string str);
    std::vector<std::string> read_file_data(const std::string& filename,
                                            bool include_first_line = false);
    std::vector<std::string> change_separator(std::vector<std::string> data,
                                              char old_separator = ';',
                                              char new_separator = '#');
    std::string merge_lines(const std::vector<std::string> data);
    void write_file_from_data(const std::string filename, const std::vector<std::string> data,
                              bool change_separator = true,
                              bool inline_data = false,
                              char old_separator = ';',
                              char new_separator = '#');
}

#endif  // HELPERS_H

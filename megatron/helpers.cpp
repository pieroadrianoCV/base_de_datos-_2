#include "helpers.h"
#include <iostream>
#include <fstream>

namespace helpers {

    std::string capitalize(std::string str) {
        if (!str.empty()) {
            str[0] = toupper(str[0]);
            transform(str.begin() + 1, str.end(), str.begin() + 1, [](unsigned char c) { return tolower(c); });
        }
        return str;
    }

    std::vector<std::string> read_file_data(const std::string& filename,
                                            bool include_first_line) {
        std::vector<std::string> data;
        std::ifstream file(filename);
        std::string line;
        while (getline(file, line)) {
            if (!include_first_line) {
                include_first_line = false;
                continue;
            }
            data.push_back(line);
        }
        file.close();
        return data;
    }

    std::vector<std::string> change_separator(std::vector<std::string> data,
                                              char old_separator,
                                              char new_separator) {
        for (std::string& line : data) {
            for (char& c : line) {
                if (c == old_separator) {
                    c = new_separator;
                }
            }
        }
        return data;
    }

    std::string merge_lines(const std::vector<std::string> data) {
        std::string result = "";
        std::cout << "Merging lines..." << std::endl;
        std::cout << "Data size: " << data.size() << std::endl;
        std::cout << data[0] << std::endl;
        std::cout << data[1] << std::endl;
        std::cout << data[2] << std::endl;
        for (int i = 0; i < data.size(); i++) {
            std::string buf_result = result;
            std::cout << std::endl;
            result = buf_result + data[i];
            std::cout << "Result: " << result << std::endl;
            std::cout << buf_result << std::endl;
        }
        return result;
    }

    void write_file_from_data(const std::string filename,
                              const std::vector<std::string> data,
                              bool have_change_separator,
                              bool inline_data,
                              char old_separator, char new_separator) {
        std::vector<std::string> data_with_correct_separator = data;
        if (have_change_separator)
            data_with_correct_separator = change_separator(data, old_separator, new_separator);
        std::ofstream file(filename, std::ios::app);
        if (inline_data) {
            std::cout << "Writing data to file: " << filename << std::endl;
            std::string line = merge_lines(data_with_correct_separator);
            std::cout << line << std::endl;
            file << line << std::endl;
        }
        else {
            for (const std::string& line : data_with_correct_separator) {
                file << line;
            }
        }
        file.close();
    }
}

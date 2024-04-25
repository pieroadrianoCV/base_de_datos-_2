#include "megatron.h"
#include <iostream>
#include <fstream>


Schema::Schema(std::string name, std::string file_name) {
    this -> name = name;
    std::vector<std::string> data = read_file_data(file_name, true);
    write_file_from_data(name, data, true, true);
}

void Schema::add_column(const std::string& name, const std::string& type) {
    columns.push_back({name, type});
}

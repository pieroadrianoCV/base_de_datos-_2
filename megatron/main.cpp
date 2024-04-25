#include "megatron.h"
#include <iostream>

int main()
{
    // std::string filename = "../static/relacion_estudiantes.csv";
    // std::vector<std::string> data = read_file_data(filename, false);
    // for (const std::string& line : data) {
    //     std::cout << line << std::endl;
    // }

    // std::vector<std::string> data = {"nombre;string", "dpto;string", "id;int"};
    // std::vector<std::string> result = change_separator(data);
    // for (const std::string& line : result) {
    //     std::cout << line;
    // }
    // for (const std::string& line : data) {
    //     std::cout << line << std::endl;
    // }
    Schema schema("Esquemas", "../static/relacion_estudiantes.csv");
    return 0;
}

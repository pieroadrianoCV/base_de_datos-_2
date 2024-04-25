#ifndef MEGATRON_H
#define MEGATRON_H

#include "helpers.h"
#include <vector>

using namespace helpers;
#define name_esquema "Esquema"
#define name_db "Megatron 3000"

struct Column {
    std::string name;
    std::string type;
};

class Schema {
public:
    // Schema(std::string name, std::vector<Column> columns = {});
    Schema(std::string name, std::string file_name);

    std::string name;
    std::vector<Column> columns;

    void save_squema();
    void add_column(const std::string&, const std::string&);
    void print_schema();
};

class Database {
public:
    Database(Schema schema, std::string name = name_db) : name(name), schema(schema) {}
    std::string name;
    Schema schema;

    void add_schema(const Schema&);
    void reload_db();
    void print_prompt();
};

#endif  // MEGATRON_H

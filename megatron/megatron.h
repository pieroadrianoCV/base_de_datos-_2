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
    Schema(std::string name = name_esquema, std::vector<Column> columns = {}) : name(name), columns(columns) {}
    std::string name;
    std::vector<Column> columns;

    void add_column(const std::string&, const std::string&);
};

class Database {
public:
    Database(std::string name = name_db, Schema schema) : name(name), schema(schema) {}
    std::string name;
    Schema schema;

    void add_schema(const Schema&);
};

#endif  // MEGATRON_H

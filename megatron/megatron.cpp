#include "megatron.h"

void Schema::add_column(const std::string& name, const std::string& type) {
    columns.push_back({name, type});
}

#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>

class Database {
public:
    static std::string determinarTipoDato(const std::string& valor);
    static std::string toLowercase(std::string str);
    static std::string capitalize(std::string& str);
    static std::vector<std::string> get_type_from_line(std::string line);
    static bool validate(std::string nombre_relacion, std::string archivo_relacion);
    static void create_esquema(std::string relacion_nombre = "");
    static bool existe_relacion(std::string relacion);
    static void create_relacion();
    static std::string encontrar_relacion(std::string nombre_relacion);
    static void consultas_datos(std::string nombre_relacion);
    static std::vector<std::string> consultar_columnas(std::string nombre_columna);
    template <typename... Args>
    static std::vector<std::string> consultar_columnas(std::string nombre_columna, Args... args);
    static std::vector<std::string> consultas_tablas(std::string nombre_relacion, std::string columna);
    static void mezclar_data(std::vector<std::vector<std::string>> data);
    static void consultas_tablas_final(std::string nombre_relacion, std::vector<std::string> columnas);
    static void where(std::string relacion, std::string data_string, std::string simbolo, int compare);
    static void where_condition(std::string relacion, std::string data_string, std::string simbolo, int compare, std::string sign,std::string new_name_relation);
    static void init();
};

#endif // DATABASE_H

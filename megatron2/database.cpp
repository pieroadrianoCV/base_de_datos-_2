#include "database.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

using namespace std;

#define esquemas "Esquemas"

string Database::determinarTipoDato(const string& valor) {
    if (valor.empty()) {
        return "empty";
    }

    bool soloDigitos = true;
    bool puntoDecimal = false;

    for (char c : valor) {
        if (isdigit(c)) {
            continue;
        } else if (c == '.' && !puntoDecimal) {
            puntoDecimal = true;
        } else {
            soloDigitos = false;
            break;
        }
    }

    if (soloDigitos) {
        if (puntoDecimal) {
            return "float";
        } else {
            return "int";
        }
    } else {
        return "string";
    }
}

string Database::toLowercase(string str) {
    transform(str.begin(), str.end(), str.begin(),
              [](unsigned char c) { return tolower(c); });
    return str;
}

string Database::capitalize(string& str) {
    if (!str.empty()) {
        str[0] = toupper(str[0]);
        transform(str.begin() + 1, str.end(), str.begin() + 1, [](unsigned char c) { return tolower(c); });
    }
    return str;
}

vector<string> Database::get_type_from_line(string line) {
    vector<string> types;
    string value;
    stringstream ss(line);
    while (getline(ss, value, ';')) {
        types.push_back(determinarTipoDato(value));
    }
    return types;
}

bool Database::validate(string nombre_relacion, string archivo_relacion) {
    string linea_esquema;
    ifstream archivo_entrada(esquemas); //archivo esquemas -> Estudiantes#nombre#string#id#int#dpto#string
    vector <string> elementos; // ["string", "int", "string"]
    int pos;
    while(getline(archivo_entrada, linea_esquema)){
        string palabra_linea_esquema;
        istringstream linea_esquema_stream(linea_esquema);
        pos = linea_esquema.find(nombre_relacion);
        if (pos == 0) {
            while (getline(linea_esquema_stream, palabra_linea_esquema, '#')) {
                if (pos > 1 && pos % 2 == 0) {
                    elementos.push_back(palabra_linea_esquema);
                }
                pos++;
            }
            break;
        }
    }

    if (pos == -1) return false;

    string linea_data_relacion;
    ifstream data_relacion(archivo_relacion);
    int pos_linea = 0;
    while(getline(data_relacion, linea_data_relacion)) {
        if (pos_linea == 0) {
            pos_linea++;
            continue;
        }
        vector<string> types = get_type_from_line(linea_data_relacion); // ["string", "int", "string"]
        if (!(elementos == types)) return false;
    }
    return true;
}

void Database::create_esquema(string relacion_nombre) {
    string archivo_csv;
    if(relacion_nombre == ""){
        cout << "Enter relationship name: ";
        cin >> relacion_nombre;
    }else{
        cout<<"The relationship will be created:"<<relacion_nombre<<endl;
    }
    relacion_nombre = capitalize(relacion_nombre);
    cout<<"Enter the name of the .csv file to create the schema"<<endl;cin>>archivo_csv;//nombre de la relacion deberia abrir

    ofstream archivo_salida_esquemas(esquemas, ios::app);
    ifstream archivo_entrada(archivo_csv);
    archivo_salida_esquemas << relacion_nombre;

    string linea;
    while (getline(archivo_entrada, linea)) {
        stringstream ss(linea);
        string valor;
        while (getline(ss, valor, ';')) {
            archivo_salida_esquemas << "#";
            archivo_salida_esquemas << valor;
        }
    }
    archivo_salida_esquemas << endl;
    archivo_entrada.close();
    archivo_salida_esquemas.close();
    cout << "Schematics created successfully" << endl;
}

bool Database::existe_relacion(string relacion) {
    ifstream archivo_entrada(esquemas);
    string linea;
    while(getline(archivo_entrada,linea,'#')){
        if(linea == relacion)
            return true;
    }
    return false;
}

void Database::create_relacion() {
    string nombre_archivo_relacion;
    string nombre_relacion;
    cout<<"Enter relationship name: "<<endl;cin>>nombre_relacion;   
    
    if(!existe_relacion(capitalize(nombre_relacion))) {
        cout<<" The relationship does not exist "<<nombre_relacion<<endl;
        create_esquema(nombre_relacion);
    }

    cout<<"Enter file name with .csv to extract data"<<endl;cin>>nombre_archivo_relacion; //pedir al usuario el nombre del archivo a extraer los datos

    ifstream archivo_entrada(nombre_archivo_relacion);
    if (!archivo_entrada.is_open()) {
        cerr << "Error opening CSV file." << endl;
        return;
    }

    string nombre_archivo_salida = capitalize(nombre_relacion); // Estudiantes
    bool validacion = validate(nombre_archivo_salida, nombre_archivo_relacion);
    if (validacion == true) {
        ofstream archivo_salida(nombre_archivo_salida, ios::app); //Estudiantes
        string linea;
        bool primera_linea = true;

        while (getline(archivo_entrada, linea)) { //estudiantes_data.csv -> Piero;123;Lima
            if (primera_linea) {
                primera_linea = false;
                continue; 
            }
            int count_commas = count(linea.begin(), linea.end(), ';');
            int count_hashtag = 0;
            stringstream ss(linea);
            string valor;
            while (getline(ss, valor, ';')) {
                archivo_salida << valor;
                if (count_hashtag < count_commas) {
                    archivo_salida << "#";
                    count_hashtag++;
                }
            }
            archivo_salida << endl;
        }

        archivo_entrada.close();
        archivo_salida.close();
        cout << "Relation created successfully" << endl;
    }
    else {
        cout << "The data does not match the schema" << endl;
    }
}

string Database::encontrar_relacion(string nombre_relacion){
    string linea_esquema;
    ifstream archivo_entrada(esquemas);
    while(getline(archivo_entrada, linea_esquema)){
        if (linea_esquema.find(nombre_relacion) != -1) {
            stringstream ss(linea_esquema);
            int pos = 0;
            while (getline(ss, linea_esquema, '#')) {
                if (pos > 0 && pos % 2 != 0) {
                    cout << linea_esquema << '#';
                }
                pos += 1;
            }
            archivo_entrada.close();
            return linea_esquema;
        }
    }
    archivo_entrada.close();
    return "Not found";
}

void Database::consultas_datos(string nombre_relacion){
    string linea_esquema;
    vector <string> elementos;
    ifstream archivo_entrada(esquemas);
    string linea;
    int pos;

    while(getline(archivo_entrada, linea_esquema)){
        string palabra_linea_esquema;
        istringstream linea_esquema_stream(linea_esquema);
        pos = linea_esquema.find(nombre_relacion);
        if (pos == 0) {
            while (getline(linea_esquema_stream, palabra_linea_esquema, '#')) {
                if (pos > 0 && pos % 2 == 1) {
                    elementos.push_back(palabra_linea_esquema);        
                }
                pos++;
            }
            break;
        }
    }
    int idx = 0;
    for (auto i : elementos) {
        cout << i;
        if (idx < elementos.size() - 1) {
            cout << "\t#\t";
            idx++;
        }
    }
    cout << endl;
    cout << "-----------------------------------------------------" << endl;
    
    ifstream data_relacion(nombre_relacion);
    int idj = 0;
    while(getline(data_relacion, linea_esquema)) {
        // cout << linea_esquema << endl;
        stringstream ss(linea_esquema);
        while (getline(ss, linea_esquema, '#')) {
            cout << linea_esquema;
            if (idj < idx) {
                cout << "\t#\t";
                idj++;
            }
        }
        idj = 0;
        cout << endl;
    } 
}

vector<string> Database::consultar_columnas(string nombre_columna) {
    vector<string> columnas;
    columnas.push_back(nombre_columna);
    return columnas;
}

template <typename... Args>
vector<string> Database::consultar_columnas(string nombre_columna, Args... args) {
    vector<string> columnas;
    columnas.push_back(nombre_columna);
    vector<string> columnas_siguientes = consultar_columnas(args...);
    columnas.insert(columnas.end(), columnas_siguientes.begin(), columnas_siguientes.end());
    return columnas;
}

vector<string> Database::consultas_tablas(string nombre_relacion, string columna) {
    string linea_esquema;
    string linea_data;
    vector <string> elementos;
    ifstream archivo_entrada(esquemas);
    ifstream archivo_data(nombre_relacion);
    string linea;
    int pos;
    int pos_columna;
    while(getline(archivo_entrada, linea_esquema)){
        istringstream linea_esquema_stream(linea_esquema);
        pos = linea_esquema.find(nombre_relacion);
        string columna_esquema;
        pos_columna = 0;
        if (pos == 0) {
            while (getline(linea_esquema_stream, columna_esquema, '#')) {
                if (columna_esquema == columna) {
                    break;
                }
                if (pos % 2 != 0) {
                    pos_columna++;
                }
                pos++;
            }
        }
        if (pos != -1) {
            break;
        }
    }
    int pos_data = 0;
    vector<string> data;
    while(getline(archivo_data, linea_data)) {
        stringstream ss(linea_data);
        string valor;
        while (getline(ss, valor, '#')) {
            if (pos_data == pos_columna) {
                data.push_back(valor);
            }
            pos_data++;
        }
        pos_data = 0;
    }
    archivo_entrada.close();
    return data;
}

void Database::mezclar_data(vector<vector<string>> data) {
    int data_size = data[0].size();
    int aux = 0;
    while (aux < data_size) {
        for (int i = 0; i < data.size(); i++) {
            cout << data[i][aux];
            if (i < data.size() - 1) cout << "\t#\t";
        }
        cout << endl;
        aux++;
    }
}

void Database::consultas_tablas_final(string nombre_relacion, vector<string> columnas) {
    vector<vector<string>> data;
    for (int i = 0; i < columnas.size(); i++) {
        cout << columnas[i];
        if (i < columnas.size() - 1) cout << "\t#\t";
        data.push_back(consultas_tablas(nombre_relacion, columnas[i]));
    }

    cout << endl;
    mezclar_data(data);
}

void Database::where(string relacion, string data_string,string simbolo, int compare) {
    string linea_esquema;
    ifstream nombre_relacion(esquemas);
    int pos;
    int pos_string;
    int pos_columna_data;
    vector <string> elementos;

    while(getline(nombre_relacion , linea_esquema)){
        string palabra_linea_esquema;
        istringstream linea_esquema_stream(linea_esquema);
        pos = linea_esquema.find(relacion);
        pos_string = palabra_linea_esquema.find(data_string);
        int pos_columna=-1;
        
        if (pos == 0) {
            while (getline(linea_esquema_stream , palabra_linea_esquema, '#')) {
                if (pos > 0 && pos % 2 == 1 ) {
                    pos_columna++;
                    if(palabra_linea_esquema == data_string){
                        pos_columna_data = pos_columna;
                    }
                    cout<< palabra_linea_esquema<<" ";
                }
                pos++;
            }
            break;
        }
        
    }
    cout<<endl;
    
    string linea_relacion;
    int pos_relacion=0;
    ifstream data(relacion);
    while(getline( data , linea_relacion )){
        string palabra_linea_relacion;
        istringstream linea_relacion_stream(linea_relacion);
            while (getline(linea_relacion_stream, palabra_linea_relacion, '#')) {
                if (pos_relacion == pos_columna_data) {
                    if(simbolo == ">"){
                        if(stoi(palabra_linea_relacion) > compare){
                            elementos.push_back(linea_relacion);
                        }
                    }
                    else if(simbolo == ">="){
                        if(stoi(palabra_linea_relacion) >= compare){
                            elementos.push_back(linea_relacion);
                        }
                    }
                    else if(simbolo == "<"){
                        if(stoi(palabra_linea_relacion) < compare){
                            elementos.push_back(linea_relacion);
                        }
                    }
                    else if(simbolo == "<="){
                        if(stoi(palabra_linea_relacion) <= compare){
                            elementos.push_back(linea_relacion);
                        }
                    }
                    else if(simbolo == "=="){
                        if(stoi(palabra_linea_relacion) == compare){
                            elementos.push_back(linea_relacion);
                        }
                    }
                    else{
                        cout<< "unsigned";
                    }

                }
                pos_relacion ++;
            }
        pos_relacion=0;
        
    }

     int idx = 0;

    for (auto i : elementos) {
        cout << i;
        if (idx < elementos.size() - 1) {
            cout << endl;
            idx++;
        }
    }
}


void Database::where_condition(string relacion, string data_string,string simbolo, int compare, string sign, string name_new_relation) {
    string linea_esquema;
    ifstream nombre_relacion(esquemas);
    int pos;
    int pos_string;
    int pos_columna_data;
    vector <string> elementos;

    while(getline(nombre_relacion , linea_esquema)){
        string palabra_linea_esquema;
        istringstream linea_esquema_stream(linea_esquema);
        pos = linea_esquema.find(relacion);
        pos_string = palabra_linea_esquema.find(data_string);
        int pos_columna=-1;
        
        if (pos == 0) {
            while (getline(linea_esquema_stream , palabra_linea_esquema, '#')) {
                if (pos > 0 && pos % 2 == 1 ) {
                    pos_columna++;
                    if(palabra_linea_esquema == data_string){
                        pos_columna_data = pos_columna;
                    }
                    cout<< palabra_linea_esquema<<" ";
                }
                pos++;
            }
            break;
        }
        
    }
    cout<<endl;
    
    
    string linea_relacion;
    int pos_relacion=0;
    ifstream data(relacion);
    while(getline( data , linea_relacion )){
        string palabra_linea_relacion;
        istringstream linea_relacion_stream(linea_relacion);
            while (getline(linea_relacion_stream, palabra_linea_relacion, '#')) {
                if (pos_relacion == pos_columna_data) {
                    if(simbolo == ">"){
                        if(stoi(palabra_linea_relacion) > compare){
                            elementos.push_back(linea_relacion);                           
                        }
                    }
                    else if(simbolo == ">="){
                        if(stoi(palabra_linea_relacion) >= compare){
                            elementos.push_back(linea_relacion);                           
                        }
                    }
                    else if(simbolo == "<"){
                        if(stoi(palabra_linea_relacion) < compare){
                            elementos.push_back(linea_relacion);                           
                        }
                    }
                    else if(simbolo == "<="){
                        if(stoi(palabra_linea_relacion) <= compare){
                            elementos.push_back(linea_relacion);
                        }
                    }
                    else if(simbolo == "=="){
                        if(stoi(palabra_linea_relacion) == compare){
                            elementos.push_back(linea_relacion);
                            name_new_relation="Relid";
                        }
                    }
                    else{
                        cout<< "unsigned";
                    }

                }
                pos_relacion ++;
            }
        pos_relacion=0;
        
    }

    if(sign=="|"){
        string linea_esquema2;
        name_new_relation = capitalize(name_new_relation);
        // ifstream archivo_esquemas(esquemas); // Aquí leerá Esquemas
        // ofstream archivo_salida_esquemas(esquemas, ios::app); // Aquí escribirá Esquemas
        std::fstream archivo_esquemas(esquemas, std::ios::in | std::ios::out | std::ios::app);
        archivo_esquemas.seekp(0, std::ios::end);
        archivo_esquemas << name_new_relation << "#"; //R1#
        // archivo_salida_esquemas << name_new_relation << "#"; //R1#

        int idx = 0;
        vector<string> elementos2;
        archivo_esquemas.seekg(0, std::ios::beg);
        while(getline(archivo_esquemas, linea_esquema2)){
            // cout << linea_esquema2 << endl;
            string palabra_linea_esquema;
            istringstream linea_esquema_stream2(linea_esquema2);
            pos = linea_esquema2.find(relacion);
            // if (pos == -1) archivo_esquemas << "nop";
            if (pos == 0) {
                while (getline(linea_esquema_stream2, palabra_linea_esquema, '#')) {
                    if (pos > 0 ) {
                        elementos2.push_back(palabra_linea_esquema);
                        // archivo_esquemas<< palabra_linea_esquema; 
                        // if (pos )
                        // archivo_esquemas<< "#";
                        // archivo_salida_esquemas<< palabra_linea_esquema << "#";
                    }
                    pos++;
                }
                break;
            }
        }
        int idxx = 0;
        for (int i = 0; i < elementos2.size(); i++) {
            archivo_esquemas << elementos2[i];
            if (idxx < elementos2.size() - 1) {
                archivo_esquemas << "#";
                idxx++;
            }
        }
        archivo_esquemas << endl;
        // archivo_salida_esquemas << endl;
        archivo_esquemas.close();

    int idx2 = 0;

    for (auto i : elementos) {
        cout << i;
        if (idx2 < elementos.size() - 1) {
            cout << endl;
            idx2++;
            }
        }
    }
    ofstream archivo_nuevo(name_new_relation);
    for (auto i : elementos) {
        archivo_nuevo << i << endl;
    }
    archivo_nuevo.close();
}

void Database::init() {
    string commands;
    cout<<" % MEGATRON3000 %"<<endl<<endl;
    cout<<"   WELCOME TO MEGATRON 3000!"<<endl<<endl;
    cout<<" Write the command to realize:"<<endl<<endl;
    while(true){
        cin>>commands;
        if(commands == "create_esquema")
            create_esquema(esquemas);
        else if(commands == "create_relacion")
            create_relacion();
        else if(commands == "SELECT*FROM") { //SELECT*FROM Titanic
            string nombre_relacion;
            cin >> nombre_relacion;
            consultas_datos(nombre_relacion);
        }
        else if(commands == "SELECT*FROM_") { //SELECT*FROM_ Estudiantes WHERE id < 64
            string nombre_relacion;
            string string_id;
            string operador;
            string separator;
            string nombre_nueva_relacion;
            int dato_operador;
            cin >> nombre_relacion;
            cout<< "WHERE" << " ";
            cin >> string_id;
            cin >> operador;
            cin >> dato_operador;
            cin >> separator;
            if(separator == "NULL"){
                cout<<endl;
                where(nombre_relacion, string_id, operador , dato_operador ); cout<<endl; 
            }else if(separator == "|"){
                cout<<endl;
                cin >> nombre_nueva_relacion; cout << endl;
                where_condition(nombre_relacion, string_id, operador, dato_operador, separator, nombre_nueva_relacion); cout<< endl;
            }
        }
        else if(commands == "SELECT") { 
            //SELECT id, nombre Estudiantes
            string nombre_relacion;
            vector<string> columnas;
            string columna;
            while (cin >> columna){
                if (columna == "FROM") break;
                columnas.push_back(columna);
            }
            cin >> nombre_relacion;
            consultas_tablas_final(nombre_relacion, columnas);
        }

        else if(commands == "exit")
            break;
        else
            cout<<"Not command"<<endl;
    }
}

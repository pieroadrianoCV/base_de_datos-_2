#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <cctype>
#include <vector>
using namespace std;
#define esquemas "Esquemas"

string determinarTipoDato(const string& valor) {
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

string toLowercase(string str) {
    transform(str.begin(), str.end(), str.begin(),
              [](unsigned char c) { return tolower(c); });
    return str;
}

string capitalize(string& str) {
    if (!str.empty()) {
        str[0] = toupper(str[0]);
        transform(str.begin() + 1, str.end(), str.begin() + 1, [](unsigned char c) { return tolower(c); });
    }
    return str;
}

vector<string> get_type_from_line(string line) {
    vector<string> types;
    string value;
    stringstream ss(line);
    while (getline(ss, value, ';')) {
        types.push_back(determinarTipoDato(value));
    }
    return types;
}

// Sigue fallando en el caso en el que se termine con 1 ; extra
bool operator==(const vector<string>& a, const vector<string>& b) {
    if (a.size() < b.size()) return false;
    if (a.size() > b.size()) return true;

    for (int i = 0; i < a.size(); i++) {
        if (a[i] != b[i] && b[i] != "empty") return false;
    }

    return true;
}

bool validate(string nombre_relacion, string archivo_relacion) {
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

void create_esquema(string relacion_nombre = "") {
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

bool existe_relacion(string relacion) {
    ifstream archivo_entrada(esquemas);
    string linea;
    while(getline(archivo_entrada,linea,'#')){
        if(linea == relacion)
            return true;
    }
    return false;
}

void create_relacion() {
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
    string nombre_archivo_salida = capitalize(nombre_relacion);
    bool validacion = validate(nombre_archivo_salida, nombre_archivo_relacion);
    cout << "Validation: " << validacion << endl;
    if (validacion == true) {
        ofstream archivo_salida(nombre_archivo_salida);

        string linea;
        bool primera_linea = true;

        while (getline(archivo_entrada, linea)) {
            if (primera_linea) {
                primera_linea = false;
                continue; 
            }
            archivo_salida << linea << endl;
        }

        archivo_entrada.close();
        archivo_salida.close();
        cout << "Relaiton created successfully" << endl;
    }
    else {
        cout << "The data does not match the schema" << endl;
    }
}



//mejorar esto
void init(){
    string commands;
    cout<<" % MEGATRON3000 %"<<endl<<endl;
    cout<<"   WELCOME TO MEGATRON 3000!"<<endl<<endl;
    cout<<" Write the command to realize:"<<endl<<endl;
    while(true){
        cin>>commands;
        if(commands == "create_esquema")
            create_esquema();
        else if(commands == "create_relacion")
            create_relacion();
        else if(commands == "exit")
            break;
        else
            cout<<"Not command"<<endl;
    }
}

int main() {
    init();
    return 0;  
}

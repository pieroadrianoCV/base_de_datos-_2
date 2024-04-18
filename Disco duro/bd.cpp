#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>

using namespace std;

string determinarTipoDato(const string& valor) {
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

void create_esquema(string nombre_archivo){
    ofstream archivo_salida_esquemas("Esquemas", ios::app);
    ifstream archivo_entrada(nombre_archivo);
    string relacion_nombre;
    cout<<"nombre de la relacion?";cin>>relacion_nombre;
    archivo_salida_esquemas<<relacion_nombre;
    
    string linea;
    while (getline(archivo_entrada, linea)) {
        stringstream ss(linea); 
        string valor;
        while (getline(ss, valor, ';')) {
            archivo_salida_esquemas << "#";
            archivo_salida_esquemas << valor;
        }
        
        //archivo_salida_esquemas<<linea;
    }
    archivo_salida_esquemas<<endl;
    archivo_entrada.close();
    archivo_salida_esquemas.close();

    cout << "Archivos de texto creados exitosamente." << endl;
}

int main() {
    const string archivo_esquemas = "Esquemas";
    const string archivo1_csv = "Titanic.csv";
    const string archivo_csv = "Titanic2.csv";
    const string estudiantes_csv = "estudiantes.csv";
    const string archivo3_csv = "Titanic control 2024.csv";
    
    create_esquema(archivo_csv);
    create_esquema(archivo1_csv);
    return 0;
}

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
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

string capitalize(string& str) {
    if (!str.empty()) {
        str[0] = toupper(str[0]);
        transform(str.begin() + 1, str.end(), str.begin() + 1, [](unsigned char c) { return tolower(c); });
    }
    return str;
}

void create_esquema(string relacion_nombre = "") {
    string archivo_csv;
    if(relacion_nombre == ""){
        cout << "Ingrese nombre de la relacion ? ";
        cin >> relacion_nombre;
    }else{
        cout<<"Se creara relacion:"<<relacion_nombre<<endl;
    }
    relacion_nombre = capitalize(relacion_nombre);
    cout<<"Enter the file name to create your outline with .csv"<<endl;cin>>archivo_csv;//nombre de la relacion deberia ser

    ofstream archivo_salida_esquemas("Esquemas", ios::app);
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
    cout << "Esquemas creados exitosamente." << endl;
}

bool existe_relacion(string relacion) {
    ifstream archivo_entrada("Esquemas");
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
    cout<<"ingrese nombre de la relacion;";cout<<endl;cin>>nombre_relacion;   
    

    if(!existe_relacion(capitalize(nombre_relacion))) {
        cout<<"No existe la relacion"<<nombre_relacion<<endl;
        create_esquema(nombre_relacion);
    }

    cout<<"Enter the name of the file to extract the data with .csv"<<endl;cin>>nombre_archivo_relacion; //pedir al usuario el nombre del archivo a extraer los datos

    ifstream archivo_entrada(nombre_archivo_relacion);
    if (!archivo_entrada.is_open()) {
        cerr << "Error al abrir el archivo CSV." << endl;
        return;
    }
    string nombre_archivo_salida = nombre_relacion;
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
}

//mejorar esto
void init(){
    int d_continue=0;
    string commands;
    while(true){
        cin>>commands;
        if(commands == "crear_esquema()")
            create_esquema();
        else if(commands == "crear_relacion()")
            create_relacion();
        else if(commands == "exit()")
            break;
        else
            cout<<"comando no valido"<<endl;
    }
    
    // if(d_continue==0){
    //     cout<<"WELCOME TO MEGATRON-3000"<<endl;
    //     create_esquema();    
    //     create_relacion();
    //     cout<<"Will you continue? '0' to continue , '1' to break"<<endl;cin>>d_continue;
    //     init();
    // }
}

int main() {
    init();
    return 0;  
}

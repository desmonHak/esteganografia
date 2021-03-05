#include <fstream>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <iomanip>

#define cabezera_IMG 716

using namespace std;

string _hex(int valor);


int main(){

    static int i = 0;
    string File = "mrdx.jpg";
    cout << "Archivo a leer: "; // mrdx.jpg
    cin >> File;

    fstream MyFile;
    MyFile.open(File, ios::binary | ios::in);

    string line;
    string Data_file = "";

    string data_inssert = "Hola Mundo!\0";
    int final_cabezera = cabezera_IMG + data_inssert.size();

    if (MyFile.is_open()){

        while( getline(MyFile, line) ){

            for (int o = 0; i <= line.size(); i++){

                //if (i == cabezera_IMG){break;}
                if (i <= final_cabezera){

                    if( i >= final_cabezera){ break; }

                    string result = _hex((int)line[o]);

                    Data_file += result+" ";
                
                }
                o++;
            }
        }

    } else {

        cout << "No se pudo abrir el archivo." << endl;
        exit(1);

    }
    MyFile.close();

    MyFile.open(File, ios::binary | ios::in |ios::out );
    string Data_output;
    if(MyFile.is_open()){
        i = 0;
        bool estado_write = true;
        Data_output  = "";
        string Data_output_real = "";
        while( getline(MyFile, line) ){

            for (int o = 0, e = 0; o <= line.size(); i++){
                if (i >= cabezera_IMG && estado_write == true){
                    cout << "cambiando el valor hexadecimal: " << _hex((int)line[o]) << " por el caracter: " << data_inssert[e] << " con valor hexadecimal: " << _hex((int)data_inssert[e]) << endl;
                    line[o] = data_inssert[e];
                    if (i == final_cabezera){ estado_write = false;}
                    e++;
                }
                if (estado_write == true){
                    string result = _hex((int)line[o]);
                    Data_output += result+" ";
                }
                Data_output_real += line[o];
                o++;
            }
        }
        MyFile.close();
        MyFile.open("Salida.jpg", ios::binary | ios::out);
        MyFile << Data_output_real;
        MyFile.close();
    }
    
    int longitud_total = Data_file.size();
    cout << "=========================================" << endl;
    cout << Data_file << endl;
    cout << "=========================================" << endl;

    cout << "-----------------------------------------" << endl;

    cout << "cantidad de bytes que contiene la imagen "<< "0x"+_hex(i)+" == " << i << endl;
    cout << "cabezera a usar: " << cabezera_IMG << endl;
    cout << "longitud mensaje: " << data_inssert.size() << endl;
    cout << "direcion por la que empezar a escribir: 0x"<< _hex((int)cabezera_IMG) << endl;
    cout << "direcion en la que se acabara de escribir: 0x"<< _hex((int)final_cabezera-1) << endl;
    cout << "mensaje a ocultar: " << data_inssert << endl;
    cout << "longitud mensaje: " << ((int)cabezera_IMG - final_cabezera)*-1 << endl;

    cout << "longitud: " << line.size() << endl;
    cout << "longitu final: "<< longitud_total << endl;
    cout << ____FILE_defined << " " << __amd64__  << " " << __FILE__  << " " << __DATE__  << " " << __TIME__  << " " << __LINE__ << endl;

    cout << "-----------------------------------------" << "\n\n";
    cout << "=========================================" << endl;
    cout << Data_output << endl;
    cout << "=========================================" << endl;


    

    return 0;
}
string _hex(int valor){
    stringstream stream;
    stream << hex << (int)valor;
    string result(stream.str());
    if (result.size() == 8){
        result[0] = '\0';
        result[1] = '\0';
        result[2] = '\0';
        result[3] = '\0';
        result[4] = '\0';
        result[5] = '\0';
    }
    if (result.size() == 1){
        result = "0"+result;

    }
    return result;
}

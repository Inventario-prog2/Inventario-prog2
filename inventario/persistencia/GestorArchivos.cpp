#include "GestorArchivos.hpp"

bool GestorArchivos::prepararArchivo(string ruta) {
    ifstream test(ruta, ios::binary);
    if (test) {
        test.close();
        return true; // Ya existe, no hacemos nada
    }

    // Si no existe, creamos el archivo con el Header inicial
    ofstream f(ruta, ios::binary);
    if (!f) return false;

    ArchivoHeader h = {1, 0, 0}; 
    f.write(reinterpret_cast<const char*>(&h), sizeof(ArchivoHeader));
    f.close();
    return true;
}

ArchivoHeader GestorArchivos::leerHeader(string ruta) {
    ArchivoHeader h = {0, 0, 0};
    ifstream f(ruta, ios::binary);
    if (f) {
        f.read(reinterpret_cast<char*>(&h), sizeof(ArchivoHeader));
        f.close();
    }
    return h;
}

bool GestorArchivos::guardarHeader(string ruta, ArchivoHeader header) {
    fstream f(ruta, ios::binary | ios::in | ios::out);
    if (!f) return false;

    // El Header siempre está en el byte 0
    f.seekp(0, ios::beg);
    f.write(reinterpret_cast<const char*>(&header), sizeof(ArchivoHeader));
    f.close();
    return true;
}

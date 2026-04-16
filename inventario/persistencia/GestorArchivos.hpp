#ifndef GESTORARCHIVOS_HPP
#define GESTORARCHIVOS_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

using namespace std;

// Estructura de control para la cabecera (Header)
struct ArchivoHeader {
    int proximoID;        // Siguiente ID correlativo a asignar
    int cantidadTotal;    // Cuántos registros hay físicamente (incluyendo eliminados)
    int registrosActivos; // Cuántos registros no están marcados como eliminados
};

class GestorArchivos {
public:
    // --- Gestión de Archivos y Headers ---
    static bool prepararArchivo(string ruta);
    static ArchivoHeader leerHeader(string ruta);
    static bool guardarHeader(string ruta, ArchivoHeader header);
    template <typename T>
    static bool escribirRegistro(string ruta, T objeto) {
        ofstream f(ruta, ios::binary | ios::app);
        if (!f) return false;
        f.write(reinterpret_cast<const char*>(&objeto), sizeof(T));
        f.close();
        return true;
    }

    // Lee un registro por su índice (0, 1, 2...)
    template <typename T>
    static bool leerPorIndice(string ruta, int indice, T &objeto) {
        ifstream f(ruta, ios::binary);
        if (!f) return false;

        // Desplazamiento: Saltamos el Header y los registros anteriores
        long pos = sizeof(ArchivoHeader) + (indice * sizeof(T));
        f.seekg(pos, ios::beg);
        f.read(reinterpret_cast<char*>(&objeto), sizeof(T));
        
        bool exito = f.gcount() > 0;
        f.close();
        return exito;
    }

    // Sobreescribe un registro en una posición específica
    template <typename T>
    static bool actualizarPorIndice(string ruta, int indice, T objeto) {
        fstream f(ruta, ios::binary | ios::in | ios::out);
        if (!f) return false;

        long pos = sizeof(ArchivoHeader) + (indice * sizeof(T));
        f.seekp(pos, ios::beg);
        f.write(reinterpret_cast<const char*>(&objeto), sizeof(T));
        f.close();
        return true;
    }
};

#endif

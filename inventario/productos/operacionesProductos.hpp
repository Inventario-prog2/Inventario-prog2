#ifndef OPERACIONESPRODUCTOS_HPP
#define OPERACIONESPRODUCTOS_HPP

#include "Producto.hpp"
#include "../persistencia/GestorArchivos.hpp"
#include <iostream>

using namespace std;

class OperacionesProducto {
public:
    static void menu();
    static void crear();
    static void buscar();
    static void actualizar();
    static void eliminar();
    static void listar();
    
    // Lógica de negocio (Abstracción)
    static bool actualizarStock(int idProd, int cambio);
    static bool existeProducto(int id, Producto &p);
    static bool codigoDuplicado(const char* codigo, int idIgnorar = -1);
};

#endif

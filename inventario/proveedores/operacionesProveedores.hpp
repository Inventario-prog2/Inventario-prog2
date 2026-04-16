#ifndef OPERACIONES_PROVEEDORES_HPP
#define OPERACIONES_PROVEEDORES_HPP

#include "Proveedor.hpp"
#include "../persistencia/GestorArchivos.hpp"
#include "../utilidades/Validaciones.hpp"
#include "../utilidades/Formatos.hpp"

class OperacionesProveedor {
public:
    static void menu();
    static void agregar();
    static void listar();
    static void buscar();
    static void modificar();
    static void eliminar();
    
    // Utilidad interna para búsquedas
    static bool buscarPorID(int id, Proveedor* resultado);
    static bool existeProveedor(int id, Proveedor &p);
};

#endif

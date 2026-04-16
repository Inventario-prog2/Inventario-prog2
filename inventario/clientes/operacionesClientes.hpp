#ifndef OPERACIONES_CLIENTES_HPP
#define OPERACIONES_CLIENTES_HPP

#include "cliente.hpp"
#include "../persistencia/GestorArchivos.hpp"
#include "../utilidades/Validaciones.hpp"
#include "../utilidades/Formatos.hpp"

class OperacionesCliente {
public:
    static void menu();
    static void agregar();
    static void listar();
    static void buscar();
    static void modificar();
    static void eliminar();

    // Utilidad para el módulo de Ventas
    static bool buscarPorCedula(const char* cedula, Cliente* resultado);
    static bool existeCliente(int id, Cliente &c);
};

#endif

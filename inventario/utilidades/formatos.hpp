#ifndef FORMATOS_HPP
#define FORMATOS_HPP

#include <iostream>
#include <string>
#include <windows.h>
#include <limits>
#include <iomanip>
using namespace std;

// --- DECLARACIONES ADELANTADAS ---
// Esto le dice al compilador: "Estas clases existen, no te asustes"
class Producto;
class Cliente;
class Proveedor;

class Formatos {
public:
    // --- Estética de Consola ---
    static void limpiar();
    static void pausa();
    static void titulo(string texto);
    static void linea(int ancho, char simbolo = '-');

    // --- Tablas de Productos ---
    static void encabezadoProductos();
    // Ahora el compilador aceptara 'p' y 'c' porque ya sabe que son clases
    static void filaProducto(const Producto& p, const char* nombreProv);
    static void pieProductos();

    // --- Tablas de Clientes ---
    static void encabezadoClientes();
    static void filaCliente(const Cliente& c); 
    static void pieClientes();

    // --- Tablas de Proveedores ---
    static void encabezadoProveedores();
    static void filaProveedor(const Proveedor& p);
    static void pieProveedores();
};

#endif

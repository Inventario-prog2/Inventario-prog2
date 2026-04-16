#ifndef OPERACIONES_TRANSACCIONES_HPP
#define OPERACIONES_TRANSACCIONES_HPP

#include "Transaccion.hpp"
#include "../tienda/Tienda.hpp"
#include "../productos/operacionesProductos.hpp"
#include "../clientes/operacionesClientes.hpp"
#include "../proveedores/operacionesProveedores.hpp"

class OperacionesTransaccion {
public:
    static void menu(Tienda &tienda);
    static void registrarVenta(Tienda &miTienda); // Afecta Tienda y Stock
    static void registrarCompra(Tienda &miTienda); // Afecta Tienda y Stock
    static void listarHistorial();
    
    // Utilidad para reportes
    static void mostrarDetalleFactura(const Transaccion& t);
};

#endif

#include "Interfaz.hpp"
#include "../utilidades/Formatos.hpp"
#include "../utilidades/Validaciones.hpp"
#include "../productos/operacionesProductos.hpp"
#include "../proveedores/operacionesProveedores.hpp"
#include "../clientes/operacionesClientes.hpp"
#include "../transacciones/operacionesTransacciones.hpp"

void Interfaz::menuPrincipal(Tienda &tienda) {
    int opcion;

    do {
        Formatos::limpiar();
        // Podemos mostrar el nombre de la tienda dinámicamente
        Formatos::titulo(tienda.getNombre());

        cout << "1. Gestion de Productos\n";
        cout << "2. Gestion de Proveedores\n";
        cout << "3. Gestion de Clientes\n";
        cout << "4. Gestion de Transacciones\n";
        cout << "5. Estado de la Tienda (Caja/Balance)\n";
        cout << "0. Salir\n\n";

        opcion = Validaciones::solicitarEnteroNoNegativo("Seleccione una opcion: ");

        switch (opcion) {
            case 1: OperacionesProducto::menu();     break;
            case 2: OperacionesProveedor::menu();    break;
            case 3: OperacionesCliente::menu();      break;
            case 4: OperacionesTransaccion::menu(tienda); break; // Pasamos la tienda para ventas
            case 5: tienda.mostrarEstado();          break;
            case 0: cout << "Saliendo y asegurando datos...\n"; break;
            default: cout << "Opcion invalida.\n";
        }

        if (opcion != 0) Formatos::pausa();

    } while (opcion != 0);
}

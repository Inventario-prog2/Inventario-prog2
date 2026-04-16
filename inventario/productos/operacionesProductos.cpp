#include "operacionesProductos.hpp"
#include "../utilidades/Validaciones.hpp"
#include "../persistencia/GestorArchivos.hpp"
#include "../utilidades/Formatos.hpp"
#include <iostream>
#include <cstring>

using namespace std;

void OperacionesProducto::crear() {
    if (!Validaciones::confirmar("¿Desea registrar un nuevo producto? (S/N): ")) return;

    ArchivoHeader h = GestorArchivos::leerHeader("datos/productos.bin");
    Producto nuevo;
    char buffer[200];
    float tempFloat;
    int tempInt;

    // --- Validación de Código ---
    // PREFIJO y ARGUMENTOS: Validaciones:: y el tamaño del buffer
    Validaciones::solicitarString("Ingrese código: ", buffer, 20);
    
    // Cambiado a codigoProductoDuplicado según tu .hpp
    if (Validaciones::codigoProductoDuplicado(buffer)) {
        cout << "ERROR: Código ya existe.\n";
        return;
    }
    nuevo.setCodigo(buffer);
    nuevo.setId(h.proximoID); 

    // --- Datos básicos ---
    Validaciones::solicitarString("Ingrese nombre: ", buffer, 100);
    nuevo.setNombre(buffer);
    
    // Usamos las entradas seguras que definimos en Validaciones
    tempFloat = Validaciones::solicitarFloatPositivo("Ingrese precio: ");
    nuevo.setPrecio(tempFloat);
    
    tempInt = Validaciones::solicitarEnteroNoNegativo("Ingrese stock inicial: ");
    nuevo.setStock(tempInt);

    nuevo.setEliminado(false);

    // --- Persistencia Delegada ---
    if (GestorArchivos::escribirRegistro("datos/productos.bin", nuevo)) {
        h.proximoID++;
        h.cantidadTotal++;
        h.registrosActivos++;
        GestorArchivos::guardarHeader("datos/productos.bin", h);
        cout << "Producto registrado con ID: " << nuevo.getId() << endl;
    }
}

void OperacionesProducto::buscar() {
    // Usamos solicitarEntero para mayor robustez
    int idBuscado = Validaciones::solicitarEnteroPositivo("Ingrese ID a buscar: ");
    
    Producto p;
    if (existeProducto(idBuscado, p)) {
        p.mostrarInformacionCompleta();
    } else {
        cout << "Producto no encontrado o eliminado.\n";
    }
}

bool OperacionesProducto::existeProducto(int id, Producto &p) {
    ArchivoHeader h = GestorArchivos::leerHeader("datos/productos.bin");
    for (int i = 0; i < h.cantidadTotal; i++) {
        if (GestorArchivos::leerPorIndice("datos/productos.bin", i, p)) {
            if (!p.isEliminado() && p.getId() == id) return true;
        }
    }
    return false;
}

void OperacionesProducto::listar() {
    ArchivoHeader h = GestorArchivos::leerHeader("datos/productos.bin");
    Producto p;
    
    cout << "\n=== LISTADO DE PRODUCTOS ===\n";
    for (int i = 0; i < h.cantidadTotal; i++) {
        if (GestorArchivos::leerPorIndice("datos/productos.bin", i, p)) {
            if (!p.isEliminado()) {
                p.mostrarInformacionBasica();
            }
        }
    }
}

void OperacionesProducto::eliminar() {
    int id = Validaciones::solicitarEnteroPositivo("ID del producto a eliminar: ");
    
    ArchivoHeader h = GestorArchivos::leerHeader("datos/productos.bin");
    Producto p;
    for (int i = 0; i < h.cantidadTotal; i++) {
        GestorArchivos::leerPorIndice("datos/productos.bin", i, p);
        if (!p.isEliminado() && p.getId() == id) {
            p.mostrarInformacionCompleta();
            // PREFIJO: Validaciones::
            if (Validaciones::confirmar("¿Seguro que desea eliminarlo? (S/N): ")) {
                p.setEliminado(true);
                GestorArchivos::actualizarPorIndice("datos/productos.bin", i, p);
                
                h.registrosActivos--;
                GestorArchivos::guardarHeader("datos/productos.bin", h);
                cout << "Eliminado exitosamente.\n";
            }
            return;
        }
    }
    cout << "No se encontró el producto.\n";
}

bool OperacionesProducto::actualizarStock(int idProd, int cambio) {
    ArchivoHeader h = GestorArchivos::leerHeader("datos/productos.bin");
    Producto p;
    for (int i = 0; i < h.cantidadTotal; i++) {
        GestorArchivos::leerPorIndice("datos/productos.bin", i, p);
        if (!p.isEliminado() && p.getId() == idProd) {
            int nuevoStock = p.getStock() + cambio;
            if (nuevoStock < 0) return false;
            
            p.setStock(nuevoStock);
            return GestorArchivos::actualizarPorIndice("datos/productos.bin", i, p);
        }
    }
    return false;
}

void OperacionesProducto::menu() {
    int opcion;
    do {
        Formatos::limpiar();
        Formatos::titulo("GESTION DE PRODUCTOS");
        cout << "1. Registrar nuevo producto\n";
        cout << "2. Buscar producto\n";
        cout << "3. Actualizar producto\n";
        cout << "4. Actualizar stock manualmente\n";
        cout << "5. Listar todos los productos\n";
        cout << "6. Eliminar producto\n";
        cout << "0. Volver al menu principal\n\n";

        opcion = Validaciones::solicitarEnteroNoNegativo("Seleccione una opcion: ");

        switch (opcion) {
            case 1: crear(); break;
            case 2: buscar(); break;
            case 3: /* implementar modificar() similar a clientes */ break; 
            case 4: {
                Formatos::titulo("AJUSTE MANUAL DE STOCK");
                int id = Validaciones::solicitarEnteroPositivo("ID Producto: ");
                int cant = Validaciones::solicitarEnteroPositivo("Cantidad a sumar (+) o restar (-): ");
                
                if (actualizarStock(id, cant)) {
                    cout << "\n[OK] Stock actualizado correctamente en disco.\n";
                } else {
                    cout << "\n[ERROR] No se pudo encontrar el producto o el stock resultaria negativo.\n";
                }
                Formatos::pausa();
                break;
            }
            case 5: listar(); break;
            case 6: eliminar(); break;
        }
    } while (opcion != 0);
}

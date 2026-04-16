#include "operacionesClientes.hpp"
#include "../utilidades/Validaciones.hpp"
#include "../utilidades/Formatos.hpp"
#include "../persistencia/GestorArchivos.hpp"
#include <iostream>
#include <cstring>

using namespace std;

void OperacionesCliente::menu() {
    int opcion;
    do {
        Formatos::limpiar();
        Formatos::titulo("GESTION DE CLIENTES");
        cout << "1. Registrar cliente\n";
        cout << "2. Buscar cliente\n";
        cout << "3. Actualizar cliente\n";
        cout << "4. Listar clientes\n";
        cout << "5. Eliminar cliente\n";
        cout << "0. Volver al menu principal\n\n";
        
        // CORRECCIÓN: Asegúrate que en Validaciones.cpp el nombre sea exacto
        opcion = Validaciones::solicitarEnteroNoNegativo("Seleccione una opcion: ");

        switch (opcion) {
            case 1: agregar(); break;
            case 2: buscar(); break;
            case 3: modificar(); break;
            case 4: listar(); break;
            case 5: eliminar(); break;
        }
    } while (opcion != 0);
}

void OperacionesCliente::agregar() {
    if (!Validaciones::confirmar("¿Desea registrar un nuevo cliente? (S/N): ")) return;

    Formatos::limpiar();
    Formatos::titulo("NUEVO CLIENTE");

    Cliente nuevo;
    char buffer[200];
    ArchivoHeader header = GestorArchivos::leerHeader("datos/clientes.bin");

    while (true) {
        // CORRECCIÓN: Agregamos nullptr para el validador opcional
        Validaciones::solicitarString("Ingrese cedula/RIF (o CANCELAR): ", buffer, 20, nullptr);
        if (strcmp(buffer, "CANCELAR") == 0) return;
        
        if (Validaciones::cedulaClienteDuplicada(buffer)) {
            cout << "ERROR: La cedula '" << buffer << "' ya esta registrada.\n";
        } else {
            nuevo.setCedula(buffer);
            break;
        }
    }

    Validaciones::solicitarString("Ingrese nombre completo: ", buffer, 100, nullptr);
    nuevo.setNombre(buffer);

    Validaciones::solicitarString("Ingrese email: ", buffer, 100, Validaciones::emailValido, "ERROR: Email invalido.\n");
    nuevo.setEmail(buffer);

    Validaciones::solicitarString("Ingrese telefono: ", buffer, 20, nullptr);
    nuevo.setTelefono(buffer);

    Validaciones::solicitarString("Ingrese direccion: ", buffer, 200, nullptr);
    nuevo.setDireccion(buffer);

    Validaciones::obtenerFechaActual(buffer);
    nuevo.setFecha(buffer);

    nuevo.setId(header.proximoID);
    nuevo.setEliminado(false);

    if (Validaciones::confirmar("\n¿Guardar cliente en disco? (S/N): ")) {
        if (GestorArchivos::escribirRegistro("datos/clientes.bin", nuevo)) {
            header.proximoID++;
            header.cantidadTotal++;
            header.registrosActivos++;
            GestorArchivos::guardarHeader("datos/clientes.bin", header);
            cout << "\n[OK] Cliente registrado exitosamente.\n";
        }
    }
    Formatos::pausa();
}

// CORRECCIÓN: Implementación de buscar() que faltaba y causaba undefined reference
void OperacionesCliente::buscar() {
    int idBuscado = Validaciones::solicitarEnteroPositivo("Ingrese ID del cliente: ");
    Cliente c;
    if (existeCliente(idBuscado, c)) {
        cout << "\n--- INFORMACION DEL CLIENTE ---\n";
        cout << "ID: " << c.getId() << "\nNombre: " << c.getNombre() << "\nCedula: " << c.getCedula() << endl;
    } else {
        cout << "Cliente no encontrado.\n";
    }
    Formatos::pausa();
}

bool OperacionesCliente::existeCliente(int id, Cliente &c) {
    ArchivoHeader h = GestorArchivos::leerHeader("datos/clientes.bin");
    for (int i = 0; i < h.cantidadTotal; i++) {
        if (GestorArchivos::leerPorIndice("datos/clientes.bin", i, c)) {
            if (!c.isEliminado() && c.getId() == id) return true;
        }
    }
    return false;
}

void OperacionesCliente::modificar() {
    int idBuscado = Validaciones::solicitarEnteroPositivo("ID del cliente a actualizar (0 para cancelar): ");
    if (idBuscado == 0) return;

    ArchivoHeader h = GestorArchivos::leerHeader("datos/clientes.bin");
    Cliente temp;
    bool encontrado = false;
    int indiceEncontrado = -1;

    for (int i = 0; i < h.cantidadTotal; i++) {
        if (GestorArchivos::leerPorIndice("datos/clientes.bin", i, temp)) {
            if (!temp.isEliminado() && temp.getId() == idBuscado) {
                encontrado = true;
                indiceEncontrado = i;
                break;
            }
        }
    }

    if (!encontrado) {
        cout << "ERROR: No existe el cliente.\n";
        Formatos::pausa();
        return;
    }

    char buffer[200];
    cout << "\nModificando a: " << temp.getNombre() << endl;
    
    Validaciones::solicitarString("Nuevo nombre (ENTER mantiene): ", buffer, 100, nullptr);
    if (strlen(buffer) > 0) temp.setNombre(buffer);

    if (Validaciones::confirmar("¿Guardar cambios? (S/N): ")) {
        GestorArchivos::actualizarPorIndice("datos/clientes.bin", indiceEncontrado, temp);
        cout << "Cliente actualizado con exito.\n";
    }
    Formatos::pausa();
}

void OperacionesCliente::eliminar() {
    int id = Validaciones::solicitarEnteroPositivo("ID del cliente a eliminar: ");
    ArchivoHeader h = GestorArchivos::leerHeader("datos/clientes.bin");
    Cliente c;

    for (int i = 0; i < h.cantidadTotal; i++) {
        if (GestorArchivos::leerPorIndice("datos/clientes.bin", i, c)) {
            if (!c.isEliminado() && c.getId() == id) {
                if (Validaciones::confirmar("¿Seguro que desea eliminar este cliente? (S/N): ")) {
                    c.setEliminado(true);
                    GestorArchivos::actualizarPorIndice("datos/clientes.bin", i, c);
                    h.registrosActivos--;
                    GestorArchivos::guardarHeader("datos/clientes.bin", h);
                    cout << "Cliente eliminado.\n";
                }
                Formatos::pausa();
                return;
            }
        }
    }
    cout << "No se encontro el cliente.\n";
    Formatos::pausa();
}

void OperacionesCliente::listar() {
    Formatos::limpiar();
    Formatos::titulo("LISTADO DE CLIENTES");
    
    // Leemos el encabezado para saber cuántos registros hay
    ArchivoHeader h = GestorArchivos::leerHeader("datos/clientes.bin");
    Cliente c;
    int activos = 0;

    // Dibujamos el encabezado de la tabla (desde Formatos)
    Formatos::encabezadoClientes();

    for (int i = 0; i < h.cantidadTotal; i++) {
        // Leemos cada registro por su índice
        if (GestorArchivos::leerPorIndice("datos/clientes.bin", i, c)) {
            // Solo mostramos los que no han sido borrados (borrado lógico)
            if (!c.isEliminado()) {
                Formatos::filaCliente(c);
                activos++;
            }
        }
    }

    // Dibujamos la línea final de la tabla
    Formatos::pieClientes();

    cout << "\nRegistros totales: " << h.cantidadTotal << " | Clientes activos: " << activos << endl;
    Formatos::pausa();
}

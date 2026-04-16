#include "operacionesProveedores.hpp"

// Reutilizamos tu lógica de registro pero usando el Gestor de Archivos
void OperacionesProveedor::agregar() {
    if (!Validaciones::confirmar("¿Desea registrar un nuevo proveedor? (S/N): "))
        return;

    Formatos::limpiar();
    Formatos::titulo("NUEVO PROVEEDOR");

    Proveedor nuevo;
    char buffer[200];
    ArchivoHeader header = GestorArchivos::leerHeader("datos/proveedores.bin");

    // RIF con validación de duplicados
    while (true) {
        Validaciones::solicitarString("Ingrese RIF (o CANCELAR): ", buffer, 20);
        if (strcmp(buffer, "CANCELAR") == 0) return;
        
        if (Validaciones::rifProveedorDuplicado(buffer)) {
            cout << "ERROR: El RIF '" << buffer << "' ya esta registrado.\n";
        } else {
            nuevo.setRif(buffer);
            break;
        }
    }

    Validaciones::solicitarString("Ingrese nombre: ", buffer, 100);
    nuevo.setNombre(buffer);

    Validaciones::solicitarString("Ingrese email: ", buffer, 100, Validaciones::emailValido, "ERROR: Email invalido.\n");
    nuevo.setEmail(buffer);

    Validaciones::solicitarString("Ingrese telefono: ", buffer, 20);
    nuevo.setTelefono(buffer);
    
    Validaciones::solicitarString("Ingrese direccion: ", buffer, 200);
    nuevo.setDireccion(buffer);

    Validaciones::obtenerFechaActual(buffer);
    nuevo.setFechaRegistro(buffer);

    nuevo.setId(header.proximoID);
    nuevo.setEliminado(false);

    if (Validaciones::confirmar("\n¿Guardar proveedor en disco? (S/N): ")) {
        if (GestorArchivos::escribirRegistro("datos/proveedores.bin", nuevo)) {
            cout << "[OK] Proveedor registrado exitosamente.\n";
        }
    }
    Formatos::pausa();
}

void OperacionesProveedor::modificar() {
    int idBuscado = Validaciones::solicitarEnteroPositivo("ID del proveedor a actualizar (0 para cancelar): ");
    if (idBuscado == 0) return;

    FILE* f = fopen("datos/proveedores.bin", "r+b");
    if (!f) return;

    ArchivoHeader h;
    fread(&h, sizeof(ArchivoHeader), 1, f);

    Proveedor temp;
    long posicionRegistro = -1;
    bool encontrado = false;

    for (int i = 0; i < h.cantidadTotal; i++) {
        posicionRegistro = ftell(f); 
        if (fread(&temp, sizeof(Proveedor), 1, f)) {
            if (!temp.isEliminado() && temp.getId() == idBuscado) {
                encontrado = true;
                break;
            }
        }
    }

    if (!encontrado) {
        cout << "ERROR: No existe un proveedor activo con ese ID.\n";
        fclose(f);
        Formatos::pausa();
        return;
    }

    // Lógica de edición (similar a la que pasaste)
    char buffer[200];
    cout << "\nModificando: " << temp.getNombre() << endl;
    
    Validaciones::solicitarString("Nuevo nombre (ENTER mantiene): ", buffer, 100);
    if (strlen(buffer) > 0) temp.setNombre(buffer);

    Validaciones::solicitarString("Nuevo Telefono (ENTER mantiene): ", buffer, 20);
    if (strlen(buffer) > 0) temp.setTelefono(buffer);

    if (Validaciones::confirmar("¿Confirmar cambios? (S/N): ")) {
        fseek(f, posicionRegistro, SEEK_SET);
        fwrite(&temp, sizeof(Proveedor), 1, f);
        cout << "Actualización exitosa.\n";
    }

    fclose(f);
    Formatos::pausa();
}

void OperacionesProveedor::eliminar() {
    int idBuscado = Validaciones::solicitarEnteroPositivo("ID del proveedor a eliminar: ");
    
    if (Validaciones::proveedorExiste(idBuscado)) {
        cout << "ERROR: No se puede eliminar. El proveedor tiene productos asociados.\n";
        Formatos::pausa();
        return;
    }
}

void OperacionesProveedor::menu() {
    int opcion;
    do {
        Formatos::limpiar();
        Formatos::titulo("GESTION DE PROVEEDORES");
        cout << "1. Registrar proveedor\n";
        cout << "2. Buscar proveedor\n";
        cout << "3. Listar proveedores\n";
        cout << "4. Eliminar proveedor\n";
        cout << "0. Volver al menu principal\n\n";

        opcion = Validaciones::solicitarEnteroNoNegativo("Seleccione una opcion: ");

        switch (opcion) {
            case 1: agregar(); break;
            case 2: buscar(); break;
            case 3: listar(); break;
            case 4: eliminar(); break;
        }
    } while (opcion != 0);
}

void OperacionesProveedor::buscar() {
    Formatos::limpiar();
    Formatos::titulo("BUSCAR PROVEEDOR");

    int idBuscado = Validaciones::solicitarEnteroPositivo("Ingrese el ID del proveedor: ");
    
    Proveedor p;
    // Usamos una lógica similar a existeProducto/existeCliente
    if (existeProveedor(idBuscado, p)) {
        cout << "\n[RESULTADO DE LA BUSQUEDA]\n";
        cout << "--------------------------------\n";
        cout << "ID:        " << p.getId() << endl;
        cout << "RIF:       " << p.getRif() << endl;
        cout << "EMPRESA:   " << p.getNombre() << endl;
        cout << "TELEFONO:  " << p.getTelefono() << endl;
        cout << "DIRECCION: " << p.getDireccion() << endl;
        cout << "--------------------------------\n";
    } else {
        cout << "\n[ERROR] Proveedor no encontrado o eliminado.\n";
    }
    Formatos::pausa();
}

void OperacionesProveedor::listar() {
    Formatos::limpiar();
    Formatos::titulo("LISTADO DE PROVEEDORES");

    ArchivoHeader h = GestorArchivos::leerHeader("datos/proveedores.bin");
    Proveedor p;
    int activos = 0;

    // Llamamos al formato de tabla que definimos en Formatos.cpp
    Formatos::encabezadoProveedores();

    for (int i = 0; i < h.cantidadTotal; i++) {
        if (GestorArchivos::leerPorIndice("datos/proveedores.bin", i, p)) {
            if (!p.isEliminado()) {
                Formatos::filaProveedor(p);
                activos++;
            }
        }
    }

    Formatos::pieProveedores();
    cout << "\nProveedores activos encontrados: " << activos << endl;
    Formatos::pausa();
}

// Función auxiliar necesaria para buscar() y para validaciones externas
bool OperacionesProveedor::existeProveedor(int id, Proveedor &p) {
    ArchivoHeader h = GestorArchivos::leerHeader("datos/proveedores.bin");
    for (int i = 0; i < h.cantidadTotal; i++) {
        if (GestorArchivos::leerPorIndice("datos/proveedores.bin", i, p)) {
            if (!p.isEliminado() && p.getId() == id) return true;
        }
    }
    return false;
}

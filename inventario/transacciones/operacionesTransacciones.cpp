#include "operacionesTransacciones.hpp"

// Función auxiliar para registrar en el archivo binario (Abstracción)
void registrarTransaccionEnDisco(Transaccion t) {
    const char* ruta = "datos/transacciones.bin";
    ArchivoHeader h = GestorArchivos::leerHeader(ruta);
    
    t.setId(h.proximoID); // Asignación automática de ID correlativo
    
    if (GestorArchivos::escribirRegistro(ruta, t)) {
        h.cantidadTotal++;
        h.proximoID++;
        h.registrosActivos++;
        GestorArchivos::guardarHeader(ruta, h);
    }
}

void OperacionesTransaccion::registrarCompra(Tienda &miTienda) {
    Formatos::limpiar();
    Formatos::titulo("REGISTRAR COMPRA (ENTRADA DE STOCK)");

    // 1. Validar Producto
    int idProd = Validaciones::solicitarEnteroPositivo("ID del Producto: ");
    Producto p;
    if (!OperacionesProducto::existeProducto(idProd, p)) {
        cout << "[ERROR] Producto no encontrado.\n";
        Formatos::pausa();
        return;
    }

    // 2. Validar Proveedor
    int idProv = Validaciones::solicitarEnteroPositivo("ID del Proveedor: ");
    if (!Validaciones::proveedorExiste(idProv)) {
        cout << "[ERROR] Proveedor no registrado.\n";
        Formatos::pausa();
        return;
    }

    // 3. Datos de la Compra
    int cant = Validaciones::solicitarEnteroPositivo("Cantidad comprada: ");
    float costoUnitario = Validaciones::solicitarFloatPositivo("Costo unitario de compra: ");
    float totalCompra = costoUnitario * cant;

    cout << "\nTotal a pagar al proveedor: $" << fixed << setprecision(2) << totalCompra << endl;

    // 4. Procesar
    if (Validaciones::confirmar("¿Confirmar ingreso de mercancia? (S/N): ")) {
        if (OperacionesProducto::actualizarStock(idProd, cant)) { // Suma stock
            Transaccion t;
            t.setTipo("COMPRA");
            t.setIdRelacionado(idProv);
            t.agregarDetalle(idProd, cant, costoUnitario);
            
            char fecha[11];
            Validaciones::obtenerFechaActual(fecha);
            t.setFecha(fecha);

            registrarTransaccionEnDisco(t); // Usamos tu función auxiliar
            
            // Afectamos la caja de la tienda
            miTienda.restarCompra(totalCompra); 
            miTienda.guardarEnDisco();

            cout << "\n[OK] Compra registrada y stock actualizado.\n";
        }
    }
    Formatos::pausa();
}

void OperacionesTransaccion::registrarVenta(Tienda &miTienda) {
    Formatos::titulo("REGISTRAR VENTA");

    // 1. Validar Producto
    int idProd = Validaciones::solicitarEnteroPositivo("ID del Producto: ");
    Producto p;
if (!OperacionesProducto::existeProducto(idProd, p)) {
        cout << "Error: Producto no encontrado.\n";
        return;
    }

    if (p.getStock() <= 0) {
        cout << "Error: Sin stock disponible.\n";
        return;
    }

    // 2. Validar Cliente
    int idCli = Validaciones::solicitarEnteroPositivo("ID del Cliente: ");
    Cliente cli;
    if (!OperacionesCliente::existeCliente(idCli, cli)) {
        cout << "Error: Cliente no encontrado.\n";
        Formatos::pausa();
        return;
    }

    // 3. Cantidad y Cálculos
    cout << "Stock: " << p.getStock() << " | Precio: $" << p.getPrecio() << endl;
    int cant = Validaciones::solicitarEnteroPositivo("Cantidad a vender: ");

    if (cant > p.getStock()) {
        cout << "Error: Stock insuficiente.\n";
        return;
    }

    float totalVenta = p.getPrecio() * cant;

    // 4. Confirmación y Registro
    if (Validaciones::confirmar("¿Procesar factura? (S/N): ")) {
        // Actualizamos stock (negativo porque sale)
        if (OperacionesProducto::actualizarStock(idProd, -cant)) {
            
            Transaccion t;
            t.setTipo("VENTA");
            t.setIdRelacionado(idCli);
            t.agregarDetalle(idProd, cant, p.getPrecio());
            
            char fecha[11];
            Validaciones::obtenerFechaActual(fecha);
            t.setFecha(fecha);

            registrarTransaccionEnDisco(t);
            
            // AFECTAMOS LA TIENDA (Caja)
            miTienda.sumarVenta(totalVenta);
            miTienda.guardarEnDisco();

            cout << "\n[VENTA EXITOSA] Total: $" << totalVenta << endl;
        }
    }
}

void OperacionesTransaccion::listarHistorial() {
    Formatos::limpiar();
    Formatos::titulo("HISTORIAL DE TRANSACCIONES");

    const char* ruta = "datos/transacciones.bin";
    ArchivoHeader h = GestorArchivos::leerHeader(ruta);
    Transaccion t;

    if (h.cantidadTotal == 0) {
        cout << "No hay transacciones registradas.\n";
        Formatos::pausa();
        return;
    }

    // Encabezado simple para el historial
    cout << left << setw(5) << "ID" 
         << setw(12) << "FECHA" 
         << setw(10) << "TIPO" 
         << setw(15) << "RELACIONADO" 
         << setw(10) << "TOTAL" << endl;
    Formatos::linea(60, '-');

    for (int i = 0; i < h.cantidadTotal; i++) {
        if (GestorArchivos::leerPorIndice(ruta, i, t)) {
            // Suponiendo que tu clase Transaccion tiene estos getters
            cout << left << setw(5) << t.getId() 
                 << setw(12) << t.getFecha() 
                 << setw(10) << t.getTipo() 
                 << setw(15) << t.getIdRelacionado() 
                 << setw(10) << fixed << setprecision(2) << t.getTotal() << endl;
        }
    }

    Formatos::linea(60, '-');
    cout << "Total de registros: " << h.cantidadTotal << endl;
    Formatos::pausa();
}

void OperacionesTransaccion::menu(Tienda &tienda) {
    int opcion;
    do {
        Formatos::limpiar();
        Formatos::titulo("MODULO DE TRANSACCIONES");
        cout << "1. Registrar VENTA (Salida de stock)\n";
        cout << "2. Registrar COMPRA (Entrada de stock)\n";
        cout << "3. Ver historial de transacciones\n";
        cout << "0. Volver al menu principal\n\n";

        opcion = Validaciones::solicitarEnteroNoNegativo("Seleccione una opcion: ");

        switch (opcion) {
            case 1: registrarVenta(tienda); break; // Aquí usas la lógica que corregimos antes
            case 2: registrarCompra(tienda); break;
            case 3: listarHistorial(); break;
        }
    } while (opcion != 0);
}

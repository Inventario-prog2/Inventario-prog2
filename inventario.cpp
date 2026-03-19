#include <iostream>
#include <ctime> 
#include <cctype>
#include <string>
#include <cstring>
#include <iomanip>
#include <limits>
#include <locale.h>
#include <windows.h>
#include <fstream>

using namespace std;

//1.1 Estructura Producto
struct Producto {
    int id;                    // Identificador Ãºnico (autoincremental)
    char codigo[20];           // CÃ³digo del producto (ej: "PROD-001")
    char nombre[100];          // Nombre del producto
    char descripcion[200];     // DescripciÃ³n del producto
    int idProveedor;           // ID del proveedor asociado
    float precio;              // Precio unitario
    int stock;                 // Cantidad en inventario
    char fechaRegistro[11];    // Formato: YYYY-MM-DD
    
    int stockMinimo;                 
    int totalVendido;                
    bool eliminado;                  // Borrado lógico
};

//1.2 Estructura Proveedor

struct Proveedor {
    int id;                    // Identificador Ãºnico (autoincremental)
    char nombre[100];          // Nombre del proveedor
    char rif[20];              // RIF o identificaciÃ³n fiscal
    char telefono[20];         // TelÃ©fono de contacto
    char email[100];           // Correo electrÃ³nico
    char direccion[200];       // DirecciÃ³n fÃ­sica
    char fechaRegistro[11];    // Formato: YYYY-MM-DD
    bool eliminado;                  // Borrado lógico
};

//1.3 Estructura Cliente

struct Cliente {
    int id;                    // Identificador Ãºnico (autoincremental)
    char nombre[100];          // Nombre completo del cliente
    char cedula[20];           // CÃ©dula o RIF
    char telefono[20];         // TelÃ©fono de contacto
    char email[100];           // Correo electrÃ³nico
    char direccion[200];       // DirecciÃ³n fÃ­sica
    char fechaRegistro[11];    // Formato: YYYY-MM-DD
    bool eliminado;                  // Borrado lógico
};

//1.4 Estructura TransacciÃ³n (CASO ESPECIAL: Esta estructura puede separarse como se comentÃ³ en clase, tienen libertad de hacerlo.)

struct Transaccion {
    int id;
    char tipo[10];           // "COMPRA" o "VENTA"
    int idRelacionado;       // ID del proveedor o cliente
    
    // Soporte para múltiples productos (Máximo 20 por factura)
    int idProductos[20];    
    int cantidades[20];      
    float precioUnitario[20]; // <-- AGREGA ESTO para guardar el precio de cada item
    int numItems;            // Cuántos de los 20 espacios se usaron
    
    float total;
    char fechaRegistro[11];
    char descripcion[200];
    bool eliminado;          // Borrado lógico
};

//1.5 Estructura Principal: Tienda

struct Tienda {
    char nombre[100];
    char rif[20];
    char direccion[200];
    char telefono[20];
    
    // Estadísticas globales (útiles para reportes)
    float totalVentasHistoricas;
    float totalComprasHistoricas;
    int cantidadOperaciones;
};


//Proyecto 2 importante
struct ArchivoHeader {
    int cantidadRegistros;   // cantidad de elementos guardados
    int proximoID;           // siguiente ID autoincremental
    int registrosActivos;    // normalmente igual a cantidadRegistros
    int version;             // por si luego cambias el formato
};

ArchivoHeader leerHeader(const char* nombreArchivo) {
    ArchivoHeader header = {0, 1, 0, 1}; // Valores por defecto
    FILE* f = fopen(nombreArchivo, "rb");
    if (f) {
        fread(&header, sizeof(ArchivoHeader), 1, f);
        fclose(f);
    } else {
        // Si no existe, lo creamos
        f = fopen(nombreArchivo, "wb");
        fwrite(&header, sizeof(ArchivoHeader), 1, f);
        fclose(f);
    }
    return header;
}

template <typename T>
void inicializarArchivo(const char* nombreArchivo) {
    FILE* f = fopen(nombreArchivo, "rb");
    if (!f) {
        f = fopen(nombreArchivo, "wb");
        if (f) {
            // cantidadRegistros, proximoID, registrosActivos, version
            ArchivoHeader h = {0, 1, 0, 1}; 
            fwrite(&h, sizeof(ArchivoHeader), 1, f);
            fclose(f);
            cout << "Archivo " << nombreArchivo << " inicializado.\n";
        }
    } else {
        fclose(f);
    }
}
//Sobrescribe solo los primeros 16 bytes
void actualizarHeader(const char* nombreArchivo, ArchivoHeader header) {
    FILE* f = fopen(nombreArchivo, "r+b"); // r+b para no borrar el contenido
    if (f) {
        fwrite(&header, sizeof(ArchivoHeader), 1, f);
        fclose(f);
    }
}

//==============
//verificaciones y utilidades
//==============

string toLower(const char* s) {
    string r = s;
    for (char& c : r) c = tolower(c);
    return r;
}

string aString(int n) {
    char buffer[20];
    sprintf(buffer, "%d", n);
    return string(buffer);
}

void convertirAMinusculas(char* cadena) {
    for (int i = 0; cadena[i]; i++)
        cadena[i] = tolower(cadena[i]);
}

bool contiene(const char* campo, const string& filtro) {
    string a = toLower(campo);
    return a.find(filtro) != string::npos;
}

bool codigoProductoDuplicado(const char* codigo, int idIgnorar = -1) {
    FILE* f = fopen("productos.bin", "rb");
    if (!f) return false;

    ArchivoHeader h;
    fread(&h, sizeof(ArchivoHeader), 1, f);

    Producto p;
    // Recorremos todos los registros guardados en el archivo
    for (int i = 0; i < h.cantidadRegistros; i++) {
        if (fread(&p, sizeof(Producto), 1, f)) {
            if (!p.eliminado && p.id != idIgnorar) {
                if (strcmp(p.codigo, codigo) == 0) {
                    fclose(f);
                    return true; // Encontrado en disco
                }
            }
        }
    }

    fclose(f);
    return false; // No hay duplicados
}

void obtenerFechaActual(char* buffer) {
    time_t t = time(nullptr);
    tm* fecha = localtime(&t);
    sprintf(buffer, "%04d-%02d-%02d",
            fecha->tm_year + 1900,
            fecha->tm_mon + 1,
            fecha->tm_mday);
}

bool validarFecha(const char* fecha) {
    if (strlen(fecha) != 10) return false;
    if (fecha[4] != '-' || fecha[7] != '-') return false;

    int anio = atoi(fecha);
    int mes = atoi(fecha + 5);
    int dia = atoi(fecha + 8);

    if (anio < 1900 || anio > 2100) return false;
    if (mes < 1 || mes > 12) return false;
    if (dia < 1 || dia > 31) return false;

    return true;
}

void obtenerNombreProveedorBin(int idBuscado, char* destino) {
    strcpy(destino, "Desconocido");
    FILE* f = fopen("proveedores.bin", "rb");
    if (!f) return;

    ArchivoHeader h;
    fread(&h, sizeof(ArchivoHeader), 1, f);

    Proveedor p;
    while (fread(&p, sizeof(Proveedor), 1, f)) {
        if (!p.eliminado && p.id == idBuscado) {
            strcpy(destino, p.nombre);
            break;
        }
    }
    fclose(f);
}

void solicitarString(const char* mensaje, char* destino, int maxLen,
                     bool (*validador)(const char*) = nullptr,
                     const char* mensajeError = "ERROR: Valor invÃ¡lido.\n") 
{
    while (true) {
        cout << mensaje;
        cin.getline(destino, maxLen);

        if (strlen(destino) == 0) {
            cout << "ERROR: No puede estar vacÃ­o.\n";
            continue;
        }

        if (validador != nullptr && !validador(destino)) {
            cout << mensajeError;
            continue;
        }

        break;
    }
}

bool sinEspacios(const char* texto) {
    for (int i = 0; texto[i] != '\0'; i++)
        if (texto[i] == ' ')
            return false;
    return true;
}


bool emailValido(const char* email) {
    const char* at = strchr(email, '@');
    if (!at) return false;

    const char* punto = strchr(at, '.');
    if (!punto) return false;

    return true;
}

bool rifDuplicado(const char* rif, int idIgnorar = -1) {
    FILE* f = fopen("proveedores.bin", "rb");
    if (!f) return false;

    ArchivoHeader h;
    fread(&h, sizeof(ArchivoHeader), 1, f);

    Proveedor p;
    while (fread(&p, sizeof(Proveedor), 1, f)) {
        if (!p.eliminado && p.id != idIgnorar) {
            if (strcmp(p.rif, rif) == 0) {
                fclose(f);
                return true;
            }
        }
    }
    fclose(f);
    return false;
}

bool clienteDuplicado(const char* cedula, int idIgnorar = -1) {
    FILE* f = fopen("clientes.bin", "rb");
    if (!f) return false;

    ArchivoHeader h;
    fread(&h, sizeof(ArchivoHeader), 1, f);

    Cliente c;
    while (fread(&c, sizeof(Cliente), 1, f)) {
        if (!c.eliminado && c.id != idIgnorar) {
            if (strcmp(c.cedula, cedula) == 0) {
                fclose(f);
                return true;
            }
        }
    }

    fclose(f);
    return false;
}

bool confirmar(const char* mensaje) {
    char op[10];
    cout << mensaje;
    cin >> op;
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // <- clave

    return toupper(op[0]) == 'S';
}


int solicitarEnteroPositivo(const char* mensaje) {
    int valor;
    cout << mensaje;
    cin >> valor;

    while (valor <= 0) {
        cout << "ERROR: Debe ser mayor a 0.\n";
        cout << mensaje;
        cin >> valor;
    }
    return valor;
}
int solicitarEnteroNoNegativo(const char* mensaje) {
    int valor;
    cout << mensaje;
    cin >> valor;

    while (valor < 0) {
        cout << "ERROR: Debe ser >= 0.\n";
        cout << mensaje;
        cin >> valor;
    }
    return valor;
}


bool buscarProductoPorID(int idBuscado, Producto* resultado) {
    FILE* f = fopen("productos.bin", "rb");
    if (!f) return false;

    ArchivoHeader h;
    if (fread(&h, sizeof(ArchivoHeader), 1, f) != 1) {
        fclose(f); return false;
    }

    Producto p;
    while (fread(&p, sizeof(Producto), 1, f)) {
        if (!p.eliminado && p.id == idBuscado) {
            if (resultado != nullptr) {
                *resultado = p; // Copiamos los datos al "contenedor" que nos pasaron
            }
            fclose(f);
            return true;
        }
    }
    fclose(f);
    return false;
}

bool existeProducto(int idBuscado) {
    return buscarProductoPorID(idBuscado, nullptr); // Pasamos nullptr porque solo queremos el bool
}

void mostrarProducto(const Producto& p) {
    cout << "CÃ³digo: " << p.codigo << endl;
    cout << "Nombre: " << p.nombre << endl;
    cout << "DescripciÃ³n: " << p.descripcion << endl;
    cout << "Proveedor ID: " << p.idProveedor << endl;
    cout << "Precio: " << p.precio << endl;
    cout << "Stock: " << p.stock << endl;
    cout << "Fecha: " << p.fechaRegistro << endl;
}

void obtenerNombreProductoBin(int idBuscado, char* destino) {
    strcpy(destino, "Desconocido");
    FILE* f = fopen("productos.bin", "rb");
    if (!f) return;

    ArchivoHeader h;
    fread(&h, sizeof(ArchivoHeader), 1, f);

    Producto p;
    while (fread(&p, sizeof(Producto), 1, f)) {
        if (p.id == idBuscado) {
            strcpy(destino, p.nombre);
            break;
        }
    }
    fclose(f);
}

//================
//tabla producto
//================

//linea horizontal
void linea(const char* tipo) {
    if (strcmp(tipo, "top") == 0)
        cout << "+--------------------------------------------------------------------------+\n";
    else if (strcmp(tipo, "mid") == 0)
        cout << "Â¦--------------------------------------------------------------------------Â¦\n";
    else if (strcmp(tipo, "sep") == 0)
        cout << "Â¦----+-----------+------------------+--------------+-------+--------+------Â¦\n";
    else if (strcmp(tipo, "bot") == 0)
        cout << "+--------------------------------------------------------------------------+\n";
}

//encabezado
void encabezadoProductos() {
    linea("top");
    cout << "Â¦                         LISTADO DE PRODUCTOS                             Â¦\n";
    linea("mid");
    cout << "Â¦ ID Â¦  CÃ³digo   Â¦     Nombre       Â¦  Proveedor   Â¦ PrecioÂ¦ Stock  Â¦ FechaÂ¦\n";
    linea("sep");
}

//fila
void filaProducto(const Producto& p, const char* nombreProv) {
    cout << "Â¦ "
         << setw(2)  << left << p.id << " Â¦ "
         << setw(9)  << left << p.codigo << " Â¦ "
         << setw(16) << left << p.nombre << " Â¦ "
         << setw(12) << left << nombreProv << " Â¦ "
         << setw(5)  << left << p.precio << " Â¦ "
         << setw(6)  << left << p.stock << " Â¦ "
         << setw(4)  << left << p.fechaRegistro << " Â¦\n";
}

void pieProductos() {
    linea("bot");
}

void MostrarProductosPorNombre(const char* filtro) {
    FILE* f = fopen("productos.bin", "rb");
    if (!f) return;

    ArchivoHeader h;
    fread(&h, sizeof(ArchivoHeader), 1, f);

    string filtroMin = toLower(filtro); // Usamos tu utilidad toLower
    Producto p;
    bool encontrado = false;

    encabezadoProductos();
    while (fread(&p, sizeof(Producto), 1, f)) {
        if (!p.eliminado && contiene(p.nombre, filtroMin)) { // Usamos tu utilidad contiene
            char provNombre[50];
            obtenerNombreProveedorBin(p.idProveedor, provNombre);
            filaProducto(p, provNombre);
            encontrado = true;
        }
    }
    pieProductos();

    if (!encontrado) cout << "No se encontraron coincidencias.\n";
    fclose(f);
}

bool productoTieneTransaccionesBin(int idBuscado) {
    FILE* f = fopen("transacciones.bin", "rb");
    if (!f) return false;

    ArchivoHeader h;
    fread(&h, sizeof(ArchivoHeader), 1, f);

    Transaccion t;
    bool encontrado = false;

    // Recorremos todas las transacciones del disco
    while (fread(&t, sizeof(Transaccion), 1, f)) {
        // Como una transacción puede tener varios productos (según tu struct)
        for (int i = 0; i < t.numItems; i++) {
            if (t.idProductos[i] == idBuscado) {
                encontrado = true;
                break;
            }
        }
        if (encontrado) break;
    }

    fclose(f);
    return encontrado;
}

bool buscarProveedorPorIDBin(int idBuscado, Proveedor* resultado) {
    FILE* f = fopen("proveedores.bin", "rb");
    if (!f) return false;

    ArchivoHeader h;
    fread(&h, sizeof(ArchivoHeader), 1, f);

    Proveedor p;
    while (fread(&p, sizeof(Proveedor), 1, f)) {
        if (!p.eliminado && p.id == idBuscado) {
            *resultado = p;
            fclose(f);
            return true;
        }
    }
    fclose(f);
    return false;
}

bool buscarProveedorPorRIFBin(const char* rifBuscado, Proveedor* resultado) {
    FILE* f = fopen("proveedores.bin", "rb");
    if (!f) return false;

    Proveedor p;
    fseek(f, sizeof(ArchivoHeader), SEEK_SET); // Saltamos el header
    
    while (fread(&p, sizeof(Proveedor), 1, f)) {
        if (!p.eliminado && strcmp(p.rif, rifBuscado) == 0) {
            if (resultado) *resultado = p;
            fclose(f);
            return true;
        }
    }
    fclose(f);
    return false;
}

void mostrarProveedor(const Proveedor& p) {
    cout << "\n=== PROVEEDOR ENCONTRADO ===\n";
    cout << "ID: " << p.id << endl;
    cout << "RIF: " << p.rif << endl;
    cout << "Nombre: " << p.nombre << endl;
    cout << "Email: " << p.email << endl;
    cout << "TelÃ©fono: " << p.telefono << endl;
}


bool existeProveedorBin(int idBuscado) {
    Proveedor p;
    return buscarProveedorPorIDBin(idBuscado, &p);
}

//================
// tabla proveedor
//================
void lineaProv(const char* tipo) {
    if (strcmp(tipo, "top") == 0)
        cout << "+--------------------------------------------------------------------------+\n";
    else if (strcmp(tipo, "mid") == 0)
        cout << "Â¦--------------------------------------------------------------------------Â¦\n";
    else if (strcmp(tipo, "sep") == 0)
        cout << "Â¦----+----------------------+------------------+---------------------------Â¦\n";
    else if (strcmp(tipo, "bot") == 0)
        cout << "+--------------------------------------------------------------------------+\n";
}
void encabezadoProveedores() {
    lineaProv("top");
    cout << "Â¦                         LISTADO DE PROVEEDORES                           Â¦\n";
    lineaProv("mid");
    cout << "Â¦ ID Â¦       Nombre         Â¦      RIF        Â¦        TelÃ©fono           Â¦\n";
    lineaProv("sep");
}
void filaProveedor(const Proveedor& p) {
    cout << "Â¦ "
         << setw(2)  << left << p.id << " Â¦ "
         << setw(20) << left << p.nombre << " Â¦ "
         << setw(16) << left << p.rif << " Â¦ "
         << setw(25) << left << p.telefono << " Â¦\n";
}
void pieProveedores() {
    lineaProv("bot");
}

void buscarYListarProveedoresPorNombreBin(const char* filtro) {
    FILE* f = fopen("proveedores.bin", "rb");
    if (!f) return;

    ArchivoHeader h;
    fread(&h, sizeof(ArchivoHeader), 1, f);

    string filtroMin = toLower(filtro);
    Proveedor p;
    bool encontrado = false;

    encabezadoProveedores();
    while (fread(&p, sizeof(Proveedor), 1, f)) {
        if (!p.eliminado && contiene(p.nombre, filtroMin)) {
            filaProveedor(p);
            encontrado = true;
        }
    }
    pieProveedores();

    if (!encontrado) cout << "No se encontraron proveedores con ese nombre.\n";
    fclose(f);
}

bool buscarClientePorIDBin(int idBuscado, Cliente* resultado) {
    FILE* f = fopen("clientes.bin", "rb");
    if (!f) return false;

    fseek(f, sizeof(ArchivoHeader), SEEK_SET);

    Cliente c;
    while (fread(&c, sizeof(Cliente), 1, f)) {
        if (!c.eliminado && c.id == idBuscado) {
            if (resultado) *resultado = c;
            fclose(f);
            return true;
        }
    }
    fclose(f);
    return false;
}

bool existeClienteBin(int idBuscado) {
	Cliente c;
    return buscarClientePorIDBin(idBuscado, &c);
}

bool buscarClientePorCedulaBin(const char* cedulaBuscada, Cliente* resultado) {
    FILE* f = fopen("clientes.bin", "rb");
    if (!f) return false;

    // Saltamos el header
    fseek(f, sizeof(ArchivoHeader), SEEK_SET);

    Cliente c;
    while (fread(&c, sizeof(Cliente), 1, f)) {
        if (!c.eliminado && strcmp(c.cedula, cedulaBuscada) == 0) {
            if (resultado) *resultado = c; // Copiamos los datos al puntero
            fclose(f);
            return true;
        }
    }
    fclose(f);
    return false;
}

void obtenerNombreClienteBin(int idBuscado, char* destino) {
    strcpy(destino, "Desconocido");
    FILE* f = fopen("clientes.bin", "rb");
    if (!f) return;

    ArchivoHeader h;
    fread(&h, sizeof(ArchivoHeader), 1, f);

    Cliente c;
    while (fread(&c, sizeof(Cliente), 1, f)) {
        if (!c.eliminado && c.id == idBuscado) {
            strcpy(destino, c.nombre);
            break;
        }
    }
    fclose(f);
}

void mostrarCliente(const Cliente& c) {
    cout << "\n=== CLIENTE ENCONTRADO ===\n";
    cout << "ID: " << c.id << endl;
    cout << "CÃ©dula/RIF: " << c.cedula << endl;
    cout << "Nombre: " << c.nombre << endl;
    cout << "Email: " << c.email << endl;
    cout << "TelÃ©fono: " << c.telefono << endl;
    cout << "DirecciÃ³n: " << c.direccion << endl;
}

//==============
//tabla clientes
//==============

void lineaClientes(const char* tipo) {
    if (strcmp(tipo, "top") == 0)
        cout << "+-------------------------------------------------------------------------------------------+\n";
    else if (strcmp(tipo, "mid") == 0)
        cout << "Â¦-------------------------------------------------------------------------------------------Â¦\n";
    else if (strcmp(tipo, "sep") == 0)
        cout << "Â¦----+----------------------+------------------+----------------------+----------------------Â¦\n";
    else if (strcmp(tipo, "bot") == 0)
        cout << "+-------------------------------------------------------------------------------------------+\n";
}

void encabezadoClientes() {
    lineaClientes("top");
    cout << "Â¦                                 LISTADO DE CLIENTES                                       Â¦\n";
    lineaClientes("mid");
    cout << "Â¦ ID Â¦       Nombre         Â¦   CÃ©dula/RIF    Â¦       TelÃ©fono       Â¦        Email         Â¦\n";
    lineaClientes("sep");
}

void filaCliente(const Cliente& c) {
    cout << "Â¦ "
         << setw(2)  << left << c.id << " Â¦ "
         << setw(20) << left << c.nombre << " Â¦ "
         << setw(16) << left << c.cedula << " Â¦ "
         << setw(20) << left << c.telefono << " Â¦ "
         << setw(20) << left << c.email << " Â¦\n";
}

void pieClientes() {
    lineaClientes("bot");
}

void buscarYListarClientesPorNombreBin(const char* filtro) {
    FILE* f = fopen("clientes.bin", "rb");
    if (!f) return;

    ArchivoHeader h;
    fread(&h, sizeof(ArchivoHeader), 1, f);

    string filtroMin = toLower(filtro);
    Cliente c;
    bool encontrado = false;

    encabezadoClientes();
    while (fread(&c, sizeof(Cliente), 1, f)) {
        if (!c.eliminado && contiene(c.nombre, filtroMin)) {
            filaCliente(c);
            encontrado = true;
        }
    }
    pieClientes();

    if (!encontrado) cout << "No se encontraron clientes con ese nombre.\n";
    fclose(f);
}

bool clienteTieneVentasBin(int idCliente) {
    FILE* f = fopen("transacciones.bin", "rb");
    if (!f) return false; // Si no hay archivo de ventas, no hay transacciones

    Transaccion t;
    fseek(f, sizeof(ArchivoHeader), SEEK_SET);

    while (fread(&t, sizeof(Transaccion), 1, f)) {
        // Asumiendo que idRelacionado guarda el ID del cliente en tipos "VENTA"
        if (t.idRelacionado == idCliente && strcmp(t.tipo, "VENTA") == 0) {
            fclose(f);
            return true;
        }
    }

    fclose(f);
    return false;
}
//=======================
// tabla transacciones
//=======================

void lineaTrans(const char* tipo) {
    if (strcmp(tipo, "top") == 0)
        cout << "+-----------------------------------------------------------------------------------------------------------+\n";
    else if (strcmp(tipo, "mid") == 0)
        cout << "Â¦-----------------------------------------------------------------------------------------------------------Â¦\n";
    else if (strcmp(tipo, "sep") == 0)
        cout << "Â¦----+----------+----------+----------------------+----------+-----------+------------+---------------------Â¦\n";
    else if (strcmp(tipo, "bot") == 0)
        cout << "+-----------------------------------------------------------------------------------------------------------+\n";
}

void encabezadoTransacciones() {
    lineaTrans("top");
    cout << "Â¦                                      LISTADO DE TRANSACCIONES                                             Â¦\n";
    lineaTrans("mid");
    cout << "Â¦ ID Â¦  Tipo    Â¦ Producto Â¦ Relacionado (Prov/Cliente) Â¦ Cantidad Â¦  Total    Â¦   Fecha    Â¦ DescripciÃ³n     Â¦\n";
    lineaTrans("sep");
}

void filaTransaccion(const Transaccion& t) {
    cout << "Â¦ "
         << setw(2)  << left << t.id << " Â¦ "
         << setw(8)  << left << t.tipo << " Â¦ "
         << setw(8)  << left << (t.numItems > 0 ? aString(t.idProductos[0]) : "N/A") << " ¦ "
         << setw(22) << left << t.idRelacionado << " Â¦ "
         << setw(8)  << left << t.cantidades << " Â¦ "
         << setw(9)  << left << t.total << " Â¦ "
         << setw(10) << left << t.fechaRegistro << " Â¦ "
         << setw(15) << left << t.descripcion << " Â¦\n";
}

void pieTransacciones() {
    lineaTrans("bot");
}

//===============
//2.1 inicializar
//===============

void inicializarTienda(Tienda* tienda, const char* nombre, const char* rif) {
    strcpy(tienda->nombre, nombre);
    strcpy(tienda->rif, rif);
    
    tienda->totalVentasHistoricas = 0;
    tienda->totalComprasHistoricas = 0;
    tienda->cantidadOperaciones = 0;

    inicializarArchivo<Producto>("productos.bin");
    inicializarArchivo<Proveedor>("proveedores.bin");
    inicializarArchivo<Cliente>("clientes.bin");
    inicializarArchivo<Transaccion>("transacciones.bin");
}

void registrarProductoEnDisco(Producto nuevo) {
    const char* nombreArchivo = "productos.bin";
    ArchivoHeader header = leerHeader(nombreArchivo);

    nuevo.id = header.proximoID;
    nuevo.eliminado = false; 

    FILE* f = fopen(nombreArchivo, "ab");
    if (f) {
        fwrite(&nuevo, sizeof(Producto), 1, f);
        fclose(f);

        header.cantidadRegistros++;
        header.proximoID++;
        header.registrosActivos++;
        actualizarHeader(nombreArchivo, header);
    }
}

void registrarTransaccionEnDisco(Transaccion nueva) {
    const char* nombreArchivo = "transacciones.bin";
    ArchivoHeader header = leerHeader(nombreArchivo);

    nueva.id = header.proximoID;
    nueva.eliminado = false;
    obtenerFechaActual(nueva.fechaRegistro);

    FILE* f = fopen(nombreArchivo, "ab");
    if (f) {
        fwrite(&nueva, sizeof(Transaccion), 1, f);
        fclose(f);

        header.cantidadRegistros++;
        header.proximoID++;
        header.registrosActivos++;
        actualizarHeader(nombreArchivo, header);
    }
}

void mostrarDetalleTransaccion(const Transaccion& t) {
    cout << "+-----------------------------------------------------------+\n";
    cout << "|            DETALLE DE TRANSACCION (DISCO)                 |\n";
    cout << "+-----------------------------------------------------------+\n";
    cout << "| ID: " << setw(54) << left << t.id << "|\n";
    cout << "| Tipo: " << setw(52) << left << t.tipo << "|\n";
    cout << "| Items: " << setw(51) << left << t.numItems << "|\n";
    
    for(int i = 0; i < t.numItems; i++) {
        char nombreP[100];
        obtenerNombreProductoBin(t.idProductos[i], nombreP);
        cout << "|  - " << setw(20) << left << nombreP 
             << " Cant: " << setw(25) << left << t.cantidades[i] << "|\n";
    }

    cout << "| Total:    $" << setw(47) << left << fixed << setprecision(2) << t.total << "|\n";
    cout << "| Fecha:    " << setw(48) << left << t.fechaRegistro << "|\n";
    cout << "+-----------------------------------------------------------+\n";
}


void crearProducto() { // Ya no necesita recibir Tienda* si usas archivos
    if (!confirmar("¿Desea registrar un nuevo producto? (S/N): "))
        return;

    Producto nuevo;

    // --- Código ---
    solicitarString("Ingrese código del producto (o CANCELAR): ", nuevo.codigo, 20, sinEspacios);
    
    if (strcmp(nuevo.codigo, "CANCELAR") == 0 || strcmp(nuevo.codigo, "0") == 0)
        return;

    // IMPORTANTE: Ahora la validación busca en el archivo .bin
    if (codigoProductoDuplicado(nuevo.codigo)) { 
        cout << "ERROR: El código '" << nuevo.codigo << "' ya está registrado.\n";
        return;
    }

    // --- Nombre y Descripción ---
    solicitarString("Ingrese nombre del producto: ", nuevo.nombre, 100);
    solicitarString("Ingrese descripción: ", nuevo.descripcion, 200);

    // --- ID Proveedor ---
    cout << "Ingrese ID del proveedor (o 0 para cancelar): ";
    cin >> nuevo.idProveedor;
    cin.ignore(); // Limpiar el buffer tras usar cin >>

    if (nuevo.idProveedor == 0) return;

    // Validación contra el archivo de proveedores
    if (!existeProveedorBin(nuevo.idProveedor)) {
        cout << "ERROR: El proveedor con ID " << nuevo.idProveedor << " no existe.\n";
        return;
    }

    // --- Precio y Stock ---
    cout << "Ingrese precio: "; cin >> nuevo.precio;
    cout << "Ingrese stock: ";  cin >> nuevo.stock;
    cin.ignore();

    // --- Fecha y Borrado Lógico ---
    obtenerFechaActual(nuevo.fechaRegistro);
    nuevo.eliminado = false; 

    // --- Resumen ---
    cout << "\n=== RESUMEN DEL PRODUCTO ===\n";
    mostrarProducto(nuevo);

    if (confirmar("¿Guardar producto en disco? (S/N): ")) {
        registrarProductoEnDisco(nuevo); 
        cout << "\nProducto guardado físicamente en productos.bin\n";
    } else {
        cout << "\nRegistro cancelado.\n";
    }
}

//========================
//2.2.2
//========================

void buscarProducto() { // Ya no necesita recibir Tienda*
    int opcion;
    cout << "\n=== BUSCAR PRODUCTO EN DISCO ===\n";
    cout << "1. Buscar por ID\n";
    cout << "2. Buscar por nombre (parcial)\n";
    cout << "3. Buscar por código (parcial)\n";
    cout << "0. Cancelar\n";
    cout << "Seleccione una opción: ";
    cin >> opcion;
    cin.ignore(); // Limpiar el buffer

    if (opcion == 0) return;

    FILE* f = fopen("productos.bin", "rb");
    if (!f) {
        cout << "ERROR: No se pudo abrir el archivo de productos.\n";
        return;
    }

    // Leemos la cabecera para saber cuántos registros hay
    ArchivoHeader h;
    fread(&h, sizeof(ArchivoHeader), 1, f);

    Producto p;
    bool encontrado = false;

    switch (opcion) {
        // 1. Buscar por ID
        case 1: {
            int idBuscado;
            cout << "Ingrese ID del producto: ";
            cin >> idBuscado;

            while (fread(&p, sizeof(Producto), 1, f)) {
                if (!p.eliminado && p.id == idBuscado) {
                    cout << "\n=== PRODUCTO ENCONTRADO ===\n";
                    mostrarProducto(p);
                    encontrado = true;
                    break; 
                }
            }
            break;
        }

        // 2. Buscar por nombre (parcial)
        case 2: {
            char buffer[100];
            solicitarString("Ingrese parte del nombre: ", buffer, 100);
            string filtro = toLower(buffer);

            cout << "\n=== RESULTADOS POR NOMBRE ===\n";
            while (fread(&p, sizeof(Producto), 1, f)) {
                if (!p.eliminado && contiene(p.nombre, filtro)) {
                    mostrarProducto(p);
                    cout << "-----------------------------\n";
                    encontrado = true;
                }
            }
            break;
        }

        // 3. Buscar por código (parcial)
        case 3: {
            char buffer[20];
            solicitarString("Ingrese parte del código: ", buffer, 20);
            string filtro = toLower(buffer);

            cout << "\n=== RESULTADOS POR CÓDIGO ===\n";
            while (fread(&p, sizeof(Producto), 1, f)) {
                if (!p.eliminado && contiene(p.codigo, filtro)) {
                    mostrarProducto(p);
                    cout << "-----------------------------\n";
                    encontrado = true;
                }
            }
            break;
        }

        default:
            cout << "Opción inválida.\n";
            break;
    }

    if (!encontrado && opcion >= 1 && opcion <= 3)
        cout << "No se encontraron coincidencias.\n";

    fclose(f);
}


//========================
//2.2.3
//========================

void actualizarProducto() {
    int idBuscado;
    cout << "Ingrese el ID del producto a actualizar: ";
    cin >> idBuscado;
    cin.ignore();

    FILE* f = fopen("productos.bin", "r+b"); // r+b permite lectura Y escritura
    if (!f) {
        cout << "ERROR: No se pudo abrir el archivo.\n";
        return;
    }

    // Leemos el header
    ArchivoHeader h;
    fread(&h, sizeof(ArchivoHeader), 1, f);

    Producto temp;
    long posicionRegistro = -1;
    bool encontrado = false;

    // Buscamos el producto y guardamos su posición
    for (int i = 0; i < h.cantidadRegistros; i++) {
        posicionRegistro = ftell(f); // Guardamos la posición actual antes de leer
        if (fread(&temp, sizeof(Producto), 1, f)) {
            if (!temp.eliminado && temp.id == idBuscado) {
                encontrado = true;
                break;
            }
        }
    }

    if (!encontrado) {
        cout << "ERROR: No existe un producto activo con ese ID.\n";
        fclose(f);
        return;
    }

    int opcion;
    do {
        cout << "\n=== EDITAR PRODUCTO (EN DISCO) ===\n";
        cout << "1. Código: " << temp.codigo << endl;
        cout << "2. Nombre: " << temp.nombre << endl;
        cout << "3. Descripción: " << temp.descripcion << endl;
        cout << "4. Precio: " << temp.precio << endl;
        cout << "5. Stock: " << temp.stock << endl;
        cout << "6. Guardar cambios en archivo\n";
        cout << "0. Salir sin guardar\n";
        cout << "Opción: ";
        cin >> opcion;
        cin.ignore();

        switch (opcion) {
            case 1: {
                char nuevoCodigo[20];
                solicitarString("Nuevo código: ", nuevoCodigo, 20, sinEspacios);
                if (codigoProductoDuplicado(nuevoCodigo, temp.id)) {
                    cout << "ERROR: Código ya existe.\n";
                } else {
                    strcpy(temp.codigo, nuevoCodigo);
                }
                break;
            }
            case 2: solicitarString("Nuevo nombre: ", temp.nombre, 100); break;
            case 3: solicitarString("Nueva descripción: ", temp.descripcion, 200); break;
            case 4: cout << "Nuevo precio: "; cin >> temp.precio; cin.ignore(); break;
            case 5: cout << "Nuevo stock: "; cin >> temp.stock; cin.ignore(); break;
            case 6: {
                if (confirmar("¿Sobrescribir datos en disco? (S/N): ")) {
                    // LA MAGIA: Volvemos a la posición donde estaba el producto
                    fseek(f, posicionRegistro, SEEK_SET); 
                    fwrite(&temp, sizeof(Producto), 1, f);
                    cout << "Archivo actualizado exitosamente.\n";
                }
                fclose(f);
                return;
            }
            case 0: fclose(f); return;
        }
    } while (true);
}

//========================
//2.2.4
//========================

bool actualizarStockProducto(int idProd, int cambioCantidad) {
    FILE* f = fopen("productos.bin", "r+b");
    if (!f) return false;

    ArchivoHeader h;
    fread(&h, sizeof(ArchivoHeader), 1, f);

    Producto p;
    long pos;
    bool exito = false;

    while (fread(&p, sizeof(Producto), 1, f)) {
        pos = ftell(f) - sizeof(Producto); // Ubicación exacta del registro
        if (p.id == idProd && !p.eliminado) {
            p.stock += cambioCantidad; // Modificamos el stock en la variable
            fseek(f, pos, SEEK_SET);   // Volvemos a la posición
            fwrite(&p, sizeof(Producto), 1, f); // Sobreescribimos
            exito = true;
            break;
        }
    }
    fclose(f);
    return exito;
}

//========================
//2.2.5
//========================

void listarProductos() {
    FILE* f = fopen("productos.bin", "rb");
    if (!f) {
        cout << "No hay productos registrados o el archivo no existe.\n";
        return;
    }

    // Leemos el Header para las estadísticas del pie de tabla
    ArchivoHeader h;
    fread(&h, sizeof(ArchivoHeader), 1, f);

    if (h.cantidadRegistros == 0) {
        cout << "El archivo está vacío.\n";
        fclose(f);
        return;
    }

    encabezadoProductos();

    Producto p;
    int mostrados = 0;

    // Recorremos el archivo registro por registro
    while (fread(&p, sizeof(Producto), 1, f)) {
        if (!p.eliminado) {
            char nombreProv[100];
            obtenerNombreProveedorBin(p.idProveedor, nombreProv); 
            
            filaProducto(p, nombreProv);
            mostrados++;
        }
    }

    pieProductos();

    // Estadísticas basadas en el Header y el conteo real
    cout << "\nTotal registros en archivo: " << h.cantidadRegistros;
    cout << "\nProductos activos listados: " << mostrados << "\n";

    fclose(f);
}


//========================
//2.2.6
//========================

void eliminarProducto() {
    int idBuscado;
    cout << "Ingrese el ID del producto a eliminar: ";
    cin >> idBuscado;
    cin.ignore();

    // 1. Buscamos el producto y obtenemos sus datos (y su posición)
    Producto p;
    // IMPORTANTE: Necesitamos saber en qué byte está el producto para sobreescribirlo
    long posicionRegistro = -1; 
    
    FILE* f = fopen("productos.bin", "r+b");
    if (!f) {
        cout << "ERROR: No se pudo abrir el archivo de productos.\n";
        return;
    }

    ArchivoHeader h;
    fread(&h, sizeof(ArchivoHeader), 1, f);

    bool encontrado = false;
    // Buscamos el registro y guardamos su posición exacta en bytes
    for (int i = 0; i < h.cantidadRegistros; i++) {
        posicionRegistro = ftell(f); // Guardamos DONDE empieza este registro
        if (fread(&p, sizeof(Producto), 1, f)) {
            if (!p.eliminado && p.id == idBuscado) {
                encontrado = true;
                break;
            }
        }
    }

    if (!encontrado) {
        cout << "ERROR: No existe un producto activo con ID " << idBuscado << ".\n";
        fclose(f);
        return;
    }

    // 2. Mostrar datos antes de borrar
    cout << "\n=== PRODUCTO A ELIMINAR ===\n";
    mostrarProducto(p);

    // 3. Verificación de integridad (Opcional pero recomendado)
    if (productoTieneTransaccionesBin(idBuscado)) {
        cout << "\nADVERTENCIA: Este producto tiene movimientos en el inventario.\n";
    }

    if (!confirmar("\n¿Esta seguro de marcar este producto como ELIMINADO? (S/N): ")) {
        cout << "Operacion cancelada.\n";
        fclose(f);
        return;
    }

    // 4. EL BORRADO LÓGICO
    p.eliminado = true;

    // 5. SOBREESCRITURA: Saltamos a la posición que guardamos antes
    fseek(f, posicionRegistro, SEEK_SET);
    fwrite(&p, sizeof(Producto), 1, f);
    fclose(f);

    // 6. ACTUALIZAR HEADER
    h = leerHeader("productos.bin");
    h.registrosActivos--;
    actualizarHeader("productos.bin", h);

    cout << "Producto eliminado (logicamente) exitosamente.\n";
}

void menuProductos() { 
    int opcion;

    do {
        system("cls");
        cout << "+-------------------------------------------+\n";
        cout << "|           GESTION DE PRODUCTOS            |\n"; // Corregido tildes para consola
        cout << "+-------------------------------------------+\n\n";

        cout << "1. Registrar nuevo producto\n";
        cout << "2. Buscar producto\n";
        cout << "3. Actualizar producto\n";
        cout << "4. Actualizar stock manualmente\n";
        cout << "5. Listar todos los productos\n";
        cout << "6. Eliminar producto\n";
        cout << "0. Volver al menu principal\n\n";
        cout << "Seleccione una opcion: ";

        cin >> opcion;
        cin.ignore();

        system("cls");

        switch (opcion) {
            case 1: crearProducto(); break;           // Llama directo al disco
            case 2: buscarProducto(); break;          // Llama directo al disco
            case 3: actualizarProducto(); break;      // Llama directo al disco
            case 4: {
                int id, cant;
                cout << "ID Producto: "; cin >> id;
                cout << "Cantidad a sumar/restar: "; cin >> cant;
                actualizarStockProducto(id, cant); 
                break;
            }
            case 5: listarProductos(); break;         // Llama directo al disco
            case 6: eliminarProducto(); break;        // Llama directo al disco
            case 0: break;
            default: cout << "Opcion invalida.\n";
        }

        if (opcion != 0) {
            cout << "\n";
            system("pause");
        }

    } while (opcion != 0);
}

//2.3

void registrarProveedorEnDisco(Proveedor nuevo) {
    const char* nombreArchivo = "proveedores.bin";
    ArchivoHeader header = leerHeader(nombreArchivo);

    nuevo.id = header.proximoID; // Asignación de ID automática desde el disco

    FILE* f = fopen(nombreArchivo, "ab");
    if (f) {
        fwrite(&nuevo, sizeof(Proveedor), 1, f);
        fclose(f);

        // Actualizamos las cuentas en el Header
        header.cantidadRegistros++;
        header.proximoID++;
        header.registrosActivos++;
        actualizarHeader(nombreArchivo, header);
    }
}
void crearProveedor() {
    if (!confirmar("¿Desea registrar un nuevo proveedor? (S/N): "))
        return;

    Proveedor nuevo;

    // --- RIF ---
    solicitarString("Ingrese RIF del proveedor (o CANCELAR): ", nuevo.rif, 20);

    if (strcmp(nuevo.rif, "CANCELAR") == 0 || strcmp(nuevo.rif, "0") == 0)
        return;

    // IMPORTANTE: Ahora la validación busca en proveedores.bin
    if (rifDuplicado(nuevo.rif)) {
        cout << "ERROR: El RIF '" << nuevo.rif << "' ya esta registrado.\n";
        return;
    }

    // --- Nombre, Email y Telefono ---
    solicitarString("Ingrese nombre: ", nuevo.nombre, 100);
    solicitarString("Ingrese email: ", nuevo.email, 100, emailValido);
    solicitarString("Ingrese telefono: ", nuevo.telefono, 20);

    // --- Campos de control ---
    nuevo.eliminado = false; 

    // --- Resumen ---
    cout << "\n=== RESUMEN DEL PROVEEDOR ===\n";
    cout << "RIF: " << nuevo.rif << endl;
    cout << "Nombre: " << nuevo.nombre << endl;
    cout << "Email: " << nuevo.email << endl;
    cout << "Telefono: " << nuevo.telefono << endl;

    if (confirmar("¿Guardar proveedor en disco? (S/N): ")) {
        // Llamamos a la utilidad genérica de guardado
        registrarProveedorEnDisco(nuevo); 
        cout << "Proveedor registrado exitosamente en proveedores.bin\n";
    } else {
        cout << "Operacion cancelada.\n";
    }
}


//2.3.2

void buscarProveedor() {
    int opcion;
    do {
        cout << "\n=== BUSCAR PROVEEDOR EN DISCO ===\n";
        cout << "1. Buscar por ID\n";
        cout << "2. Buscar por nombre (coincidencia parcial)\n";
        cout << "3. Buscar por RIF\n";
        cout << "0. Cancelar\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        cin.ignore();

        if (opcion == 0) return;

        Proveedor p;
        bool encontrado = false;

        switch (opcion) {
            case 1: {
                int id;
                cout << "Ingrese ID del proveedor: ";
                cin >> id;
                encontrado = buscarProveedorPorIDBin(id, &p); // Pasamos p por referencia
                break;
            }
            case 2: {
                char nombre[100];
                solicitarString("Ingrese nombre o parte del nombre: ", nombre, 100);
                // Para nombres parciales, solemos listar las coincidencias
                buscarYListarProveedoresPorNombreBin(nombre);
                encontrado = true; // El listado maneja su propio mensaje si no hay nada
                break;
            }
            case 3: {
                char rif[20];
                solicitarString("Ingrese RIF del proveedor: ", rif, 20);
                encontrado = buscarProveedorPorRIFBin(rif, &p);
                break;
            }
            default:
                cout << "Opcion invalida.\n";
                continue;
        }

        // Si fue una búsqueda exacta (ID o RIF) y se encontró, mostramos el detalle
        if ((opcion == 1 || opcion == 3)) {
            if (encontrado) {
                cout << "\n=== PROVEEDOR ENCONTRADO ===\n";
                mostrarProveedor(p);
            } else {
                cout << "No se encontro ningun proveedor con ese criterio.\n";
            }
        }

        if (opcion != 0) system("pause");

    } while (opcion != 0);
}

//2.3.3

void actualizarProveedor() {
    int idBuscado;
    cout << "Ingrese el ID del proveedor a actualizar (0 para cancelar): ";
    cin >> idBuscado;
    cin.ignore();

    if (idBuscado == 0) return;

    // Abrimos en modo r+b (lectura y escritura sin borrar el archivo)
    FILE* f = fopen("proveedores.bin", "r+b");
    if (!f) {
        cout << "ERROR: No se pudo abrir el archivo de proveedores.\n";
        return;
    }

    ArchivoHeader h;
    fread(&h, sizeof(ArchivoHeader), 1, f);

    Proveedor temp;
    long posicionRegistro = -1;
    bool encontrado = false;

    // 1. Buscamos el proveedor y guardamos su posición en bytes
    for (int i = 0; i < h.cantidadRegistros; i++) {
        posicionRegistro = ftell(f); // Guardamos donde empieza este registro
        if (fread(&temp, sizeof(Proveedor), 1, f)) {
            if (!temp.eliminado && temp.id == idBuscado) {
                encontrado = true;
                break;
            }
        }
    }

    if (!encontrado) {
        cout << "ERROR: No existe un proveedor activo con ID " << idBuscado << ".\n";
        fclose(f);
        return;
    }

    // Guardamos una copia de respaldo para comparar cambios
    Proveedor original = temp;

    cout << "\n=== PROVEEDOR ENCONTRADO ===\n";
    mostrarProveedor(original);

    if (!confirmar("\n¿Desea modificar este proveedor? (S/N): ")) {
        fclose(f);
        return;
    }

    // --- Edición de campos (Lógica de "Enter para mantener") ---
    char buffer[200];

    cout << "\nNombre actual: " << original.nombre << endl;
    solicitarString("Nuevo nombre (ENTER para mantener): ", buffer, 100);
    if (strlen(buffer) > 0) strcpy(temp.nombre, buffer);

    cout << "\nRIF actual: " << original.rif << endl;
    solicitarString("Nuevo RIF (ENTER para mantener): ", buffer, 20);
    if (strlen(buffer) > 0) {
        if (rifDuplicado(buffer, original.id)) {
            cout << "ERROR: El RIF ya existe en otro proveedor.\n";
            fclose(f); return;
        }
        strcpy(temp.rif, buffer);
    }

    cout << "\nTelefono actual: " << original.telefono << endl;
    solicitarString("Nuevo telefono (ENTER para mantener): ", buffer, 20);
    if (strlen(buffer) > 0) strcpy(temp.telefono, buffer);

    cout << "\nEmail actual: " << original.email << endl;
    solicitarString("Nuevo email (ENTER para mantener): ", buffer, 100);
    if (strlen(buffer) > 0) {
        if (!emailValido(buffer)) {
            cout << "ERROR: Email invalido.\n";
            fclose(f); return;
        }
        strcpy(temp.email, buffer);
    }

    // --- Guardado Final ---
    cout << "\n=== NUEVOS DATOS DEL PROVEEDOR ===\n";
    mostrarProveedor(temp);

    if (confirmar("¿Guardar cambios en disco? (S/N): ")) {
        // 2. Movemos el cabezal de escritura a la posición guardada
        fseek(f, posicionRegistro, SEEK_SET); 
        // 3. Sobreescribimos solo ese bloque de datos
        fwrite(&temp, sizeof(Proveedor), 1, f);
        cout << "Proveedor actualizado exitosamente en el archivo.\n";
    }

    fclose(f);
}

//2.3.4

void listarProveedores() {
    FILE* f = fopen("proveedores.bin", "rb");
    if (!f) {
        cout << "No hay proveedores registrados o el archivo no existe.\n";
        return;
    }

    // Leemos el Header para saber la salud del archivo
    ArchivoHeader h;
    fread(&h, sizeof(ArchivoHeader), 1, f);

    if (h.cantidadRegistros == 0) {
        cout << "El archivo de proveedores esta vacio.\n";
        fclose(f);
        return;
    }

    encabezadoProveedores(); // Tu función de formato de tabla

    Proveedor p;
    int mostrados = 0;

    // Recorrido Secuencial del archivo
    while (fread(&p, sizeof(Proveedor), 1, f)) {
        // REGLA DE ORO: Solo listar los que no están eliminados
        if (!p.eliminado) {
            filaProveedor(p); // Tu función que imprime la línea de la tabla
            mostrados++;
        }
    }

    pieProveedores(); // Cierre de la tabla

    // Información útil para el usuario
    cout << "\nTotal registros (incluyendo eliminados): " << h.cantidadRegistros;
    cout << "\nProveedores activos actualmente: " << mostrados << "\n";

    fclose(f);
}

//2.3.5

bool proveedorTieneProductosBin(int idProv) {
    FILE* f = fopen("productos.bin", "rb");
    if (!f) return false;

    Producto p;
    fseek(f, sizeof(ArchivoHeader), SEEK_SET);

    while (fread(&p, sizeof(Producto), 1, f)) {
        // Si el producto no esta borrado y pertenece al proveedor
        if (!p.eliminado && p.idProveedor == idProv) {
            fclose(f);
            return true; 
        }
    }

    fclose(f);
    return false;
}

void eliminarProveedor() { 
    int idBuscado;
    cout << "Ingrese el ID del proveedor a eliminar (0 para cancelar): ";
    cin >> idBuscado;
    cin.ignore();

    if (idBuscado == 0) return;

    // 1. Buscamos la posicion del proveedor en el archivo
    // Usamos una funcion que nos devuelva la posicion exacta en bytes
    long posicionReg = -1;
    Proveedor p;
    
    FILE* f = fopen("proveedores.bin", "r+b");
    if (!f) return;

    ArchivoHeader h;
    fread(&h, sizeof(ArchivoHeader), 1, f);

    bool encontrado = false;
    for (int i = 0; i < h.cantidadRegistros; i++) {
        posicionReg = ftell(f);
        if (fread(&p, sizeof(Proveedor), 1, f)) {
            if (!p.eliminado && p.id == idBuscado) {
                encontrado = true;
                break;
            }
        }
    }

    if (!encontrado) {
        cout << "ERROR: No existe un proveedor activo con ese ID.\n";
        fclose(f);
        return;
    }

    // 2. VERIFICACIÓN CRÍTICA: Buscar en el archivo de productos
    if (proveedorTieneProductosBin(idBuscado)) {
        cout << "\nERROR: No se puede eliminar el proveedor '" << p.nombre << "'.\n";
        cout << "Tiene productos vinculados en productos.bin.\n";
        fclose(f);
        return;
    }

    cout << "\n=== PROVEEDOR A ELIMINAR ===\n";
    mostrarProveedor(p);

    if (!confirmar("\n¿Confirmar borrado logico? (S/N): ")) {
        cout << "Operacion cancelada.\n";
        fclose(f);
        return;
    }

    // 3. EJECUTAR BORRADO LÓGICO
    p.eliminado = true;
    fseek(f, posicionReg, SEEK_SET);
    fwrite(&p, sizeof(Proveedor), 1, f);
    fclose(f);

    // 4. ACTUALIZAR EL HEADER
    h = leerHeader("proveedores.bin");
    h.registrosActivos--;
    actualizarHeader("proveedores.bin", h);

    cout << "Proveedor eliminado exitosamente del sistema.\n";
}

void menuProveedores() {
    int opcion;

    do {
        system("cls");
        cout << "+-------------------------------------------+\n";
        cout << "|           GESTION DE PROVEEDORES          |\n"; 
        cout << "+-------------------------------------------+\n\n";

        cout << "1. Registrar proveedor\n";
        cout << "2. Buscar proveedor\n";
        cout << "3. Actualizar proveedor\n";
        cout << "4. Listar proveedores\n";
        cout << "5. Eliminar proveedor\n";
        cout << "0. Volver al menu principal\n\n";
        cout << "Seleccione una opcion: ";

        cin >> opcion;
        cin.ignore();

        system("cls");

        switch (opcion) {
            case 1: crearProveedor(); break;      // Funciona directo con disco
            case 2: buscarProveedor(); break;     // Funciona directo con disco
            case 3: actualizarProveedor(); break; // Funciona directo con disco
            case 4: listarProveedores(); break;    // Funciona directo con disco
            case 5: eliminarProveedor(); break;   // Funciona directo con disco
            case 0: break;
            default: cout << "Opcion invalida.\n";
        }

        if (opcion != 0) {
            cout << "\n";
            system("pause");
        }

    } while (opcion != 0);
}

//=======================
//2.4
//=======================

void registrarClienteEnDisco(Cliente nuevo) {
    const char* ruta = "clientes.bin";
    ArchivoHeader header = leerHeader(ruta);

    // Asignamos el ID correlativo que lleva el Header
    nuevo.id = header.proximoID; 

    FILE* f = fopen(ruta, "ab"); // Append Binary
    if (f) {
        fwrite(&nuevo, sizeof(Cliente), 1, f);
        fclose(f);

        // Actualizamos los contadores del Header
        header.cantidadRegistros++;
        header.proximoID++;
        header.registrosActivos++;
        actualizarHeader(ruta, header);
    }
}

void crearCliente() {
    if (!confirmar("¿Desea registrar un nuevo cliente? (S/N): "))
        return;

    Cliente nuevo;

    // --- Cedula / RIF ---
    solicitarString("Ingrese cedula/RIF del cliente (o CANCELAR): ", nuevo.cedula, 20);

    if (strcmp(nuevo.cedula, "CANCELAR") == 0 || strcmp(nuevo.cedula, "0") == 0)
        return;

    // Validación contra el archivo clientes.bin
    if (clienteDuplicado(nuevo.cedula)) {
        cout << "ERROR: La cedula/RIF '" << nuevo.cedula << "' ya esta registrada.\n";
        return;
    }

    // --- Nombre, Email, Telefono y Direccion ---
    solicitarString("Ingrese nombre del cliente: ", nuevo.nombre, 100);
    solicitarString("Ingrese email: ", nuevo.email, 100, emailValido);
    solicitarString("Ingrese telefono: ", nuevo.telefono, 20);
    solicitarString("Ingrese direccion: ", nuevo.direccion, 200);

    // --- Metadatos y Borrado Lógico ---
    obtenerFechaActual(nuevo.fechaRegistro);
    nuevo.eliminado = false; 

    // --- Resumen ---
    cout << "\n=== RESUMEN DEL CLIENTE ===\n";
    cout << "ID: (Se asignara al guardar)\n";
    cout << "Cedula/RIF: " << nuevo.cedula << endl;
    cout << "Nombre: " << nuevo.nombre << endl;
    cout << "Email: " << nuevo.email << endl;
    cout << "Telefono: " << nuevo.telefono << endl;
    cout << "Direccion: " << nuevo.direccion << endl;

    if (confirmar("¿Guardar cliente en disco? (S/N): ")) {
        // Esta función maneja el ID y el Header automáticamente
        registrarClienteEnDisco(nuevo); 
        cout << "\nCliente registrado exitosamente en clientes.bin.\n";
    } else {
        cout << "\nOperacion cancelada.\n";
    }
}

//2.4.2

void buscarCliente() { // No recibe Tienda*
    int opcion;
    do {
        cout << "\n=== BUSCAR CLIENTE EN DISCO ===\n";
        cout << "1. Buscar por ID\n";
        cout << "2. Buscar por nombre (coincidencia parcial)\n";
        cout << "3. Buscar por cedula/RIF\n";
        cout << "0. Cancelar\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        cin.ignore();

        if (opcion == 0) return;

        Cliente c;
        bool encontrado = false;

        switch (opcion) {
            case 1: {
                int id;
                cout << "Ingrese ID del cliente: ";
                cin >> id;
                encontrado = buscarClientePorIDBin(id, &c); 
                break;
            }
            case 2: {
                char nombre[100];
                solicitarString("Ingrese nombre o parte del nombre: ", nombre, 100);
                // Las búsquedas parciales suelen listar varios resultados
                buscarYListarClientesPorNombreBin(nombre);
                encontrado = true; // El listado maneja su propio mensaje
                break;
            }
            case 3: {
                char cedula[20];
                solicitarString("Ingrese cedula/RIF del cliente: ", cedula, 20);
                encontrado = buscarClientePorCedulaBin(cedula, &c);
                break;
            }
            default:
                cout << "Opcion invalida.\n";
                continue;
        }

        // Para búsquedas exactas (ID o Cédula), mostramos el detalle si se encontró
        if ((opcion == 1 || opcion == 3) && encontrado) {
            cout << "\n=== CLIENTE ENCONTRADO ===\n";
            mostrarCliente(c);
        } else if ((opcion == 1 || opcion == 3) && !encontrado) {
            cout << "No se encontro ningun cliente con ese criterio.\n";
        }

        if (opcion != 0) system("pause");

    } while (opcion != 0);
}

//2.4.3

void actualizarCliente() {
    int idBuscado;
    cout << "Ingrese el ID del cliente a actualizar (0 para cancelar): ";
    cin >> idBuscado;
    cin.ignore();

    if (idBuscado == 0) return;

    FILE* f = fopen("clientes.bin", "r+b"); // r+b para leer y escribir
    if (!f) {
        cout << "ERROR: El archivo de clientes no existe.\n";
        return;
    }

    // Leemos el Header para saber el límite de búsqueda
    ArchivoHeader h;
    fread(&h, sizeof(ArchivoHeader), 1, f);

    Cliente temp;
    long posicionRegistro = -1;
    bool encontrado = false;

    // 1. Buscamos el registro y marcamos su posición
    for (int i = 0; i < h.cantidadRegistros; i++) {
        posicionRegistro = ftell(f); // Guardamos la ubicación actual en bytes
        if (fread(&temp, sizeof(Cliente), 1, f)) {
            if (!temp.eliminado && temp.id == idBuscado) {
                encontrado = true;
                break;
            }
        }
    }

    if (!encontrado) {
        cout << "ERROR: No existe un cliente activo con ese ID.\n";
        fclose(f);
        return;
    }

    // Guardamos una copia para comparar o restaurar
    Cliente original = temp;

    cout << "\n=== CLIENTE ENCONTRADO ===\n";
    mostrarCliente(original);

    if (!confirmar("\n¿Desea modificar este cliente? (S/N): ")) {
        fclose(f);
        return;
    }

    char buffer[200];

    // --- Nombre ---
    cout << "\nNombre actual: " << original.nombre << endl;
    solicitarString("Nuevo nombre (ENTER para mantener): ", buffer, 100);
    if (strlen(buffer) > 0) strcpy(temp.nombre, buffer);

    // --- Cedula/RIF ---
    cout << "\nCedula/RIF actual: " << original.cedula << endl;
    solicitarString("Nueva cedula/RIF (ENTER para mantener): ", buffer, 20);
    if (strlen(buffer) > 0) {
        // Validamos duplicado en el archivo, ignorando el ID actual
        if (clienteDuplicado(buffer, original.id)) {
            cout << "ERROR: La cedula '" << buffer << "' ya pertenece a otro cliente.\n";
            fclose(f); return;
        }
        strcpy(temp.cedula, buffer);
    }

    // --- Telefono ---
    cout << "\nTelefono actual: " << original.telefono << endl;
    solicitarString("Nuevo telefono (ENTER para mantener): ", buffer, 20);
    if (strlen(buffer) > 0) strcpy(temp.telefono, buffer);

    // --- Email ---
    cout << "\nEmail actual: " << original.email << endl;
    solicitarString("Nuevo email (ENTER para mantener): ", buffer, 100);
    if (strlen(buffer) > 0) {
        if (!emailValido(buffer)) {
            cout << "ERROR: Formato de email invalido.\n";
            fclose(f); return;
        }
        strcpy(temp.email, buffer);
    }

    // --- Direccion ---
    cout << "\nDireccion actual: " << original.direccion << endl;
    solicitarString("Nueva direccion (ENTER para mantener): ", buffer, 200);
    if (strlen(buffer) > 0) strcpy(temp.direccion, buffer);

    // --- Confirmación y Guardado ---
    cout << "\n=== NUEVOS DATOS DEL CLIENTE ===\n";
    mostrarCliente(temp);

    if (confirmar("¿Guardar cambios en el archivo? (S/N): ")) {
        // LA CLAVE: Volvemos a donde estaba el registro original
        fseek(f, posicionRegistro, SEEK_SET); 
        fwrite(&temp, sizeof(Cliente), 1, f);
        cout << "Cliente actualizado exitosamente en el disco.\n";
    }

    fclose(f);
}

//2.4.4

void listarClientes() {
    FILE* f = fopen("clientes.bin", "rb");
    if (!f) {
        cout << "No hay clientes registrados o el archivo no existe.\n";
        return;
    }

    // Leemos el Header para auditoría
    ArchivoHeader h;
    fread(&h, sizeof(ArchivoHeader), 1, f);

    if (h.cantidadRegistros == 0) {
        cout << "La base de datos de clientes esta vacia.\n";
        fclose(f);
        return;
    }

    encabezadoClientes(); // Tu función de formato

    Cliente c;
    int contadorActivos = 0;

    // Recorrido secuencial del archivo binario
    while (fread(&c, sizeof(Cliente), 1, f)) {
        // Solo mostramos si el flag de borrado es falso
        if (!c.eliminado) {
            filaCliente(c); // Tu función que imprime la línea de la tabla
            contadorActivos++;
        }
    }

    pieClientes();

    // Información de control de archivos (muy valorado en la URU)
    cout << "\nTotal registros en archivo: " << h.cantidadRegistros;
    cout << "\nClientes activos listados: " << contadorActivos << "\n";

    fclose(f);
}

//2.4.5

void eliminarCliente() {
    int idBuscado;
    cout << "Ingrese el ID del cliente a eliminar (0 para cancelar): ";
    cin >> idBuscado;
    cin.ignore();

    if (idBuscado == 0) return;

    // 1. Abrimos el archivo en modo r+b para poder sobreescribir el flag
    FILE* f = fopen("clientes.bin", "r+b");
    if (!f) {
        cout << "ERROR: No se pudo abrir el archivo de clientes.\n";
        return;
    }

    ArchivoHeader h;
    fread(&h, sizeof(ArchivoHeader), 1, f);

    Cliente c;
    long posicionReg = -1;
    bool encontrado = false;

    // 2. Buscamos la posición exacta del cliente en el archivo
    for (int i = 0; i < h.cantidadRegistros; i++) {
        posicionReg = ftell(f); // Guardamos la ubicación actual antes de leer
        if (fread(&c, sizeof(Cliente), 1, f)) {
            if (!c.eliminado && c.id == idBuscado) {
                encontrado = true;
                break;
            }
        }
    }

    if (!encontrado) {
        cout << "ERROR: No existe un cliente activo con ID " << idBuscado << ".\n";
        fclose(f);
        return;
    }

    // 3. Verificación de Transacciones en Disco (Integridad Referencial)
    if (clienteTieneVentasBin(idBuscado)) {
        cout << "\nERROR: No se puede eliminar a " << c.nombre << ".\n";
        cout << "Existen ventas registradas a su nombre en transacciones.bin.\n";
        fclose(f);
        return;
    }

    cout << "\n=== CLIENTE A ELIMINAR ===\n";
    mostrarCliente(c);

    if (!confirmar("\n¿Confirmar borrado logico? (S/N): ")) {
        cout << "Operacion cancelada.\n";
        fclose(f);
        return;
    }

    // 4. Marcamos como eliminado y sobreescribimos
    c.eliminado = true;
    fseek(f, posicionReg, SEEK_SET); // Saltamos directo al registro
    fwrite(&c, sizeof(Cliente), 1, f);
    fclose(f);

    // 5. Actualizamos el Header (Bajamos los registros activos)
    h = leerHeader("clientes.bin");
    h.registrosActivos--;
    actualizarHeader("clientes.bin", h);

    cout << "Cliente eliminado logicamente con exito.\n";
}

void menuClientes() {
    int opcion;

    do {
        system("cls");
        cout << "+-------------------------------------------+\n";
        cout << "|             GESTION DE CLIENTES           |\n"; 
        cout << "+-------------------------------------------+\n\n";

        cout << "1. Registrar cliente\n";
        cout << "2. Buscar cliente\n";
        cout << "3. Actualizar cliente\n";
        cout << "4. Listar clientes\n";
        cout << "5. Eliminar cliente\n";
        cout << "0. Volver al menu principal\n\n";
        cout << "Seleccione una opcion: ";

        cin >> opcion;
        cin.ignore();

        system("cls");

        switch (opcion) {
            case 1: crearCliente(); break;      // Operación directa en clientes.bin
            case 2: buscarCliente(); break;     // Búsqueda en disco
            case 3: actualizarCliente(); break; // fseek + r+b en disco
            case 4: listarClientes(); break;    // Lectura secuencial de clientes.bin
            case 5: eliminarCliente(); break;   // Borrado lógico en disco
            case 0: break;
            default: cout << "Opcion invalida.\n";
        }

        if (opcion != 0) {
            cout << "\n";
            system("pause");
        }

    } while (opcion != 0);
}

//==============
// 2.5
//==============

void registrarCompra() { // Sin Tienda*
    cout << "\n=== REGISTRAR COMPRA (DISCO) ===\n";

    // 1. Validar Producto
    int idProd;
    cout << "Ingrese ID del producto (0 para cancelar): ";
    cin >> idProd; cin.ignore();
    if (idProd == 0) return;

    Producto prod; // Creamos una variable local para recibir los datos
	if (buscarProductoPorID(idProd, &prod)) { // Solo pasamos ID y la dirección de la variable
    // Si entra aquí, 'prod' ya tiene los datos cargados desde el archivo
    cout << "Producto encontrado: " << prod.nombre << endl;
	} else {
    cout << "Error: Producto no existe en el archivo." << endl;
	}

    // 2. Validar Proveedor
    int idProv;
    cout << "Ingrese ID del proveedor (0 para cancelar): ";
    cin >> idProv; cin.ignore();
    if (idProv == 0) return;

    Proveedor prov;
    if (!buscarProveedorPorIDBin(idProv, &prov)) { // Función que ya hicimos
        cout << "ERROR: El proveedor no existe o esta eliminado.\n";
        return;
    }

    // 3. Captura de Datos
    int cantidad = solicitarEnteroPositivo("Cantidad comprada (>0): ");
    float precioUnit;
    cout << "Precio unitario de compra (>0): ";
    cin >> precioUnit;
    if (precioUnit <= 0) {
        cout << "ERROR: Precio invalido.\n";
        return;
    }

    float total = cantidad * precioUnit;

    // 4. Resumen
    cout << "\n=== RESUMEN DE COMPRA ===\n";
    cout << "Producto:  " << prod.nombre << " (Stock actual: " << prod.stock << ")\n";
    cout << "Proveedor: " << prov.nombre << "\n";
    cout << "Cantidad:  " << cantidad << "\n";
    cout << "Total:     " << total << "\n";

    if (!confirmar("¿Confirmar ingreso a almacen? (S/N): ")) return;

    // 5. OPERACIONES EN DISCO (Punto crítico)
    
    // A. Actualizar Stock en productos.bin
    if (actualizarStockProducto(idProd, cantidad)) { // Suma la cantidad al stock
        
        // B. Crear el registro de la Transacción
        Transaccion t;
        t.id = idProd;
        t.idRelacionado = idProv; // ID del Proveedor
        t.cantidades[20] = cantidad;
        t.precioUnitario[20] = precioUnit;
        t.total = total;
        strcpy(t.tipo, "COMPRA");
        strcpy(t.descripcion, "Compra a proveedor");
        obtenerFechaActual(t.fechaRegistro);
        t.eliminado = false;

        registrarTransaccionEnDisco(t);

        cout << "\nInventario actualizado y compra registrada exitosamente.\n";
    } else {
        cout << "\nERROR CRITICO: No se pudo actualizar el stock en el archivo.\n";
    }
}

// 2.5.2

  void registrarVenta() {
    cout << "\n=== REGISTRAR VENTA (DISCO) ===\n";

    // 1. Validar Producto
    int idProd;
    cout << "Ingrese ID del producto (0 para cancelar): ";
    cin >> idProd; cin.ignore();
    if (idProd == 0) return;

    Producto p;
    if (!buscarProductoPorID(idProd, &p)) {
        cout << "ERROR: El producto no existe.\n";
        return;
    }

    if (p.stock <= 0) {
        cout << "ERROR: Producto sin stock disponible.\n";
        return;
    }

    // 2. Validar Cliente
    int idCli;
    cout << "Ingrese ID del cliente (0 para cancelar): ";
    cin >> idCli; cin.ignore();
    if (idCli == 0) return;

    Cliente cli;
    if (!buscarClientePorIDBin(idCli, &cli)) {
        cout << "ERROR: El cliente no existe.\n";
        return;
    }

    // 3. Cantidad y Cálculos
    cout << "Stock disponible: " << p.stock << endl;
    int cantidad = solicitarEnteroPositivo("Cantidad vendida (>0): ");

    if (cantidad > p.stock) {
        cout << "ERROR: Stock insuficiente.\n";
        return;
    }

    float precioUnit = p.precio;
    float total = precioUnit * (float)cantidad;

    // 4. Resumen
    cout << "\n=== RESUMEN DE VENTA ===\n";
    cout << "Producto: " << p.nombre << endl;
    cout << "Cliente:  " << cli.nombre << endl;
    cout << "Cantidad: " << cantidad << endl;
    cout << "Total:     " << total << endl;

    if (!confirmar("¿Confirmar venta? (S/N): ")) return;

    // 5. REGISTRO EN DISCO
    // A. Restar stock en productos.bin
    if (actualizarStockProducto(idProd, -cantidad)) { // Mandamos negativo para restar
        
        Transaccion t;
        // Limpiamos los arreglos de la transacción
        memset(t.idProductos, 0, sizeof(t.idProductos));
        memset(t.cantidades, 0, sizeof(t.cantidades));
        // t.preciosUnitarios es el nombre correcto según el struct nuevo
        memset(t.precioUnitario, 0, sizeof(t.precioUnitario));

        // Asignamos a la posición [0] porque es una venta de un solo item (por ahora)
        t.idRelacionado = idCli;
        t.idProductos[0] = idProd;
        t.cantidades[0] = cantidad;
        t.precioUnitario[0] = precioUnit;
        t.numItems = 1; // Indicamos que usamos 1 espacio de los 20
        
        t.total = total;
        strcpy(t.tipo, "VENTA");
        strcpy(t.descripcion, "Venta a cliente");
        obtenerFechaActual(t.fechaRegistro); // Asegúrate que el campo se llame fechaRegistro
        t.eliminado = false;

        registrarTransaccionEnDisco(t);

        cout << "\nVenta registrada y stock actualizado.\n";
    } else {
        cout << "ERROR CRITICO: No se pudo actualizar el stock en disco.\n";
    }
}

//2.5.3

void buscarTransacciones() {
    FILE* f = fopen("transacciones.bin", "rb");
    if (!f) {
        cout << "No hay transacciones registradas.\n";
        return;
    }

    cout << "\n=== BUSCAR TRANSACCIONES (DISCO) ===\n";
    cout << "1. Por ID de factura\n";
    cout << "2. Por ID de producto (contenido en factura)\n";
    cout << "3. Por ID de cliente/proveedor\n";
    cout << "4. Por fecha (YYYY-MM-DD)\n";
    cout << "0. Cancelar\n";
    cout << "Opcion: ";

    int op;
    cin >> op; cin.ignore();
    if (op == 0) { fclose(f); return; }

    int idBuscado = -1;
    char filtro[50] = "";

    // Pedimos el criterio antes de entrar al bucle para no pedirlo 100 veces
    if (op == 1 || op == 2 || op == 3) {
        cout << "Ingrese el ID a buscar: ";
        cin >> idBuscado; cin.ignore();
    } else if (op == 4) {
        solicitarString("Ingrese fecha (YYYY-MM-DD): ", filtro, 11);
    }

    // Saltamos el Header
    fseek(f, sizeof(ArchivoHeader), SEEK_SET);

    Transaccion t;
    bool encontrado = false;
    cout << "\n=== RESULTADOS ENCONTRADOS ===\n";

    while (fread(&t, sizeof(Transaccion), 1, f)) {
        if (t.eliminado) continue;

        bool coincide = false;

        switch (op) {
            case 1: // Por ID de factura
                coincide = (t.id == idBuscado);
                break;

            case 2: // Por ID de producto (Buscamos en el arreglo de 20)
                for (int j = 0; j < t.numItems; j++) {
                    if (t.idProductos[j] == idBuscado) {
                        coincide = true;
                        break;
                    }
                }
                break;

            case 3: // Por ID de Relacionado (Cliente o Proveedor)
                coincide = (t.idRelacionado == idBuscado);
                break;

            case 4: // Por Fecha
                coincide = (strcmp(t.fechaRegistro, filtro) == 0);
                break;
        }

        if (coincide) {
            mostrarDetalleTransaccion(t); // Ahora recibe la transaccion directamente
            encontrado = true;
            cout << "-------------------------------------------\n";
        }
    }

    if (!encontrado) cout << "No se encontraron transacciones con ese criterio.\n";
    
    fclose(f);
}

//2.5.4

void listarTransacciones() {
    FILE* f = fopen("transacciones.bin", "rb");
    if (!f) {
        cout << "No hay historial de transacciones o el archivo no existe.\n";
        return;
    }

    // Leemos el Header para control
    ArchivoHeader h;
    fread(&h, sizeof(ArchivoHeader), 1, f);

    if (h.cantidadRegistros == 0) {
        cout << "El historial de transacciones esta vacio.\n";
        fclose(f);
        return;
    }

    encabezadoTransacciones(); // Tu función de formato de tabla

    Transaccion t;
    int contActivas = 0;
    float recaudacionVentas = 0;
    float inversionCompras = 0;

    // Recorrido secuencial del archivo
    while (fread(&t, sizeof(Transaccion), 1, f)) {
        if (!t.eliminado) {
            filaTransaccion(t); // Imprime la línea resumen de la factura
            contActivas++;

            // Acumuladores para reporte rápido
            if (strcmp(t.tipo, "VENTA") == 0) recaudacionVentas += t.total;
            else if (strcmp(t.tipo, "COMPRA") == 0) inversionCompras += t.total;
        }
    }

    pieTransacciones();

    // Resumen financiero rápido (Muy útil para la sustentación en la URU)
    cout << "\n=== RESUMEN DE OPERACIONES ===";
    cout << "\nTotal Transacciones: " << contActivas;
    cout << "\nTotal en Ventas:     $" << recaudacionVentas;
    cout << "\nTotal en Compras:    $" << inversionCompras;
    cout << "\nBalance Neto:        $" << (recaudacionVentas - inversionCompras) << "\n";

    fclose(f);
}

//2.5.5

void cancelarTransaccion() {
    int idBuscado;
    cout << "ID de la transaccion a cancelar (0 para salir): ";
    cin >> idBuscado; cin.ignore();

    if (idBuscado == 0) return;

    FILE* f = fopen("transacciones.bin", "r+b");
    if (!f) {
        cout << "ERROR: No se pudo abrir el archivo de transacciones.\n";
        return;
    }

    Transaccion t;
    long posicionTransaccion = -1;
    bool encontrada = false;

    // 1. Buscar la transacción en el archivo
    fseek(f, sizeof(ArchivoHeader), SEEK_SET);
    while (fread(&t, sizeof(Transaccion), 1, f)) {
        if (t.id == idBuscado && !t.eliminado) {
            posicionTransaccion = ftell(f) - sizeof(Transaccion);
            encontrada = true;
            break;
        }
    }

    if (!encontrada) {
        cout << "ERROR: Transaccion no encontrada o ya cancelada.\n";
        fclose(f);
        return;
    }

    // 2. Mostrar detalle y confirmar
    mostrarDetalleTransaccion(t);
    if (!confirmar("\n¿Esta seguro de CANCELAR esta operacion? El stock se revertira. (S/N): ")) {
        fclose(f);
        return;
    }

    // 3. REVERTIR STOCK DE TODOS LOS PRODUCTOS EN LA FACTURA
    // Recorremos el arreglo de items que tiene la transaccion
    for (int i = 0; i < t.numItems; i++) {
        int idP = t.idProductos[i];
        int cant = t.cantidades[i];

        if (strcmp(t.tipo, "COMPRA") == 0) {
            // Si cancelo una compra, RESTO del stock (porque el producto nunca llego)
            actualizarStockProducto(idP, -cant);
        } else {
            // Si cancelo una venta, SUMO al stock (porque el producto "volvio" a la tienda)
            actualizarStockProducto(idP, cant);
        }
    }

    // 4. MARCAR COMO ELIMINADA (Borrado Lógico)
    t.eliminado = true;
    fseek(f, posicionTransaccion, SEEK_SET);
    fwrite(&t, sizeof(Transaccion), 1, f);
    fclose(f);

    // 5. ACTUALIZAR HEADER
    ArchivoHeader h = leerHeader("transacciones.bin");
    h.registrosActivos--;
    actualizarHeader("transacciones.bin", h);

    cout << "\nTransaccion cancelada exitosamente. El inventario ha sido restaurado.\n";
}

void menuTransacciones() {
    int opcion;

    do {
        system("cls");
        cout << "+-------------------------------------------+\n";
        cout << "|           MENU DE TRANSACCIONES           |\n";
        cout << "+-------------------------------------------+\n\n";
        
        cout << "1. Registrar compra (Entrada Inventario)\n";
        cout << "2. Registrar venta (Salida Inventario)\n";
        cout << "3. Buscar transacciones\n";
        cout << "4. Listar historial completo\n";
        cout << "5. Cancelar transaccion (Revertir stock)\n";
        cout << "0. Volver al menu principal\n\n";
        
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        cin.ignore();

        system("cls");

        switch (opcion) {
            case 1: registrarCompra(); break;      // Suma al stock en disco
            case 2: registrarVenta(); break;       // Resta al stock en disco
            case 3: buscarTransacciones(); break;  // Filtra en transacciones.bin
            case 4: listarTransacciones(); break;  // Lectura secuencial
            case 5: cancelarTransaccion(); break;  // Borrado logico + Reversion de stock
            case 0: 
                cout << "Volviendo al menu principal...\n"; 
                break;
            default: 
                cout << "Opcion invalida.\n";
        }

        if (opcion != 0) {
            cout << "\n";
            system("pause");
        }

    } while (opcion != 0);
}
//main 

int main() {
    setlocale(LC_ALL, "Spanish");

    Tienda tienda; // Declaramos la estructura

    // LLAMADA CORRECTA: (Puntero, Nombre, RIF)
    inicializarTienda(&tienda, "A&S TECHNOLOGY", "J-12345678-9");

    int opcion;

    do {
        system("cls");
        cout << "+-------------------------------------------+\n";
        cout << "|      SISTEMA DE GESTION DE INVENTARIO     |\n";
        cout << "|              A&S TECHNOLOGY               |\n";
        cout << "+-------------------------------------------+\n\n";

        cout << "1. Gestion de Productos\n";
        cout << "2. Gestion de Proveedores\n";
        cout << "3. Gestion de Clientes\n";
        cout << "4. Gestion de Transacciones\n";
        cout << "5. Salir\n\n";
        cout << "Seleccione una opcion: ";

        if (!(cin >> opcion)) { // Validacion por si el usuario mete letras
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }
        cin.ignore();

        switch (opcion) {
            case 1: menuProductos();      break; // Ya no enviamos &tienda
            case 2: menuProveedores();    break; // Ahora leen directo de archivos
            case 3: menuClientes();       break;
            case 4: menuTransacciones();  break;
            case 5: cout << "Saliendo y asegurando datos...\n"; break;
            default: cout << "Opcion invalida.\n";
        }

        if (opcion != 5) {
            cout << "\n";
            system("pause");
        }

    } while (opcion != 5);

    // Ya no necesitas "guardarAlFinal" porque cada vez que registras algo
    // en los submenús, ya se guardó automáticamente en el .bin.
    
    return 0;
}

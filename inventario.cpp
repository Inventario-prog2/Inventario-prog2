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
    int id;                    // Identificador único (autoincremental)
    char codigo[20];           // Código del producto (ej: "PROD-001")
    char nombre[100];          // Nombre del producto
    char descripcion[200];     // Descripción del producto
    int idProveedor;           // ID del proveedor asociado
    float precio;              // Precio unitario
    int stock;                 // Cantidad en inventario
    char fechaRegistro[11];    // Formato: YYYY-MM-DD
    
    int stockMinimo;                 
    int totalVendido;                
    bool eliminado;                  // Borrado l�gico
};

//1.2 Estructura Proveedor

struct Proveedor {
    int id;                    // Identificador único (autoincremental)
    char nombre[100];          // Nombre del proveedor
    char rif[20];              // RIF o identificación fiscal
    char telefono[20];         // Teléfono de contacto
    char email[100];           // Correo electrónico
    char direccion[200];       // Dirección física
    char fechaRegistro[11];    // Formato: YYYY-MM-DD
    bool eliminado;                  // Borrado l�gico
};

//1.3 Estructura Cliente

struct Cliente {
    int id;                    // Identificador único (autoincremental)
    char nombre[100];          // Nombre completo del cliente
    char cedula[20];           // Cédula o RIF
    char telefono[20];         // Teléfono de contacto
    char email[100];           // Correo electrónico
    char direccion[200];       // Dirección física
    char fechaRegistro[11];    // Formato: YYYY-MM-DD
    bool eliminado;                  // Borrado l�gico
};

//1.4 Estructura Transacción (CASO ESPECIAL: Esta estructura puede separarse como se comentó en clase, tienen libertad de hacerlo.)

struct Transaccion {
    int id;
    char tipo[10];           // "COMPRA" o "VENTA"
    int idRelacionado;       // ID del proveedor o cliente
    
    // Soporte para m�ltiples productos (M�ximo 20 por factura)
    int idProductos[20];    
    int cantidades[20];      
    int numItems;            // Cu�ntos de los 20 espacios se usaron
    
    float total;
    char fechaRegistro[11];
    char descripcion[200];
    bool eliminado;          // Borrado l�gico
};

//1.5 Estructura Principal: Tienda

struct Tienda {
    char nombre[100];
    char rif[20];
    char direccion[200];
    char telefono[20];
    
    // Estad�sticas globales (�tiles para reportes)
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
        // El archivo no existe, lo creamos
        f = fopen(nombreArchivo, "wb");
        if (f) {
            ArchivoHeader h = {0, 0}; // cantidadRegistros = 0, ultimoId = 0
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
                     const char* mensajeError = "ERROR: Valor inválido.\n") 
{
    while (true) {
        cout << mensaje;
        cin.getline(destino, maxLen);

        if (strlen(destino) == 0) {
            cout << "ERROR: No puede estar vacío.\n";
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


int buscarProductoPorID(int idBuscado) {
    FILE* f = fopen("productos.bin", "rb");
    if (!f) return -1;

    ArchivoHeader h;
    fread(&h, sizeof(ArchivoHeader), 1, f);

    Producto p;
    for (int i = 0; i < h.cantidadRegistros; i++) {
        fread(&p, sizeof(Producto), 1, f);
        if (!p.eliminado && p.id == idBuscado) {
            fclose(f);
            return i; // Retornamos el �ndice en el archivo
        }
    }
    fclose(f);
    return -1;
}

bool existeProducto(int idBuscado) {
    return buscarProductoPorID(idBuscado) != -1;
}

void mostrarProducto(const Producto& p) {
    cout << "Código: " << p.codigo << endl;
    cout << "Nombre: " << p.nombre << endl;
    cout << "Descripción: " << p.descripcion << endl;
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
        cout << "¦--------------------------------------------------------------------------¦\n";
    else if (strcmp(tipo, "sep") == 0)
        cout << "¦----+-----------+------------------+--------------+-------+--------+------¦\n";
    else if (strcmp(tipo, "bot") == 0)
        cout << "+--------------------------------------------------------------------------+\n";
}

//encabezado
void encabezadoProductos() {
    linea("top");
    cout << "¦                         LISTADO DE PRODUCTOS                             ¦\n";
    linea("mid");
    cout << "¦ ID ¦  Código   ¦     Nombre       ¦  Proveedor   ¦ Precio¦ Stock  ¦ Fecha¦\n";
    linea("sep");
}

//fila
void filaProducto(const Producto& p, const char* nombreProv) {
    cout << "¦ "
         << setw(2)  << left << p.id << " ¦ "
         << setw(9)  << left << p.codigo << " ¦ "
         << setw(16) << left << p.nombre << " ¦ "
         << setw(12) << left << nombreProv << " ¦ "
         << setw(5)  << left << p.precio << " ¦ "
         << setw(6)  << left << p.stock << " ¦ "
         << setw(4)  << left << p.fechaRegistro << " ¦\n";
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
int buscarProveedorPorIDBin(int idBuscado) {
    FILE* f = fopen("proveedores.bin", "rb");
    if (!f) return -1;

    ArchivoHeader h;
    fread(&h, sizeof(ArchivoHeader), 1, f);

    Proveedor p;
    for (int i = 0; i < h.cantidadRegistros; i++) {
        fread(&p, sizeof(Proveedor), 1, f);
        if (!p.eliminado && p.id == idBuscado) {
            fclose(f);
            return i; // �ndice f�sico para fseek
        }
    }
    fclose(f);
    return -1;
}

int buscarProveedorPorRIFBin(const char* rif) {
    FILE* f = fopen("proveedores.bin", "rb");
    if (!f) return -1;

    ArchivoHeader h;
    fread(&h, sizeof(ArchivoHeader), 1, f);

    Proveedor p;
    for (int i = 0; i < h.cantidadRegistros; i++) {
        if (fread(&p, sizeof(Proveedor), 1, f)) {
            if (!p.eliminado && strcmp(p.rif, rif) == 0) {
                fclose(f);
                return i; // �ndice para fseek
            }
        }
    }
    fclose(f);
    return -1;
}

void mostrarProveedor(const Proveedor& p) {
    cout << "\n=== PROVEEDOR ENCONTRADO ===\n";
    cout << "ID: " << p.id << endl;
    cout << "RIF: " << p.rif << endl;
    cout << "Nombre: " << p.nombre << endl;
    cout << "Email: " << p.email << endl;
    cout << "Teléfono: " << p.telefono << endl;
}


bool existeProveedorBin(int idBuscado) {
    return buscarProveedorPorIDBin(idBuscado) != -1;
}

//================
// tabla proveedor
//================
void lineaProv(const char* tipo) {
    if (strcmp(tipo, "top") == 0)
        cout << "+--------------------------------------------------------------------------+\n";
    else if (strcmp(tipo, "mid") == 0)
        cout << "¦--------------------------------------------------------------------------¦\n";
    else if (strcmp(tipo, "sep") == 0)
        cout << "¦----+----------------------+------------------+---------------------------¦\n";
    else if (strcmp(tipo, "bot") == 0)
        cout << "+--------------------------------------------------------------------------+\n";
}
void encabezadoProveedores() {
    lineaProv("top");
    cout << "¦                         LISTADO DE PROVEEDORES                           ¦\n";
    lineaProv("mid");
    cout << "¦ ID ¦       Nombre         ¦      RIF        ¦        Teléfono           ¦\n";
    lineaProv("sep");
}
void filaProveedor(const Proveedor& p) {
    cout << "¦ "
         << setw(2)  << left << p.id << " ¦ "
         << setw(20) << left << p.nombre << " ¦ "
         << setw(16) << left << p.rif << " ¦ "
         << setw(25) << left << p.telefono << " ¦\n";
}
void pieProveedores() {
    lineaProv("bot");
}

void buscarYMostrarProveedoresPorNombreBin(const char* filtro) {
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

int buscarClientePorIDBin(int idBuscado) {
    FILE* f = fopen("clientes.bin", "rb");
    if (!f) return -1;

    ArchivoHeader h;
    fread(&h, sizeof(ArchivoHeader), 1, f);

    Cliente c;
    for (int i = 0; i < h.cantidadRegistros; i++) {
        fread(&c, sizeof(Cliente), 1, f);
        if (!c.eliminado && c.id == idBuscado) {
            fclose(f);
            return i;
        }
    }
    fclose(f);
    return -1;
}

bool existeClienteBin(int idBuscado) {
    return buscarClientePorIDBin(idBuscado) != -1;
}

int buscarClientePorCedulaBin(const char* cedula) {
    FILE* f = fopen("clientes.bin", "rb");
    if (!f) return -1;

    ArchivoHeader h;
    fread(&h, sizeof(ArchivoHeader), 1, f);

    Cliente c;
    for (int i = 0; i < h.cantidadRegistros; i++) {
        if (fread(&c, sizeof(Cliente), 1, f)) {
            if (!c.eliminado && strcmp(c.cedula, cedula) == 0) {
                fclose(f);
                return i; // Retorna el �ndice f�sico
            }
        }
    }
    fclose(f);
    return -1;
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
    cout << "Cédula/RIF: " << c.cedula << endl;
    cout << "Nombre: " << c.nombre << endl;
    cout << "Email: " << c.email << endl;
    cout << "Teléfono: " << c.telefono << endl;
    cout << "Dirección: " << c.direccion << endl;
}

//==============
//tabla clientes
//==============

void lineaClientes(const char* tipo) {
    if (strcmp(tipo, "top") == 0)
        cout << "+-------------------------------------------------------------------------------------------+\n";
    else if (strcmp(tipo, "mid") == 0)
        cout << "¦-------------------------------------------------------------------------------------------¦\n";
    else if (strcmp(tipo, "sep") == 0)
        cout << "¦----+----------------------+------------------+----------------------+----------------------¦\n";
    else if (strcmp(tipo, "bot") == 0)
        cout << "+-------------------------------------------------------------------------------------------+\n";
}

void encabezadoClientes() {
    lineaClientes("top");
    cout << "¦                                 LISTADO DE CLIENTES                                       ¦\n";
    lineaClientes("mid");
    cout << "¦ ID ¦       Nombre         ¦   Cédula/RIF    ¦       Teléfono       ¦        Email         ¦\n";
    lineaClientes("sep");
}

void filaCliente(const Cliente& c) {
    cout << "¦ "
         << setw(2)  << left << c.id << " ¦ "
         << setw(20) << left << c.nombre << " ¦ "
         << setw(16) << left << c.cedula << " ¦ "
         << setw(20) << left << c.telefono << " ¦ "
         << setw(20) << left << c.email << " ¦\n";
}

void pieClientes() {
    lineaClientes("bot");
}

void buscarYMostrarClientesPorNombreBin(const char* filtro) {
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

//=======================
// tabla transacciones
//=======================

void lineaTrans(const char* tipo) {
    if (strcmp(tipo, "top") == 0)
        cout << "+-----------------------------------------------------------------------------------------------------------+\n";
    else if (strcmp(tipo, "mid") == 0)
        cout << "¦-----------------------------------------------------------------------------------------------------------¦\n";
    else if (strcmp(tipo, "sep") == 0)
        cout << "¦----+----------+----------+----------------------+----------+-----------+------------+---------------------¦\n";
    else if (strcmp(tipo, "bot") == 0)
        cout << "+-----------------------------------------------------------------------------------------------------------+\n";
}

void encabezadoTransacciones() {
    lineaTrans("top");
    cout << "¦                                      LISTADO DE TRANSACCIONES                                             ¦\n";
    lineaTrans("mid");
    cout << "¦ ID ¦  Tipo    ¦ Producto ¦ Relacionado (Prov/Cliente) ¦ Cantidad ¦  Total    ¦   Fecha    ¦ Descripción     ¦\n";
    lineaTrans("sep");
}

void filaTransaccion(const Transaccion& t) {
    cout << "¦ "
         << setw(2)  << left << t.id << " ¦ "
         << setw(8)  << left << t.tipo << " ¦ "
         << setw(8)  << left << t.idProductos << " ¦ "
         << setw(22) << left << t.idRelacionado << " ¦ "
         << setw(8)  << left << t.cantidades << " ¦ "
         << setw(9)  << left << t.total << " ¦ "
         << setw(10) << left << t.fechaRegistro << " ¦ "
         << setw(15) << left << t.descripcion << " ¦\n";
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
    
    // Estad�sticas iniciales
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

    // Asignamos el ID autoincremental del Header
    nuevo.id = header.proximoID;
    nuevo.eliminado = false; // Flag para borrado l�gico

    // 'ab' (append binary) coloca el cursor al final autom�ticamente
    FILE* f = fopen(nombreArchivo, "ab");
    if (f) {
        fwrite(&nuevo, sizeof(Producto), 1, f);
        fclose(f);

        // Actualizamos contadores del Header
        header.cantidadRegistros++;
        header.proximoID++;
        header.registrosActivos++;
        actualizarHeader(nombreArchivo, header);
    }
}

void cargarProductosBinario(Tienda* tienda) {
    ifstream in("productos.bin", ios::binary);
    if (!in) return; // si no existe, se deja todo como está

    ArchivoHeader header;
    in.read(reinterpret_cast<char*>(&header), sizeof(header));
    if (!in) return;

    // ajustar capacidad
    if (header.cantidadRegistros > tienda->capacidadProductos) {
        delete[] tienda->productos;
        tienda->capacidadProductos = max(header.cantidadRegistros, tienda->capacidadProductos);
        tienda->productos = new Producto[tienda->capacidadProductos];
    }

    if (header.cantidadRegistros > 0) {
        in.read(reinterpret_cast<char*>(tienda->productos),
                sizeof(Producto) * header.cantidadRegistros);
        if (!in) return;
    }

    tienda->numProductos = header.cantidadRegistros;
    tienda->siguienteIdProducto = header.proximoID;
}

void guardarProveedoresBinario(Tienda* tienda) {
    ofstream out("proveedores.bin", ios::binary | ios::trunc);
    if (!out) return;

    ArchivoHeader header;
    header.cantidadRegistros = tienda->numProveedores;
    header.proximoID = tienda->siguienteIdProveedor;
    header.registrosActivos = tienda->numProveedores;
    header.version = 1;

    out.write(reinterpret_cast<char*>(&header), sizeof(header));

    if (tienda->numProveedores > 0) {
        out.write(reinterpret_cast<char*>(tienda->proveedores),
                  sizeof(Proveedor) * tienda->numProveedores);
    }
}

void cargarProveedoresBinario(Tienda* tienda) {
    ifstream in("proveedores.bin", ios::binary);
    if (!in) return;

    ArchivoHeader header;
    in.read(reinterpret_cast<char*>(&header), sizeof(header));
    if (!in) return;

    if (header.cantidadRegistros > tienda->capacidadProveedores) {
        delete[] tienda->proveedores;
        tienda->capacidadProveedores = max(header.cantidadRegistros, tienda->capacidadProveedores);
        tienda->proveedores = new Proveedor[tienda->capacidadProveedores];
    }

    if (header.cantidadRegistros > 0) {
        in.read(reinterpret_cast<char*>(tienda->proveedores),
                sizeof(Proveedor) * header.cantidadRegistros);
        if (!in) return;
    }

    tienda->numProveedores = header.cantidadRegistros;
    tienda->siguienteIdProveedor = header.proximoID;
}

void guardarClientesBinario(Tienda* tienda) {
    ofstream out("clientes.bin", ios::binary | ios::trunc);
    if (!out) return;

    ArchivoHeader header;
    header.cantidadRegistros = tienda->numClientes;
    header.proximoID = tienda->siguienteIdCliente;
    header.registrosActivos = tienda->numClientes;
    header.version = 1;

    out.write(reinterpret_cast<char*>(&header), sizeof(header));

    if (tienda->numClientes > 0) {
        out.write(reinterpret_cast<char*>(tienda->clientes),
                  sizeof(Cliente) * tienda->numClientes);
    }
}

void cargarClientesBinario(Tienda* tienda) {
    ifstream in("clientes.bin", ios::binary);
    if (!in) return;

    ArchivoHeader header;
    in.read(reinterpret_cast<char*>(&header), sizeof(header));
    if (!in) return;

    if (header.cantidadRegistros > tienda->capacidadClientes) {
        delete[] tienda->clientes;
        tienda->capacidadClientes = max(header.cantidadRegistros, tienda->capacidadClientes);
        tienda->clientes = new Cliente[tienda->capacidadClientes];
    }

    if (header.cantidadRegistros > 0) {
        in.read(reinterpret_cast<char*>(tienda->clientes),
                sizeof(Cliente) * header.cantidadRegistros);
        if (!in) return;
    }

    tienda->numClientes = header.cantidadRegistros;
    tienda->siguienteIdCliente = header.proximoID;
}

void guardarTransaccionesBinario(Tienda* tienda) {
    ofstream out("transacciones.bin", ios::binary | ios::trunc);
    if (!out) return;

    ArchivoHeader header;
    header.cantidadRegistros = tienda->numTransacciones;
    header.proximoID = tienda->siguienteIdTransaccion;
    header.registrosActivos = tienda->numTransacciones;
    header.version = 1;

    out.write(reinterpret_cast<char*>(&header), sizeof(header));

    if (tienda->numTransacciones > 0) {
        out.write(reinterpret_cast<char*>(tienda->transacciones),
                  sizeof(Transaccion) * tienda->numTransacciones);
    }
}

void cargarTransaccionesBinario(Tienda* tienda) {
    ifstream in("transacciones.bin", ios::binary);
    if (!in) return;

    ArchivoHeader header;
    in.read(reinterpret_cast<char*>(&header), sizeof(header));
    if (!in) return;

    if (header.cantidadRegistros > tienda->capacidadTransacciones) {
        delete[] tienda->transacciones;
        tienda->capacidadTransacciones = max(header.cantidadRegistros, tienda->capacidadTransacciones);
        tienda->transacciones = new Transaccion[tienda->capacidadTransacciones];
    }

    if (header.cantidadRegistros > 0) {
        in.read(reinterpret_cast<char*>(tienda->transacciones),
                sizeof(Transaccion) * header.cantidadRegistros);
        if (!in) return;
    }

    tienda->numTransacciones = header.cantidadRegistros;
    tienda->siguienteIdTransaccion = header.proximoID;
}


void mostrarDetalleTransaccion(const Transaccion& t) {
    // Buscamos el producto en el archivo para obtener su nombre real
    Producto p;
    bool encontrado = buscarProductoPorID(t.idProducto, &p);

    cout << "+-----------------------------------------------------------+\n";
    cout << "|              DETALLE DE TRANSACCION (DISCO)               |\n";
    cout << "+-----------------------------------------------------------+\n";
    cout << "| ID: " << setw(54) << left << t.id << "|\n";
    cout << "| Producto: " << setw(48) << left << (encontrado ? p.nombre : "No encontrado") << "|\n";
    cout << "| Cantidad: " << setw(48) << left << t.cantidad << "|\n";
    cout << "| Total:    $" << setw(47) << left << fixed << setprecision(2) << t.total << "|\n";
    cout << "| Fecha:    " << setw(48) << left << t.fecha << "|\n";
    cout << "+-----------------------------------------------------------+\n";
}


//======================
//2.2.1
//======================

void crearProducto(Tienda* tienda) {
    if (!confirmar("¿Desea registrar un nuevo producto? (S/N): "))
        return;

    Producto nuevo;

    // --- Código ---
   solicitarString(
    "Ingrese código del producto (o CANCELAR): ",
    nuevo.codigo,
    20,
    sinEspacios,
    "ERROR: El código no puede contener espacios.\n"
);

if (strcmp(nuevo.codigo, "CANCELAR") == 0 || strcmp(nuevo.codigo, "0") == 0)
    return;

if (codigoProductoDuplicado(tienda, nuevo.codigo)) {
    cout << "ERROR: El código '" << nuevo.codigo << "' ya está registrado.\n";
    return;
}

    // --- Nombre ---
    solicitarString("Ingrese nombre del producto (o CANCELAR): ", nuevo.nombre, 100);
    if (strcmp(nuevo.nombre, "CANCELAR") == 0 || strcmp(nuevo.nombre, "0") == 0)
        return;

    // --- Descripción ---
    solicitarString("Ingrese descripción (o CANCELAR): ", nuevo.descripcion, 200);
    if (strcmp(nuevo.descripcion, "CANCELAR") == 0 || strcmp(nuevo.descripcion, "0") == 0)
        return;

    // --- ID Proveedor ---
    cout << "Ingrese ID del proveedor (o 0 para cancelar): ";
    cin >> nuevo.idProveedor;

    if (nuevo.idProveedor == 0)
        return;

    if (!existeProveedor(tienda, nuevo.idProveedor)) {
        cout << "ERROR: El proveedor con ID " << nuevo.idProveedor << " no existe.\n";
        return;
    }

    // --- Precio ---
    nuevo.precio = solicitarEnteroPositivo("Ingrese precio (>0): ");

    // --- Stock ---
    nuevo.stock = solicitarEnteroNoNegativo("Ingrese stock (>=0): ");

    // Fecha automática
    obtenerFechaActual(nuevo.fechaRegistro);

    // --- Resumen ---
    cout << "\n=== RESUMEN DEL PRODUCTO ===\n";
    mostrarProducto(nuevo);

    if (!confirmar("¿Guardar producto? (S/N): "))
        return;

    // --- Redimensionar si es necesario ---
    if (tienda->numProductos >= tienda->capacidadProductos) {
    redimensionarProductos(tienda);
}


    // Guardado final
    nuevo.id = tienda->siguienteIdProducto++;
    tienda->productos[tienda->numProductos++] = nuevo;

    cout << "\nProducto registrado exitosamente.\n";
}

//========================
//2.2.2
//========================

void buscarProducto(Tienda* tienda) {
    int opcion;
    cout << "\n=== BUSCAR PRODUCTO ===\n";
    cout << "1. Buscar por ID\n";
    cout << "2. Buscar por nombre (parcial)\n";
    cout << "3. Buscar por código (parcial)\n";
    cout << "4. Listar por proveedor\n";
    cout << "0. Cancelar\n";
    cout << "Seleccione una opción: ";
    cin >> opcion;

    if (opcion == 0) return;

    char buffer[200];
    string filtro;

    switch (opcion) {

    // 1. Buscar por ID
    case 1: {
        int id;
        cout << "Ingrese ID del producto: ";
        cin >> id;

        int index = buscarProductoPorID(tienda, id);
        if (index == -1) {
            cout << "ERROR: No se encontró un producto con ID " << id << ".\n";
            return;
        }

        cout << "\n=== PRODUCTO ENCONTRADO ===\n";
        mostrarProducto(tienda->productos[index]);
        return;
    }

    // 2. Buscar por nombre (parcial)
    case 2: {
    solicitarString("Ingrese parte del nombre: ", buffer, 200);

    int numResultados = 0;
    int* indices = buscarProductosPorNombre(tienda, buffer, &numResultados);

    if (numResultados == 0) {
        cout << "No se encontraron coincidencias.\n";
        return;
    }

    cout << "\n=== RESULTADOS ===\n";
    for (int i = 0; i < numResultados; i++) {
        int idx = indices[i];
        mostrarProducto(tienda->productos[idx]);
        cout << "-----------------------------\n";
    }

    delete[] indices; // obligatorio
    return;
	}


    // 3. Buscar por código (parcial)
    case 3: {
        solicitarString("Ingrese parte del código: ", buffer, 200);
        filtro = toLower(buffer);

        bool encontrado = false;
        cout << "\n=== RESULTADOS ===\n";

        for (int i = 0; i < tienda->numProductos; i++) {
            if (contiene(tienda->productos[i].codigo, filtro)) {
                mostrarProducto(tienda->productos[i]);
                cout << "-----------------------------\n";
                encontrado = true;
            }
        }

        if (!encontrado)
            cout << "No se encontraron coincidencias.\n";

        return;
    }

    // 4. Listar por proveedor
    case 4: {
        int idProv;
        cout << "Ingrese ID del proveedor: ";
        cin >> idProv;

        if (!existeProveedor(tienda, idProv)) {
            cout << "ERROR: El proveedor con ID " << idProv << " no existe.\n";
            return;
        }

        bool encontrado = false;
        cout << "\n=== PRODUCTOS DEL PROVEEDOR " 
             << obtenerNombreProveedor(tienda, idProv) << " ===\n";

        for (int i = 0; i < tienda->numProductos; i++) {
            if (tienda->productos[i].idProveedor == idProv) {
                mostrarProducto(tienda->productos[i]);
                cout << "-----------------------------\n";
                encontrado = true;
            }
        }

        if (!encontrado)
            cout << "No hay productos asociados a ese proveedor.\n";

        return;
    }

    default:
        cout << "Opción inválida.\n";
        return;
    }
}

//========================
//2.2.3
//========================

void actualizarProducto(Tienda* tienda) {
    int id;
    cout << "Ingrese el ID del producto a actualizar: ";
    cin >> id;

    int index = buscarProductoPorID(tienda, id);
    if (index == -1) {
        cout << "ERROR: No existe un producto con ese ID.\n";
        return;
    }

    Producto* original = &tienda->productos[index];
    Producto temp = *original; // copia temporal

    int opcion;

    do {
        cout << "\n=== EDITAR PRODUCTO ===\n";
        cout << "1. Código: " << temp.codigo << endl;
        cout << "2. Nombre: " << temp.nombre << endl;
        cout << "3. Descripción: " << temp.descripcion << endl;
        cout << "4. Proveedor ID: " << temp.idProveedor << endl;
        cout << "5. Precio: " << temp.precio << endl;
        cout << "6. Stock: " << temp.stock << endl;
        cout << "7. Guardar cambios\n";
        cout << "0. Cancelar sin guardar\n";
        cout << "Opción: ";
        cin >> opcion;

        cin.ignore(); // limpiar buffer

        switch (opcion) {

        case 1: { // Código
            char nuevoCodigo[20];
            solicitarString("Nuevo código: ", nuevoCodigo, 20);

            if (codigoProductoDuplicado(tienda, nuevoCodigo, temp.id)) {
                cout << "ERROR: El código '" << nuevoCodigo << "' ya está registrado.\n";
            } else {
                strcpy(temp.codigo, nuevoCodigo);
            }
            break;
        }

        case 2: // Nombre
            solicitarString("Nuevo nombre: ", temp.nombre, 100);
            break;

        case 3: // Descripción
            solicitarString("Nueva descripción: ", temp.descripcion, 200);
            break;

        case 4: { // Proveedor
            int nuevoProv;
            cout << "Nuevo ID de proveedor: ";
            cin >> nuevoProv;

            if (!existeProveedor(tienda, nuevoProv)) {
                cout << "ERROR: El proveedor con ID " << nuevoProv << " no existe.\n";
            } else {
                temp.idProveedor = nuevoProv;
            }
            break;
        }

        case 5: // Precio
            temp.precio = solicitarEnteroPositivo("Nuevo precio (>0): ");
            break;

        case 6: // Stock
            temp.stock = solicitarEnteroNoNegativo("Nuevo stock (>=0): ");
            break;

        case 7: // Guardar cambios
            cout << "\n=== RESUMEN FINAL ===\n";
            mostrarProducto(temp);

            if (confirmar("¿Confirmar cambios? (S/N): ")) {
                *original = temp;
                cout << "Cambios guardados exitosamente.\n";
            } else {
                cout << "Cambios descartados.\n";
            }
            return;

        case 0:
            cout << "Cancelado. No se guardaron cambios.\n";
            return;

        default:
            cout << "Opción inválida.\n";
        }

    } while (true);
}

//========================
//2.2.4
//========================

void actualizarStockProducto(int idProducto, int cantidadCambio) {
    int indice = buscarProductoPorID(idProducto); // Tu funci�n ya devuelve el �ndice
    if (indice == -1) return;

    FILE* f = fopen("productos.bin", "r+b"); // r+b es vital para sobreescribir
    if (!f) return;

    Producto p;
    // Nos saltamos el header y llegamos al registro exacto
    fseek(f, sizeof(ArchivoHeader) + (indice * sizeof(Producto)), SEEK_SET);
    fread(&p, sizeof(Producto), 1, f);

    p.stock += cantidadCambio; // Actualizamos el dato en memoria

    // Volvemos atr�s para sobreescribir el registro con el nuevo stock
    fseek(f, sizeof(ArchivoHeader) + (indice * sizeof(Producto)), SEEK_SET);
    fwrite(&p, sizeof(Producto), 1, f);

    fclose(f);
}

//========================
//2.2.5
//========================

void listarProductos(Tienda* tienda) {
    if (tienda->numProductos == 0) {
        cout << "No hay productos registrados.\n";
        return;
    }

    encabezadoProductos();

    for (int i = 0; i < tienda->numProductos; i++) {
        Producto& p = tienda->productos[i];
        const char* nombreProv = obtenerNombreProveedor(tienda, p.idProveedor);
        filaProducto(p, nombreProv);
    }

    pieProductos();

    cout << "\nTotal de productos: " << tienda->numProductos << "\n";
}


//========================
//2.2.6
//========================

void eliminarProducto(Tienda* tienda) {
    int id;
    cout << "Ingrese el ID del producto a eliminar: ";
    cin >> id;

    int index = buscarProductoPorID(tienda, id);
    if (index == -1) {
        cout << "ERROR: No existe un producto con ese ID.\n";
        return;
    }

    Producto& p = tienda->productos[index];

    cout << "\n=== PRODUCTO A ELIMINAR ===\n";
    mostrarProducto(p);

    // Advertencia por transacciones asociadas
    bool tieneTransacciones = false;
    for (int i = 0; i < tienda->numTransacciones; i++) {
        if (tienda->transacciones[i].idProducto == id) {
            tieneTransacciones = true;
            break;
        }
    }

    if (tieneTransacciones) {
        cout << "\nADVERTENCIA: Este producto tiene transacciones asociadas.\n";
    }

    if (!confirmar("\n¿Eliminar producto? (S/N): ")) {
        cout << "Eliminación cancelada.\n";
        return;
    }

    // Eliminar moviendo elementos
    for (int i = index; i < tienda->numProductos - 1; i++) {
        tienda->productos[i] = tienda->productos[i + 1];
    }

    tienda->numProductos--;

    cout << "Producto eliminado exitosamente.\n";
}

void menuProductos(Tienda* tienda) {
    int opcion;

    do {
        system("cls");
        cout << "+-------------------------------------------+\n";
        cout << "¦        GESTIÓN DE PRODUCTOS              ¦\n";
        cout << "+-------------------------------------------+\n\n";

        cout << "1. Registrar nuevo producto\n";
        cout << "2. Buscar producto\n";
        cout << "3. Actualizar producto\n";
        cout << "4. Actualizar stock manualmente\n";
        cout << "5. Listar todos los productos\n";
        cout << "6. Eliminar producto\n";
        cout << "0. Volver al menú principal\n\n";
        cout << "Seleccione una opción: ";

        cin >> opcion;
        cin.ignore();

        system("cls");

        switch (opcion) {
            case 1: crearProducto(tienda); break;
            case 2: buscarProducto(tienda); break;
            case 3: actualizarProducto(tienda); break;
            case 4: actualizarStockProducto(tienda); break;
            case 5: listarProductos(tienda); break;
            case 6: eliminarProducto(tienda); break;
            case 0: break;
            default: cout << "Opción inválida.\n";
        }

        if (opcion != 0) system("pause");

    } while (opcion != 0);
}

//2.3

void crearProveedor(Tienda* tienda) {
    if (!confirmar("¿Desea registrar un nuevo proveedor? (S/N): "))
        return;

    Proveedor nuevo;

    // --- RIF ---
    solicitarString(
        "Ingrese RIF del proveedor (o CANCELAR): ",
        nuevo.rif,
        20,
        nullptr
    );

    if (strcmp(nuevo.rif, "CANCELAR") == 0 || strcmp(nuevo.rif, "0") == 0)
        return;

    if (rifDuplicado(tienda, nuevo.rif)) {
        cout << "ERROR: El RIF '" << nuevo.rif << "' ya está registrado.\n";
        return;
    }

    // --- Nombre ---
    solicitarString("Ingrese nombre del proveedor (o CANCELAR): ",
                    nuevo.nombre, 100);

    if (strcmp(nuevo.nombre, "CANCELAR") == 0 || strcmp(nuevo.nombre, "0") == 0)
        return;

    // --- Email ---
    solicitarString(
        "Ingrese email del proveedor (o CANCELAR): ",
        nuevo.email,
        100,
        emailValido,
        "ERROR: El email debe contener '@' y un punto.\n"
    );

    if (strcmp(nuevo.email, "CANCELAR") == 0 || strcmp(nuevo.email, "0") == 0)
        return;

    // --- Teléfono ---
    solicitarString("Ingrese teléfono del proveedor (o CANCELAR): ",
                    nuevo.telefono, 20);

    if (strcmp(nuevo.telefono, "CANCELAR") == 0 || strcmp(nuevo.telefono, "0") == 0)
        return;

    // --- Confirmación ---
    cout << "\n=== RESUMEN DEL PROVEEDOR ===\n";
    cout << "RIF: " << nuevo.rif << endl;
    cout << "Nombre: " << nuevo.nombre << endl;
    cout << "Email: " << nuevo.email << endl;
    cout << "Teléfono: " << nuevo.telefono << endl;

    if (!confirmar("¿Guardar proveedor? (S/N): "))
        return;

    // --- Redimensionar si es necesario ---
    if (tienda->numProveedores >= tienda->capacidadProveedores)
        redimensionarProveedores(tienda);

    // --- Guardar ---
    nuevo.id = tienda->siguienteIdProveedor++;
    tienda->proveedores[tienda->numProveedores++] = nuevo;

    cout << "Proveedor registrado exitosamente.\n";
}

//2.3.2

void buscarProveedor(Tienda* tienda) {
    int opcion;

    do {
        cout << "\n=== BUSCAR PROVEEDOR ===\n";
        cout << "1. Buscar por ID\n";
        cout << "2. Buscar por nombre (coincidencia parcial)\n";
        cout << "3. Buscar por RIF\n";
        cout << "0. Cancelar\n";
        cout << "Seleccione una opción: ";
        cin >> opcion;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (opcion == 0)
            return;

        int index = -1;

        switch (opcion) {

            case 1: {
                int id;
                cout << "Ingrese ID del proveedor: ";
                cin >> id;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                index = buscarProveedorPorID(tienda, id);
                break;
            }

            case 2: {
                char nombre[100];
                solicitarString("Ingrese nombre o parte del nombre: ", nombre, 100);
                index = buscarProveedorPorNombre(tienda, nombre);
                break;
            }

            case 3: {
                char rif[20];
                solicitarString("Ingrese RIF del proveedor: ", rif, 20);
                index = buscarProveedorPorRIF(tienda, rif);
                break;
            }

            default:
                cout << "Opción inválida.\n";
                continue;
        }

        if (index == -1) {
            cout << "No se encontró ningún proveedor con ese criterio.\n";
        } else {
            mostrarProveedor(tienda->proveedores[index]);
        }

        system("pause");

    } while (opcion != 0);
}

//2.3.3

void actualizarProveedor(Tienda* tienda) {
    int id;
    cout << "Ingrese el ID del proveedor a actualizar (0 para cancelar): ";
    cin >> id;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (id == 0)
        return;

    int index = buscarProveedorPorID(tienda, id);
    if (index == -1) {
        cout << "ERROR: No existe un proveedor con ese ID.\n";
        return;
    }

    Proveedor& original = tienda->proveedores[index];
    Proveedor temp = original; // copia temporal

    cout << "\n=== PROVEEDOR ENCONTRADO ===\n";
    mostrarProveedor(original);

    if (!confirmar("\n¿Desea modificar este proveedor? (S/N): "))
        return;

    // --- Nombre ---
    cout << "\nNombre actual: " << temp.nombre << endl;
    solicitarString("Nuevo nombre (ENTER para mantener): ", temp.nombre, 100);
    if (strlen(temp.nombre) == 0)
        strcpy(temp.nombre, original.nombre);

    // --- RIF ---
    cout << "\nRIF actual: " << temp.rif << endl;
    solicitarString("Nuevo RIF (ENTER para mantener): ", temp.rif, 20);
    if (strlen(temp.rif) == 0)
        strcpy(temp.rif, original.rif);
    else if (rifDuplicado(tienda, temp.rif, original.id)) {
        cout << "ERROR: El RIF '" << temp.rif << "' ya está registrado.\n";
        return;
    }

    // --- Teléfono ---
    cout << "\nTeléfono actual: " << temp.telefono << endl;
    solicitarString("Nuevo teléfono (ENTER para mantener): ", temp.telefono, 20);
    if (strlen(temp.telefono) == 0)
        strcpy(temp.telefono, original.telefono);

    // --- Email ---
    cout << "\nEmail actual: " << temp.email << endl;
    solicitarString("Nuevo email (ENTER para mantener): ", temp.email, 100);
    if (strlen(temp.email) == 0)
        strcpy(temp.email, original.email);
    else if (!emailValido(temp.email)) {
        cout << "ERROR: El email debe contener '@' y un dominio.\n";
        return;
    }

    // --- Dirección ---
    cout << "\nDirección actual: " << temp.direccion << endl;
    solicitarString("Nueva dirección (ENTER para mantener): ", temp.direccion, 200);
    if (strlen(temp.direccion) == 0)
        strcpy(temp.direccion, original.direccion);

    // --- Confirmación final ---
    cout << "\n=== NUEVOS DATOS DEL PROVEEDOR ===\n";
    mostrarProveedor(temp);

    if (!confirmar("¿Guardar cambios? (S/N): "))
        return;

    original = temp;
    cout << "Proveedor actualizado exitosamente.\n";
}

//2.3.4

void listarProveedores(Tienda* tienda) {
    if (tienda->numProveedores == 0) {
        cout << "No hay proveedores registrados.\n";
        return;
    }

    encabezadoProveedores();

    for (int i = 0; i < tienda->numProveedores; i++) {
        filaProveedor(tienda->proveedores[i]);
    }

    pieProveedores();

    cout << "\nTotal de proveedores: " << tienda->numProveedores << "\n";
}

//2.3.5

void eliminarProveedor(Tienda* tienda) {
    int id;
    cout << "Ingrese el ID del proveedor a eliminar (0 para cancelar): ";
    cin >> id;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (id == 0)
        return;

    int index = buscarProveedorPorID(tienda, id);
    if (index == -1) {
        cout << "ERROR: No existe un proveedor con ese ID.\n";
        return;
    }

    Proveedor& p = tienda->proveedores[index];

    cout << "\n=== PROVEEDOR A ELIMINAR ===\n";
    mostrarProveedor(p);

    // --- Verificar si tiene productos asociados ---
    bool tieneProductos = false;

    for (int i = 0; i < tienda->numProductos; i++) {
        if (tienda->productos[i].idProveedor == id) {
            tieneProductos = true;
            break;
        }
    }

    if (tieneProductos) {
        cout << "\nADVERTENCIA: Este proveedor tiene productos asociados.\n";
        cout << "No se puede eliminar mientras existan productos vinculados.\n";
        return;
    }

    // --- Confirmación ---
    if (!confirmar("\n¿Eliminar proveedor? (S/N): ")) {
        cout << "Eliminación cancelada.\n";
        return;
    }

    // --- Eliminar moviendo elementos ---
    for (int i = index; i < tienda->numProveedores - 1; i++) {
        tienda->proveedores[i] = tienda->proveedores[i + 1];
    }

    tienda->numProveedores--;

    cout << "Proveedor eliminado exitosamente.\n";
}

void menuProveedores(Tienda* tienda) {
    int opcion;

    do {
        system("cls");
        cout << "+-------------------------------------------+\n";
        cout << "¦       GESTIÓN DE PROVEEDORES             ¦\n";
        cout << "+-------------------------------------------+\n\n";

        cout << "1. Registrar proveedor\n";
        cout << "2. Buscar proveedor\n";
        cout << "3. Actualizar proveedor\n";
        cout << "4. Listar proveedores\n";
        cout << "5. Eliminar proveedor\n";
        cout << "0. Volver al menú principal\n\n";
        cout << "Seleccione una opción: ";

        cin >> opcion;
        cin.ignore();

        system("cls");

        switch (opcion) {
            case 1: crearProveedor(tienda); break;
            case 2: buscarProveedor(tienda); break;
            case 3: actualizarProveedor(tienda); break;
            case 4: listarProveedores(tienda); break;
            case 5: eliminarProveedor(tienda); break;
            case 0: break;
            default: cout << "Opción inválida.\n";
        }

        if (opcion != 0) system("pause");

    } while (opcion != 0);
}

//=======================
//2.4
//=======================

void crearCliente(Tienda* tienda) {
    if (!confirmar("¿Desea registrar un nuevo cliente? (S/N): "))
        return;

    Cliente nuevo;

    // --- Cédula / RIF ---
    solicitarString(
        "Ingrese cédula/RIF del cliente (o CANCELAR): ",
        nuevo.cedula,
        20
    );

    if (strcmp(nuevo.cedula, "CANCELAR") == 0 || strcmp(nuevo.cedula, "0") == 0)
        return;

    if (clienteDuplicado(tienda, nuevo.cedula)) {
        cout << "ERROR: La cédula/RIF '" << nuevo.cedula << "' ya está registrada.\n";
        return;
    }

    // --- Nombre ---
    solicitarString("Ingrese nombre del cliente (o CANCELAR): ",
                    nuevo.nombre, 100);

    if (strcmp(nuevo.nombre, "CANCELAR") == 0 || strcmp(nuevo.nombre, "0") == 0)
        return;

    // --- Email ---
    solicitarString(
        "Ingrese email del cliente (o CANCELAR): ",
        nuevo.email,
        100,
        emailValido,
        "ERROR: El email debe contener '@' y un punto.\n"
    );

    if (strcmp(nuevo.email, "CANCELAR") == 0 || strcmp(nuevo.email, "0") == 0)
        return;

    // --- Teléfono ---
    solicitarString("Ingrese teléfono del cliente (o CANCELAR): ",
                    nuevo.telefono, 20);

    if (strcmp(nuevo.telefono, "CANCELAR") == 0 || strcmp(nuevo.telefono, "0") == 0)
        return;

    // --- Dirección ---
    solicitarString("Ingrese dirección del cliente (o CANCELAR): ",
                    nuevo.direccion, 200);

    if (strcmp(nuevo.direccion, "CANCELAR") == 0 || strcmp(nuevo.direccion, "0") == 0)
        return;

    // --- Fecha automática ---
    obtenerFechaActual(nuevo.fechaRegistro);

    // --- Resumen ---
    cout << "\n=== RESUMEN DEL CLIENTE ===\n";
    cout << "Cédula/RIF: " << nuevo.cedula << endl;
    cout << "Nombre: " << nuevo.nombre << endl;
    cout << "Email: " << nuevo.email << endl;
    cout << "Teléfono: " << nuevo.telefono << endl;
    cout << "Dirección: " << nuevo.direccion << endl;

    if (!confirmar("¿Guardar cliente? (S/N): "))
        return;

    // --- Redimensionar si es necesario ---
    if (tienda->numClientes >= tienda->capacidadClientes)
        redimensionarClientes(tienda);

    // --- Guardar ---
    nuevo.id = tienda->siguienteIdCliente++;
    tienda->clientes[tienda->numClientes++] = nuevo;

    cout << "Cliente registrado exitosamente.\n";
}

//2.4.2

void buscarCliente(Tienda* tienda) {
    int opcion;

    do {
        cout << "\n=== BUSCAR CLIENTE ===\n";
        cout << "1. Buscar por ID\n";
        cout << "2. Buscar por nombre (coincidencia parcial)\n";
        cout << "3. Buscar por cédula/RIF\n";
        cout << "0. Cancelar\n";
        cout << "Seleccione una opción: ";
        cin >> opcion;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (opcion == 0)
            return;

        int index = -1;

        switch (opcion) {

            case 1: {
                int id;
                cout << "Ingrese ID del cliente: ";
                cin >> id;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                index = buscarClientePorID(tienda, id);
                break;
            }

            case 2: {
                char nombre[100];
                solicitarString("Ingrese nombre o parte del nombre: ", nombre, 100);
                index = buscarClientePorNombre(tienda, nombre);
                break;
            }

            case 3: {
                char cedula[20];
                solicitarString("Ingrese cédula/RIF del cliente: ", cedula, 20);
                index = buscarClientePorCedula(tienda, cedula);
                break;
            }

            default:
                cout << "Opción inválida.\n";
                continue;
        }

        if (index == -1) {
            cout << "No se encontró ningún cliente con ese criterio.\n";
        } else {
            mostrarCliente(tienda->clientes[index]);
        }

        system("pause");

    } while (opcion != 0);
}

//2.4.3

void actualizarCliente(Tienda* tienda) {
    int id;
    cout << "Ingrese el ID del cliente a actualizar (0 para cancelar): ";
    cin >> id;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (id == 0)
        return;

    int index = buscarClientePorID(tienda, id);
    if (index == -1) {
        cout << "ERROR: No existe un cliente con ese ID.\n";
        return;
    }

    Cliente& original = tienda->clientes[index];
    Cliente temp = original; // copia temporal

    cout << "\n=== CLIENTE ENCONTRADO ===\n";
    mostrarCliente(original);

    if (!confirmar("\n¿Desea modificar este cliente? (S/N): "))
        return;

    // --- Nombre ---
    cout << "\nNombre actual: " << temp.nombre << endl;
    solicitarString("Nuevo nombre (ENTER para mantener): ", temp.nombre, 100);
    if (strlen(temp.nombre) == 0)
        strcpy(temp.nombre, original.nombre);

    // --- Cédula/RIF ---
    cout << "\nCédula/RIF actual: " << temp.cedula << endl;
    solicitarString("Nueva cédula/RIF (ENTER para mantener): ", temp.cedula, 20);
    if (strlen(temp.cedula) == 0)
        strcpy(temp.cedula, original.cedula);
    else if (clienteDuplicado(tienda, temp.cedula, original.id)) {
        cout << "ERROR: La cédula/RIF '" << temp.cedula << "' ya está registrada.\n";
        return;
    }

    // --- Teléfono ---
    cout << "\nTeléfono actual: " << temp.telefono << endl;
    solicitarString("Nuevo teléfono (ENTER para mantener): ", temp.telefono, 20);
    if (strlen(temp.telefono) == 0)
        strcpy(temp.telefono, original.telefono);

    // --- Email ---
    cout << "\nEmail actual: " << temp.email << endl;
    solicitarString("Nuevo email (ENTER para mantener): ", temp.email, 100);
    if (strlen(temp.email) == 0)
        strcpy(temp.email, original.email);
    else if (!emailValido(temp.email)) {
        cout << "ERROR: El email debe contener '@' y un dominio.\n";
        return;
    }

    // --- Dirección ---
    cout << "\nDirección actual: " << temp.direccion << endl;
    solicitarString("Nueva dirección (ENTER para mantener): ", temp.direccion, 200);
    if (strlen(temp.direccion) == 0)
        strcpy(temp.direccion, original.direccion);

    // --- Confirmación final ---
    cout << "\n=== NUEVOS DATOS DEL CLIENTE ===\n";
    mostrarCliente(temp);

    if (!confirmar("¿Guardar cambios? (S/N): "))
        return;

    original = temp;

    cout << "Cliente actualizado exitosamente.\n";
}

//2.4.4

void listarClientes(Tienda* tienda) {
    if (tienda->numClientes == 0) {
        cout << "No hay clientes registrados.\n";
        return;
    }

    encabezadoClientes();

    for (int i = 0; i < tienda->numClientes; i++) {
        filaCliente(tienda->clientes[i]);
    }

    pieClientes();

    cout << "\nTotal de clientes: " << tienda->numClientes << "\n";
}

//2.4.5

void eliminarCliente(Tienda* tienda) {
    int id;
    cout << "Ingrese el ID del cliente a eliminar (0 para cancelar): ";
    cin >> id;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (id == 0)
        return;

    int index = buscarClientePorID(tienda, id);
    if (index == -1) {
        cout << "ERROR: No existe un cliente con ese ID.\n";
        return;
    }

    Cliente& c = tienda->clientes[index];

    cout << "\n=== CLIENTE A ELIMINAR ===\n";
    mostrarCliente(c);

    // Verificar transacciones asociadas
    bool tieneTransacciones = false;
    for (int i = 0; i < tienda->numTransacciones; i++) {
        if (tienda->transacciones[i].idRelacionado == id &&
            strcmp(tienda->transacciones[i].tipo, "VENTA") == 0) {
            tieneTransacciones = true;
            break;
        }
    }

    if (tieneTransacciones) {
        cout << "\nADVERTENCIA: Este cliente tiene transacciones asociadas.\n";
        cout << "No se puede eliminar mientras existan ventas registradas.\n";
        return;
    }

    if (!confirmar("\n¿Eliminar cliente? (S/N): ")) {
        cout << "Eliminación cancelada.\n";
        return;
    }

    // Eliminar moviendo elementos
    for (int i = index; i < tienda->numClientes - 1; i++) {
        tienda->clientes[i] = tienda->clientes[i + 1];
    }

    tienda->numClientes--;

    cout << "Cliente eliminado exitosamente.\n";
}

void menuClientes(Tienda* tienda) {
    int opcion;

    do {
        system("cls");
        cout << "+-------------------------------------------+\n";
        cout << "¦         GESTIÓN DE CLIENTES              ¦\n";
        cout << "+-------------------------------------------+\n\n";

        cout << "1. Registrar cliente\n";
        cout << "2. Buscar cliente\n";
        cout << "3. Actualizar cliente\n";
        cout << "4. Listar clientes\n";
        cout << "5. Eliminar cliente\n";
        cout << "0. Volver al menú principal\n\n";
        cout << "Seleccione una opción: ";

        cin >> opcion;
        cin.ignore();

        system("cls");

        switch (opcion) {
            case 1: crearCliente(tienda); break;
            case 2: buscarCliente(tienda); break;
            case 3: actualizarCliente(tienda); break;
            case 4: listarClientes(tienda); break;
            case 5: eliminarCliente(tienda); break;
            case 0: break;
            default: cout << "Opción inválida.\n";
        }

        if (opcion != 0) system("pause");

    } while (opcion != 0);
}

//==============
// 2.5
//==============

void registrarCompra(Tienda* tienda) {
    cout << "\n=== REGISTRAR COMPRA ===\n";

    int idProd;
    cout << "Ingrese ID del producto (0 para cancelar): ";
    cin >> idProd;
    if (idProd == 0) return;

    if (!existeProducto(tienda, idProd)) {
        cout << "ERROR: El producto no existe.\n";
        return;
    }

    int posProd = buscarProductoPorID(tienda, idProd);

    int idProv;
    cout << "Ingrese ID del proveedor (0 para cancelar): ";
    cin >> idProv;
    if (idProv == 0) return;

    if (!existeProveedor(tienda, idProv)) {
        cout << "ERROR: El proveedor no existe.\n";
        return;
    }

    int posProv = buscarProveedorPorID(tienda, idProv);

    int cantidad = solicitarEnteroPositivo("Cantidad comprada (>0): ");

    float precioUnit;
    cout << "Precio unitario (>0): ";
    cin >> precioUnit;
    if (precioUnit <= 0) {
        cout << "ERROR: El precio debe ser mayor a 0.\n";
        return;
    }

    float total = cantidad * precioUnit;

    cout << "\n=== RESUMEN DE COMPRA ===\n";
    cout << "Producto: " << tienda->productos[posProd].nombre << endl;
    cout << "Proveedor: " << tienda->proveedores[posProv].nombre << endl;
    cout << "Cantidad: " << cantidad << endl;
    cout << "Total: " << total << endl;

    if (!confirmar("¿Confirmar compra? (S/N): ")) return;

    tienda->productos[posProd].stock += cantidad;

    if (tienda->numTransacciones >= tienda->capacidadTransacciones)
        redimensionarTransacciones(tienda);

    Transaccion& t = tienda->transacciones[tienda->numTransacciones++];
    t.id = tienda->siguienteIdTransaccion++;
    strcpy(t.tipo, "COMPRA");
    t.idProducto = idProd;
    t.idRelacionado = idProv;
    t.cantidad = cantidad;
    t.precioUnitario = precioUnit;
    t.total = total;
    obtenerFechaActual(t.fecha);
    strcpy(t.descripcion, "Compra a proveedor");

    cout << "\nCompra registrada exitosamente.\n";
}



// 2.5.2

  void registrarVenta(Tienda* tienda) {
    cout << "\n=== REGISTRAR VENTA ===\n";

    int idProd;
    cout << "Ingrese ID del producto (0 para cancelar): ";
    cin >> idProd;
    if (idProd == 0) return;

    if (!existeProducto(tienda, idProd)) {
        cout << "ERROR: El producto no existe.\n";
        return;
    }

    int posProd = buscarProductoPorID(tienda, idProd);
    Producto& p = tienda->productos[posProd];

    cout << "Stock disponible: " << p.stock << endl;

    int idCli;
    cout << "Ingrese ID del cliente (0 para cancelar): ";
    cin >> idCli;
    if (idCli == 0) return;

    if (!existeCliente(tienda, idCli)) {
        cout << "ERROR: El cliente no existe.\n";
        return;
    }

    int posCli = buscarClientePorID(tienda, idCli);

    int cantidad = solicitarEnteroPositivo("Cantidad vendida (>0): ");

    if (cantidad > p.stock) {
        cout << "ERROR: Stock insuficiente.\n";
        return;
    }

    float precioUnit = p.precio;
    float total = precioUnit * cantidad;

    cout << "\n=== RESUMEN DE VENTA ===\n";
    cout << "Producto: " << p.nombre << endl;
    cout << "Cliente: " << tienda->clientes[posCli].nombre << endl;
    cout << "Cantidad: " << cantidad << endl;
    cout << "Total: " << total << endl;

    if (!confirmar("¿Confirmar venta? (S/N): ")) return;

    p.stock -= cantidad;

    if (tienda->numTransacciones >= tienda->capacidadTransacciones)
        redimensionarTransacciones(tienda);

    Transaccion& t = tienda->transacciones[tienda->numTransacciones++];
    t.id = tienda->siguienteIdTransaccion++;
    strcpy(t.tipo, "VENTA");
    t.idProducto = idProd;
    t.idRelacionado = idCli;
    t.cantidad = cantidad;
    t.precioUnitario = precioUnit;
    t.total = total;
    obtenerFechaActual(t.fecha);
    strcpy(t.descripcion, "Venta a cliente");

    cout << "\nVenta registrada exitosamente.\n";
}



//2.5.3

void buscarTransacciones(Tienda* tienda) {
    cout << "\n=== BUSCAR TRANSACCIONES ===\n";
    cout << "1. Por ID\n";
    cout << "2. Por ID de producto\n";
    cout << "3. Por ID de cliente\n";
    cout << "4. Por ID de proveedor\n";
    cout << "5. Por fecha (YYYY-MM-DD)\n";
    cout << "6. Por tipo (COMPRA/VENTA)\n";
    cout << "0. Cancelar\n";
    cout << "Opción: ";

    int op;
    cin >> op;
    cin.ignore();

    if (op == 0) return;

    char filtro[50];
    int id;

    cout << "\n=== RESULTADOS ===\n";

    for (int i = 0; i < tienda->numTransacciones; i++) {
        Transaccion& t = tienda->transacciones[i];
        bool coincide = false;

        switch (op) {
            case 1:
                cout << "ID: ";
                cin >> id;
                coincide = (t.id == id);
                break;

            case 2:
                cout << "ID Producto: ";
                cin >> id;
                coincide = (t.idProducto == id);
                break;

            case 3:
                cout << "ID Cliente: ";
                cin >> id;
                coincide = (strcmp(t.tipo, "VENTA") == 0 && t.idRelacionado == id);
                break;

            case 4:
                cout << "ID Proveedor: ";
                cin >> id;
                coincide = (strcmp(t.tipo, "COMPRA") == 0 && t.idRelacionado == id);
                break;

            case 5:
                solicitarString("Fecha: ", filtro, 11);
                coincide = (strcmp(t.fecha, filtro) == 0);
                break;

            case 6: {
                solicitarString("Tipo (COMPRA/VENTA): ", filtro, 10);
                string tipoFiltro = toLower(filtro);
                string tipoTrans = toLower(t.tipo);
                coincide = (tipoFiltro == tipoTrans);
                break;
            }
        }

        if (coincide) {
    mostrarDetalleTransaccion(tienda, t);
}

    }
}

//2.5.4

void listarTransacciones(Tienda* tienda) {
    if (tienda->numTransacciones == 0) {
        cout << "No hay transacciones registradas.\n";
        return;
    }

    encabezadoTransacciones();

    for (int i = 0; i < tienda->numTransacciones; i++) {
        filaTransaccion(tienda->transacciones[i]);
    }

    pieTransacciones();

    cout << "\nTotal de transacciones: " << tienda->numTransacciones << "\n";
}



//2.5.5

void cancelarTransaccion(Tienda* tienda) {
    int id;
    cout << "ID de la transacción a cancelar (0 para salir): ";
    cin >> id;

    if (id == 0) return;

    int pos = -1;
    for (int i = 0; i < tienda->numTransacciones; i++)
        if (tienda->transacciones[i].id == id)
            pos = i;

    if (pos == -1) {
        cout << "ERROR: Transacción no encontrada.\n";
        return;
    }

    Transaccion t = tienda->transacciones[pos];

    mostrarDetalleTransaccion(tienda, t);

    if (!confirmar("¿Cancelar transacción? (S/N): ")) return;

    int posProd = buscarProductoPorID(tienda, t.idProducto);

    if (strcmp(t.tipo, "COMPRA") == 0)
        tienda->productos[posProd].stock -= t.cantidad;
    else
        tienda->productos[posProd].stock += t.cantidad;

    for (int i = pos; i < tienda->numTransacciones - 1; i++)
        tienda->transacciones[i] = tienda->transacciones[i + 1];

    tienda->numTransacciones--;

    cout << "Transacción cancelada y stock revertido.\n";
}

void menuTransacciones(Tienda* tienda) {
    int opcion;

    do {
        system("cls");
        cout << "\n=== MENU DE TRANSACCIONES ===\n";
        cout << "1. Registrar compra (Proveedor)\n";
        cout << "2. Registrar venta (Cliente)\n";
        cout << "3. Buscar transacciones\n";
        cout << "4. Listar transacciones\n";
        cout << "5. Cancelar transacción\n";
        cout << "0. Volver al menú principal\n";
        cout << "Seleccione una opción: ";
        cin >> opcion;
        cin.ignore();

        system("cls");

        switch (opcion) {
            case 1: registrarCompra(tienda); break;
            case 2: registrarVenta(tienda); break;
            case 3: buscarTransacciones(tienda); break;
            case 4: listarTransacciones(tienda); break;
            case 5: cancelarTransaccion(tienda); break;

            case 0:
                cout << "Volviendo al menú principal...\n";
                break;

            default:
                cout << "Opción inválida.\n";
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

    Tienda tienda;

    // Inicialización básica
    tienda.numProductos = 0;
    tienda.capacidadProductos = 10;
    tienda.productos = new Producto[tienda.capacidadProductos];
    tienda.siguienteIdProducto = 1;

    tienda.numProveedores = 0;
    tienda.capacidadProveedores = 10;
    tienda.proveedores = new Proveedor[tienda.capacidadProveedores];
    tienda.siguienteIdProveedor = 1;

    tienda.numClientes = 0;
    tienda.capacidadClientes = 10;
    tienda.clientes = new Cliente[tienda.capacidadClientes];
    tienda.siguienteIdCliente = 1;

    tienda.numTransacciones = 0;
    tienda.capacidadTransacciones = 10;
    tienda.transacciones = new Transaccion[tienda.capacidadTransacciones];
    tienda.siguienteIdTransaccion = 1;

    // Cargar datos desde archivos binarios (si existen)
    cargarProductosBinario(&tienda);
    cargarProveedoresBinario(&tienda);
    cargarClientesBinario(&tienda);
    cargarTransaccionesBinario(&tienda);

	
    int opcion;

 do {
    system("cls");
    cout << "+-------------------------------------------+\n";
    cout << "¦   SISTEMA DE GESTIÓN DE INVENTARIO        ¦\n";
    cout << "¦   A&S TECHNOLOGY                          ¦\n";
    cout << "+-------------------------------------------+\n\n";

    cout << "1. Gestión de Productos\n";
    cout << "2. Gestión de Proveedores\n";
    cout << "3. Gestión de Clientes\n";
    cout << "4. Gestión de Transacciones\n";
    cout << "5. Salir\n\n";
    cout << "Seleccione una opción: ";

    cin >> opcion;
    cin.ignore();

    switch (opcion) {
        case 1: menuProductos(&tienda); break;
        case 2: menuProveedores(&tienda); break;
        case 3: menuClientes(&tienda); break;
        case 4: menuTransacciones(&tienda); break;
        case 5: cout << "Saliendo...\n"; break;
        default: cout << "Opción inválida.\n";
    }

    if (opcion != 5) system("pause");

} while (opcion != 5);

    // Guardar datos en archivos binarios antes de salir
    guardarProductosBinario(&tienda);
    guardarProveedoresBinario(&tienda);
    guardarClientesBinario(&tienda);
    guardarTransaccionesBinario(&tienda);

    // (Opcional) liberar memoria si quieres usar liberarTienda:
    // liberarTienda(&tienda);

    return 0;
}




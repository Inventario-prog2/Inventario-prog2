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
class Producto {
private:
    int id;
    char codigo[20];
    char nombre[100];
    char descripcion[200];
    int idProveedor;
    float precio;
    int stock;
    char fechaRegistro[11];
    int stockMinimo;
    int totalVendido;
    bool eliminado;

public:
    // 1. CONSTRUCTOR VACÍO (Para lectura de archivos)
    Producto() {
        id = 0;
        precio = 0.0f;
        stock = 0;
        stockMinimo = 0;
        totalVendido = 0;
        eliminado = false;
        idProveedor = 0;
        memset(codigo, 0, sizeof(codigo));
        memset(nombre, 0, sizeof(nombre));
        memset(descripcion, 0, sizeof(descripcion));
        memset(fechaRegistro, 0, sizeof(fechaRegistro));
    }

    // 2. CONSTRUCTOR PARAMETRIZADO (Para crear nuevos)
    Producto(int _id, const char* _nombre, float _precio, int _stock) {
        // Primero limpiamos todo para que no haya basura
        *this = Producto(); 
        
        id = _id;
        precio = _precio;
        stock = _stock;
        strncpy(nombre, _nombre, sizeof(nombre) - 1);
    }

    // 3. GETTERS
    int getId() const { return id; }
    const char* getCodigo() const { return codigo; } // <--- FALTABA ESTA LÍNEA
    float getPrecio() const { return precio; }
    int getStock() const { return stock; }
    bool isEliminado() const { return eliminado; }
    const char* getNombre() const { return nombre; }
	const char* getDescripcion() const { return descripcion; }
	int getIdProveedor() const { return idProveedor; }
	const char* getFechaRegistro() const { return fechaRegistro; }
	int getStockMinimo() const { return stockMinimo; }
	int getTotalVendido() const { return totalVendido; }
    // 4. SETTERS
    void setPrecio(float _precio) { if(_precio >= 0) precio = _precio; }
    void setStock(int _stock) { if(_stock >= 0) stock = _stock; }
    void setEliminado(bool _estado) { eliminado = _estado; }
    void setId(int _id) { id = _id; }
    void setCodigo(const char* _cod) { strncpy(codigo, _cod, sizeof(codigo) - 1); codigo[sizeof(codigo)-1] = '\0'; }
    void setNombre(const char* _nom) { strncpy(nombre, _nom, sizeof(nombre) - 1); nombre[sizeof(nombre)-1] = '\0'; }
    void setDescripcion(const char* _desc) { strncpy(descripcion, _desc, sizeof(descripcion) - 1); descripcion[sizeof(descripcion)-1] = '\0'; }
    void setIdProveedor(int _idProv) { idProveedor = _idProv; }
    void setFechaRegistro(const char* _fec) { strncpy(fechaRegistro, _fec, sizeof(fechaRegistro) - 1); }
};
//1.2 Estructura Proveedor

class Proveedor {
private:
    int id;
    char nombre[100];
    char rif[20];
    char telefono[20];
    char email[100];
    char direccion[200];
    char fechaRegistro[11];
    bool eliminado;

public:
    // 1. Constructor
    Proveedor() {
        id = 0;
        eliminado = false;
        memset(nombre, 0, sizeof(nombre));
        memset(rif, 0, sizeof(rif));
        memset(telefono, 0, sizeof(telefono));
        memset(email, 0, sizeof(email));
        memset(direccion, 0, sizeof(direccion));
        memset(fechaRegistro, 0, sizeof(fechaRegistro));
    }

    // 2. Constructor parametrizado
    Proveedor(int _id, const char* _nombre, const char* _rif) {
        *this = Proveedor(); // Inicializa todo en limpio primero
        id = _id;
        strncpy(nombre, _nombre, sizeof(nombre) - 1);
        strncpy(rif, _rif, sizeof(rif) - 1);
    }

    // 3. Getters
    int getId() const { return id; }
    const char* getNombre() const { return nombre; }
    const char* getRif() const { return rif; }
    bool isEliminado() const { return eliminado; }
    const char* getTelefono() const { return telefono; }
    const char* getEmail() const { return email; }
    const char* getDireccion() const { return direccion; }
    const char* getFechaRegistro() const { return fechaRegistro; }
    
    // 4. Setters
    void setNombre(const char* _nombre) { strncpy(nombre, _nombre, sizeof(nombre) - 1); }
    void setEliminado(bool _estado) { eliminado = _estado; }
    void setId(int _id) { id = _id; }
    void setRif(const char* _r) { strncpy(rif, _r, 19)[19] = '\0'; }
    void setEmail(const char* _e) { strncpy(email, _e, 99)[99] = '\0'; }
    void setTelefono(const char* _t) { strncpy(telefono, _t, 19)[19] = '\0'; }
};

//1.3 cliente
class Cliente {
private:
    int id;
    char nombre[100];
    char cedula[20];
    char telefono[20];
    char email[100];
    char direccion[200];
    char fechaRegistro[11];
    bool eliminado;

public:
    // 1. Constructor
    Cliente() {
        id = 0;
        eliminado = false;
        // Limpiamos TODA la basura de la RAM
        memset(nombre, 0, sizeof(nombre));
        memset(cedula, 0, sizeof(cedula));
        memset(telefono, 0, sizeof(telefono));
        memset(email, 0, sizeof(email));
        memset(direccion, 0, sizeof(direccion));
        memset(fechaRegistro, 0, sizeof(fechaRegistro));
    }

    // 2. Constructor parametrizado
    Cliente(int _id, const char* _nombre, const char* _cedula) {
        // Llamamos al constructor por defecto para que limpie todo primero
        *this = Cliente(); 
        
        id = _id;
        // Usamos strncpy para no desbordar el arreglo
        strncpy(nombre, _nombre, sizeof(nombre) - 1);
        strncpy(cedula, _cedula, sizeof(cedula) - 1);
    }

    // 3. Getters
    int getId() const { return id; }
    const char* getNombre() const { return nombre; }
    const char* getCedula() const { return cedula; }
    bool isEliminado() const { return eliminado; }
    const char* getTelefono() const { return telefono; }
    const char* getEmail() const { return email; }
    const char* getDireccion() const { return direccion; }
    const char* getFechaRegistro() const { return fechaRegistro; }

    // 4. Setters
    void setNombre(const char* _nombre) { strncpy(nombre, _nombre, sizeof(nombre) - 1); }
    void setEliminado(bool _estado) { eliminado = _estado; }
    void setFecha(const char* _f) { strncpy(fechaRegistro, _f, sizeof(fechaRegistro)-1); }
    void setId(int _id) { id = _id; }
    // --- SETTERS COMPACTOS PARA CLIENTE ---
    void setCedula(const char* _c) { strncpy(cedula, _c, 19)[19] = '\0'; }
    void setEmail(const char* _e) { strncpy(email, _e, 99)[99] = '\0'; }
    void setTelefono(const char* _t) { strncpy(telefono, _t, 19)[19] = '\0'; }
    void setDireccion(const char* _d) { strncpy(direccion, _d, 199)[199] = '\0'; }
};

//1.4 Estructura TransacciÃ³n 

class Transaccion {
private:
    int id;
    char tipo[10];           // "COMPRA" o "VENTA"
    int idRelacionado;       // ID del proveedor o cliente
    
    int idProductos[20];    
    int cantidades[20];      
    float precioUnitario[20]; 
    int numItems;            // ¡Controla el llenado de los arreglos!
    
    float total;
    char fechaRegistro[11];
    char descripcion[200];
    bool eliminado;

public:
    // 1. Constructor por defecto (Limpieza total)
    Transaccion() {
        id = 0;
        idRelacionado = 0;
        numItems = 0; // SUPER IMPORTANTE: Empezamos en el índice 0
        total = 0.0f;
        eliminado = false;

        // Limpieza de cadenas
        memset(tipo, 0, sizeof(tipo));
        memset(fechaRegistro, 0, sizeof(fechaRegistro));
        memset(descripcion, 0, sizeof(descripcion));

        // Limpieza de arreglos paralelos
        for (int i = 0; i < 20; i++) {
            idProductos[i] = 0;
            cantidades[i] = 0;
            precioUnitario[i] = 0.0f;
        }
    }

    // 2. Método para agregar productos (Lógica de negocio)
    // No puedes acceder a los arreglos directamente desde fuera, así que usas esto:
    bool agregarDetalle(int _idProd, int _cant, float _precio) {
        if (numItems < 20) {
            idProductos[numItems] = _idProd;
            cantidades[numItems] = _cant;
            precioUnitario[numItems] = _precio;
            
            // Actualizamos el total de la transacción automáticamente
            total += (_cant * _precio);
            
            numItems++; // Avanzamos al siguiente espacio
            return true;
        }
        return false; // Error: Factura llena
    }

    // 3. Getters básicos
    int getId() const { return id; }
    int getIdRelacionado() const { return idRelacionado;}
    float getTotal() const { return total; }
    int getNumItems() const { return numItems; }
    const char* getTipo() const { return tipo; }
    int getIdProducto(int indice) const { return idProductos[indice]; }
    int getCantidad(int index) const { return cantidades[index]; }
    const char* getFecha() const { return fechaRegistro; }
    bool isEliminado() const { return eliminado; }
      
    // Setters necesarios
    void setId(int _id) { id = _id; }
    void setTipo(const char* _tipo) { strncpy(tipo, _tipo, sizeof(tipo) - 1); }
    void setIdRelacionado(int _idRel) { idRelacionado = _idRel; }
    void setEliminado(bool _e) { eliminado = _e; }
    void setFecha(const char* _f) { strncpy(fechaRegistro, _f, sizeof(fechaRegistro)-1); }
    void setTotal(float _t) { total = (_t >= 0) ? _t : 0; }
    void setDescripcion(const char* _desc) { strncpy(descripcion, _desc, 99); descripcion[99] = '\0'; }
    void setFechaRegistro(const char* _fecha) { strncpy(fechaRegistro, _fecha, 10); fechaRegistro[10] = '\0'; }
    void setIdProductoEnIndice(int index, int _idProd) {
        if (index >= 0 && index < 20) idProductos[index] = _idProd;
    }
    void setCantidad(int index, int _cant) {
        if (index >= 0 && index < 20) cantidades[index] = _cant;
    }
    void setPrecioUnitario(int index, float _precio) {
        if (index >= 0 && index < 20) precioUnitario[index] = _precio;
    }
    void setNumItems(int _n) { numItems = (_n >= 0 && _n <= 20) ? _n : 0; }
};

//1.5 Estructura Principal: Tienda

class Tienda {
private:
    char nombre[100];
    char rif[20];
    char direccion[200];
    char telefono[20];
    
    float totalVentasHistoricas;
    float totalComprasHistoricas;
    int cantidadOperaciones;

public:
    // Constructor: Inicializa la tienda con valores en cero o por defecto
    Tienda() {
        memset(nombre, 0, sizeof(nombre));
        memset(rif, 0, sizeof(rif));
        memset(direccion, 0, sizeof(direccion));
        memset(telefono, 0, sizeof(telefono));
        
        totalVentasHistoricas = 0.0f;
        totalComprasHistoricas = 0.0f;
        cantidadOperaciones = 0;
    }

    // Constructor con datos iniciales
    Tienda(const char* _nom, const char* _rif) {
        *this = Tienda(); // Limpia todo primero
        strncpy(nombre, _nom, sizeof(nombre) - 1);
        strncpy(rif, _rif, sizeof(rif) - 1);
    }

    // Métodos para actualizar estadísticas (Lógica de negocio)
    void registrarVenta(float monto) {
        totalVentasHistoricas += monto;
        cantidadOperaciones++;
    }

    void registrarCompra(float monto) {
        totalComprasHistoricas += monto;
        cantidadOperaciones++;
    }

    // Getters para los reportes
    const char* getNombre() const { return nombre; }
    float getTotalVentas() const { return totalVentasHistoricas; }
    float getTotalCompras() const { return totalComprasHistoricas; }
    int getCantOperaciones() const { return cantidadOperaciones; }
    
    void setNombre(const char* n) { strcpy(nombre, n); }
	void setRif(const char* r) { strcpy(rif, r); }
	void setTotalVentas(float v) { totalVentasHistoricas = v; }
	void setTotalCompras(float c) { totalComprasHistoricas = c; }
	void setCantidadOperaciones(int o) { cantidadOperaciones = o; }
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
    string f = ""; 
    // Convertir también el filtro a minúsculas para que coincidan
    for(char c : filtro) f += tolower(c); 
    return a.find(f) != string::npos;
}

bool codigoProductoDuplicado(const char* codigo, int idIgnorar = -1) {
    FILE* f = fopen("productos.bin", "rb");
    if (!f) return false;

    ArchivoHeader h;
    fread(&h, sizeof(ArchivoHeader), 1, f);

    Producto p;
    // Recorremos los registros en el archivo
    while (fread(&p, sizeof(Producto), 1, f)) {
        // 1. Verificamos que no esté eliminado
        // 2. Verificamos que no sea el mismo ID que estamos editando (idIgnorar)
        if (!p.isEliminado() && p.getId() != idIgnorar) {
            // 3. Comparamos los códigos de texto
            if (strcmp(p.getCodigo(), codigo) == 0) {
                fclose(f);
                return true; 
            }
        }
    }

    fclose(f);
    return false; 
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
        // Cambio necesario para cuando Proveedor sea clase:
		if (!p.isEliminado() && p.getId() == idBuscado) {
 	   strcpy(destino, p.getNombre());
    	break;
		}
    }
    fclose(f);
}

bool sinEspacios(const char* texto) {
    if (texto == nullptr) return false;

    for (int i = 0; texto[i] != '\0'; i++) {
        if (isspace(texto[i])) // Verifica espacios, tabs o saltos de línea
            return false;
    }
    return true;
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

bool emailValido(const char* email) {
    if (!sinEspacios(email)) return false; // Un email no puede tener espacios
    const char* at = strchr(email, '@');
    if (!at || at == email) return false; // Debe haber algo antes del @
    const char* punto = strchr(at, '.');
    return punto && *(punto + 1) != '\0'; // Debe haber algo después del punto
}

bool rifDuplicado(const char* rif, int idIgnorar = -1) {
    FILE* f = fopen("proveedores.bin", "rb");
    if (!f) return false;

    ArchivoHeader h;
    fread(&h, sizeof(ArchivoHeader), 1, f);

    Proveedor p;
    while (fread(&p, sizeof(Proveedor), 1, f)) {
        // Usamos getters: isEliminado() y getId()
        if (!p.isEliminado() && p.getId() != idIgnorar) {
            // Nota: Si 'rif' en la clase es privado, asegúrate de tener un getRif()
            if (strcmp(p.getRif(), rif) == 0) { 
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
        // Usamos getters: isEliminado() y getId()
        if (!c.isEliminado() && c.getId() != idIgnorar) {
            // Asegúrate de tener getCedula() en tu clase Cliente
            if (strcmp(c.getCedula(), cedula) == 0) { 
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
    while (true) {
        cout << mensaje;
        if (cin >> valor && valor > 0) {
            cin.ignore(1000, '\n');
            return valor;
        } else {
            cout << "ERROR: Entrada no valida. Ingrese un numero mayor a 0.\n";
            cin.clear(); // Limpia el error de cin
            cin.ignore(1000, '\n'); // Descarta lo que el usuario escribió mal
        }
    }
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
        // CAMBIO: Usar p.isEliminado() y p.getId()
        if (!p.isEliminado() && p.getId() == idBuscado) {
            if (resultado != nullptr) {
                *resultado = p; 
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
    cout << "ID: " << p.getId() << endl; // ¡No olvides el ID!
    cout << "Codigo: " << p.getCodigo() << endl;
    cout << "Nombre: " << p.getNombre() << endl;
    cout << "Descripcion: " << p.getDescripcion() << endl;
    cout << "Proveedor ID: " << p.getIdProveedor() << endl; 
    cout << "Precio: " << p.getPrecio() << endl;
    cout << "Stock: " << p.getStock() << endl;
    cout << "Fecha: " << p.getFechaRegistro() << endl; 
}

void obtenerNombreProductoBin(int idBuscado, char* destino) {
    Producto p;
    if (buscarProductoPorID(idBuscado, &p)) {
        strcpy(destino, p.getNombre());
    } else {
        strcpy(destino, "Desconocido");
    }
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
    // Usamos los Getters para acceder a los datos privados
    cout << "¦ "
         << setw(2)  << left << p.getId() << " ¦ "
         << setw(9)  << left << p.getCodigo() << " ¦ "
         << setw(16) << left << p.getNombre() << " ¦ "
         << setw(12) << left << nombreProv << " ¦ "
         << setw(5)  << left << p.getPrecio() << " ¦ "
         << setw(6)  << left << p.getStock() << " ¦ "
         << setw(5)  << left << p.getFechaRegistro() << " ¦\n";
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
      	if (!p.isEliminado() && contiene(p.getNombre(), filtroMin.c_str())) { 
   			char provNombre[50];
    		obtenerNombreProveedorBin(p.getIdProveedor(), provNombre);
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

    while (fread(&t, sizeof(Transaccion), 1, f)) {
        // Usamos los getters para no violar la privacidad
        for (int i = 0; i < t.getNumItems(); i++) {
            if (t.getIdProducto(i) == idBuscado) {
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
        if (!p.isEliminado() && p.getId() == idBuscado) {
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
        if (!p.isEliminado() && strcmp(p.getRif(), rifBuscado) == 0) {
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
    cout << "ID: " << p.getId() << endl;
	cout << "RIF: " << p.getRif() << endl;
	cout << "Nombre: " << p.getNombre() << endl;
    cout << "Email: " << p.getEmail() << endl; 
    cout << "Telefono: " << p.getTelefono() << endl;
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
    cout << "¦ "
         << setw(2)  << left << p.getId() << " ¦ "
         << setw(20) << left << p.getNombre() << " ¦ "
         << setw(16) << left << p.getRif() << " ¦ "
         << setw(25) << left << p.getTelefono() << " ¦\n";
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
        if (!p.isEliminado() && contiene(p.getNombre(), filtroMin.c_str())) {
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
        // CAMBIO: Usamos los getters
        if (!c.isEliminado() && c.getId() == idBuscado) {
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

    fseek(f, sizeof(ArchivoHeader), SEEK_SET);

    Cliente c;
    while (fread(&c, sizeof(Cliente), 1, f)) {
        // CAMBIO: Usamos getCedula() e isEliminado()
        if (!c.isEliminado() && strcmp(c.getCedula(), cedulaBuscada) == 0) {
            if (resultado) *resultado = c;
            fclose(f);
            return true;
        }
    }
    fclose(f);
    return false;
}

void obtenerNombreClienteBin(int idBuscado, char* destino) {
    strcpy(destino, "Desconocido");
    Cliente c;
    // Reutilizamos la función de búsqueda para evitar repetir el código de archivos
    if (buscarClientePorIDBin(idBuscado, &c)) {
        strcpy(destino, c.getNombre());
    }
}

void mostrarCliente(const Cliente& c) {
    cout << "\n=== CLIENTE ENCONTRADO ===\n";
    cout << "ID: " << c.getId() << endl;
    cout << "Cedula/RIF: " << c.getCedula() << endl;
    cout << "Nombre: " << c.getNombre() << endl;
    cout << "Email: " << c.getEmail() << endl;
    cout << "Telefono: " << c.getTelefono() << endl;
    cout << "Direccion: " << c.getDireccion() << endl;
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
    cout << "¦ "
         << setw(2)  << left << c.getId() << " ¦ "
         << setw(20) << left << c.getNombre() << " ¦ "
         << setw(16) << left << c.getCedula() << " ¦ "
         << setw(20) << left << c.getTelefono() << " ¦ "
         << setw(20) << left << c.getEmail() << " ¦\n";
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
        // Ajuste: isEliminado() y getNombre()
        if (!c.isEliminado() && contiene(c.getNombre(), filtroMin.c_str())) {
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
        if (t.getIdRelacionado() == idCliente && strcmp(t.getTipo(), "VENTA") == 0) {
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
    cout << "¦ "
         << setw(2)  << left << t.getId() << " ¦ "
         << setw(8)  << left << t.getTipo() << " ¦ ";
    if (t.getNumItems() > 0) {
        cout << setw(8) << left << t.getIdProducto(0) << " ¦ ";
    } else {
        cout << setw(8) << left << "N/A" << " ¦ ";
    }

    cout << setw(22) << left << t.getIdRelacionado() << " ¦ "
         << setw(8)  << left << (t.getNumItems() > 0 ? t.getCantidad(0) : 0) << " ¦ " // Mostramos la primera cantidad
         << setw(9)  << left << t.getTotal() << " ¦ "
         << setw(10) << left << t.getFecha() << " ¦ "
         << setw(15) << left << "Ver detalle" << " ¦\n"; // La descripción suele ser larga, mejor un resumen
}

void pieTransacciones() {
    lineaTrans("bot");
}

//===============
//2.1 inicializar
//===============

void inicializarTienda(Tienda* tienda, const char* nombre, const char* rif) {
    tienda->setNombre(nombre);
    tienda->setRif(rif);
    
    tienda->setTotalVentas(0);
    tienda->setTotalCompras(0);
    tienda->setCantidadOperaciones(0);

    // Inicialización de archivos usando la plantilla
    inicializarArchivo<Producto>("productos.bin");
    inicializarArchivo<Proveedor>("proveedores.bin");
    inicializarArchivo<Cliente>("clientes.bin");
    inicializarArchivo<Transaccion>("transacciones.bin");
}

void registrarProductoEnDisco(Producto nuevo) {
    const char* nombreArchivo = "productos.bin";
    ArchivoHeader header = leerHeader(nombreArchivo);

    // CAMBIO: Usar Setters
    nuevo.setId(header.proximoID);
    nuevo.setEliminado(false); 

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

    nueva.setId(header.proximoID);
    nueva.setEliminado(false);
    
    char fecha[20];
    obtenerFechaActual(fecha);
    nueva.setFecha(fecha); // CAMBIO: Usar Setter para la fecha

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
    cout << "| ID: " << setw(54) << left << t.getId() << "|\n";
    cout << "| Tipo: " << setw(52) << left << t.getTipo() << "|\n";
    cout << "| Items: " << setw(51) << left << t.getNumItems() << "|\n";
    
    for(int i = 0; i < t.getNumItems(); i++) {
        char nombreP[100];
        // Accedemos al ID del producto mediante el getter por índice
        obtenerNombreProductoBin(t.getIdProducto(i), nombreP);
        cout << "|  - " << setw(20) << left << nombreP 
             << " Cant: " << setw(25) << left << t.getCantidad(i) << "|\n";
    }

    cout << "| Total:    $" << setw(47) << left << fixed << setprecision(2) << t.getTotal() << "|\n";
    cout << "| Fecha:    " << setw(48) << left << t.getFecha() << "|\n";
    cout << "+-----------------------------------------------------------+\n";
}


void crearProducto() {
    if (!confirmar("¿Desea registrar un nuevo producto? (S/N): ")) return;

    Producto nuevo;
    char tempStr[200]; // Buffer temporal
    int tempInt;
    float tempFloat;

    // --- Código ---
    solicitarString("Ingrese código del producto (o CANCELAR): ", tempStr, 20, sinEspacios);
    if (strcmp(tempStr, "CANCELAR") == 0 || strcmp(tempStr, "0") == 0) return;

    if (codigoProductoDuplicado(tempStr)) { 
        cout << "ERROR: El código '" << tempStr << "' ya está registrado.\n";
        return;
    }
    nuevo.setCodigo(tempStr); // Usar Setter

    // --- Nombre y Descripción ---
    solicitarString("Ingrese nombre: ", tempStr, 100);
    nuevo.setNombre(tempStr);
    
    solicitarString("Ingrese descripción: ", tempStr, 200);
    nuevo.setDescripcion(tempStr);

    // --- ID Proveedor ---
    cout << "Ingrese ID del proveedor (o 0 para cancelar): ";
    cin >> tempInt; cin.ignore();
    if (tempInt == 0) return;

    if (!existeProveedorBin(tempInt)) {
        cout << "ERROR: El proveedor con ID " << tempInt << " no existe.\n";
        return;
    }
    nuevo.setIdProveedor(tempInt);

    // --- Precio y Stock ---
    cout << "Ingrese precio: "; cin >> tempFloat;
    nuevo.setPrecio(tempFloat);
    
    cout << "Ingrese stock: ";  cin >> tempInt; cin.ignore();
    nuevo.setStock(tempInt);

    // --- Fecha y Borrado Lógico ---
    char fecha[20];
    obtenerFechaActual(fecha);
    nuevo.setFechaRegistro(fecha);
    nuevo.setEliminado(false); 

    // --- Resumen y Guardado ---
    cout << "\n=== RESUMEN DEL PRODUCTO ===\n";
    mostrarProducto(nuevo);

    if (confirmar("¿Guardar producto en disco? (S/N): ")) {
        registrarProductoEnDisco(nuevo); 
        cout << "\nProducto guardado físicamente en productos.bin\n";
    }
}

//========================
//2.2.2
//========================

void buscarProducto() { 
    int opcion;
    cout << "\n=== BUSCAR PRODUCTO EN DISCO ===\n";
    cout << "1. Buscar por ID\n";
    cout << "2. Buscar por nombre (parcial)\n";
    cout << "3. Buscar por codigo (parcial)\n";
    cout << "0. Cancelar\n";
    cout << "Seleccione una opcion: ";
    cin >> opcion;
    cin.ignore(); 

    if (opcion == 0) return;

    FILE* f = fopen("productos.bin", "rb");
    if (!f) {
        cout << "ERROR: No se pudo abrir el archivo de productos.\n";
        return;
    }

    ArchivoHeader h;
    fread(&h, sizeof(ArchivoHeader), 1, f);

    Producto p;
    bool encontrado = false;

    switch (opcion) {
        case 1: { // Buscar por ID
            int idBuscado;
            cout << "Ingrese ID del producto: ";
            cin >> idBuscado;

            while (fread(&p, sizeof(Producto), 1, f)) {
                // AJUSTE: Usar isEliminado() y getId()
                if (!p.isEliminado() && p.getId() == idBuscado) {
                    cout << "\n=== PRODUCTO ENCONTRADO ===\n";
                    mostrarProducto(p);
                    encontrado = true;
                    break; 
                }
            }
            break;
        }

        case 2: { // Buscar por nombre (parcial)
            char buffer[100];
            solicitarString("Ingrese parte del nombre: ", buffer, 100);
            string filtro = toLower(buffer);

            cout << "\n=== RESULTADOS POR NOMBRE ===\n";
            while (fread(&p, sizeof(Producto), 1, f)) {
                // AJUSTE: Usar getNombre() y pasar filtro.c_str() si contiene espera char*
                if (!p.isEliminado() && contiene(p.getNombre(), filtro.c_str())) {
                    mostrarProducto(p);
                    cout << "-----------------------------\n";
                    encontrado = true;
                }
            }
            break;
        }

        case 3: { // Buscar por codigo (parcial)
            char buffer[20];
            solicitarString("Ingrese parte del codigo: ", buffer, 20);
            string filtro = toLower(buffer);

            cout << "\n=== RESULTADOS POR CODIGO ===\n";
            while (fread(&p, sizeof(Producto), 1, f)) {
                // AJUSTE: Usar getCodigo()
                if (!p.isEliminado() && contiene(p.getCodigo(), filtro.c_str())) {
                    mostrarProducto(p);
                    cout << "-----------------------------\n";
                    encontrado = true;
                }
            }
            break;
        }

        default:
            cout << "Opcion invalida.\n";
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

    FILE* f = fopen("productos.bin", "r+b"); // r+b para leer y escribir
    if (!f) {
        cout << "ERROR: No se pudo abrir el archivo.\n";
        return;
    }

    ArchivoHeader h;
    fread(&h, sizeof(ArchivoHeader), 1, f);

    Producto temp;
    long posicionRegistro = -1;
    bool encontrado = false;

    // Busqueda y captura de posicion
    for (int i = 0; i < h.cantidadRegistros; i++) {
        posicionRegistro = ftell(f); 
        if (fread(&temp, sizeof(Producto), 1, f)) {
            // AJUSTE: Usar isEliminado() y getId()
            if (!temp.isEliminado() && temp.getId() == idBuscado) {
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
    char buffer[200]; // Buffer para strings
    do {
        cout << "\n=== EDITAR PRODUCTO (EN DISCO) ===\n";
        // AJUSTE: Usar Getters para mostrar
        cout << "1. Codigo: " << temp.getCodigo() << endl;
        cout << "2. Nombre: " << temp.getNombre() << endl;
        cout << "3. Descripcion: " << temp.getDescripcion() << endl;
        cout << "4. Precio: " << temp.getPrecio() << endl;
        cout << "5. Stock: " << temp.getStock() << endl;
        cout << "6. Guardar cambios en archivo\n";
        cout << "0. Salir sin guardar\n";
        cout << "Opcion: ";
        cin >> opcion;
        cin.ignore();

        switch (opcion) {
            case 1: {
                solicitarString("Nuevo codigo: ", buffer, 20, sinEspacios);
                if (codigoProductoDuplicado(buffer, temp.getId())) {
                    cout << "ERROR: Codigo ya existe.\n";
                } else {
                    temp.setCodigo(buffer); // AJUSTE: Usar Setter
                }
                break;
            }
            case 2: 
                solicitarString("Nuevo nombre: ", buffer, 100);
                temp.setNombre(buffer);
                break;
            case 3: 
                solicitarString("Nueva descripcion: ", buffer, 200);
                temp.setDescripcion(buffer);
                break;
            case 4: {
                float nPrecio;
                cout << "Nuevo precio: "; cin >> nPrecio; cin.ignore();
                temp.setPrecio(nPrecio);
                break;
            }
            case 5: {
                int nStock;
                cout << "Nuevo stock: "; cin >> nStock; cin.ignore();
                temp.setStock(nStock);
                break;
            }
            case 6: {
                if (confirmar("¿Sobrescribir datos en disco? (S/N): ")) {
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
        // Calculamos la posición donde empieza este registro
        pos = ftell(f) - sizeof(Producto); 
        
        // AJUSTE: Usar getId() e isEliminado()
        if (p.getId() == idProd && !p.isEliminado()) {
            
            // AJUSTE: Usar getStock() y setStock() para modificar
            int nuevoStock = p.getStock() + cambioCantidad;
            
            // Validación de seguridad: no permitir stock negativo
            if (nuevoStock < 0) {
                fclose(f);
                return false; 
            }
            
            p.setStock(nuevoStock);
            
            fseek(f, pos, SEEK_SET);   // Volvemos al inicio del registro
            fwrite(&p, sizeof(Producto), 1, f); // Sobreescribimos solo este producto
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
        cout << "No hay productos registrados.\n";
        return;
    }

    ArchivoHeader h;
    fread(&h, sizeof(ArchivoHeader), 1, f);

    encabezadoProductos();

    Producto p;
    char nombreProv[20];

    while (fread(&p, sizeof(Producto), 1, f)) {
        if (!p.isEliminado()) {
            // Buscamos el nombre del proveedor en su propio archivo binario
            obtenerNombreProveedorBin(p.getIdProveedor(), nombreProv);
            filaProducto(p, nombreProv);
        }
    }

    pieProductos();
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

    Producto p;
    long posicionRegistro = -1; 
    
    FILE* f = fopen("productos.bin", "r+b");
    if (!f) {
        cout << "ERROR: No se pudo abrir el archivo de productos.\n";
        return;
    }

    ArchivoHeader h;
    fread(&h, sizeof(ArchivoHeader), 1, f);

    bool encontrado = false;
    for (int i = 0; i < h.cantidadRegistros; i++) {
        posicionRegistro = ftell(f); 
        if (fread(&p, sizeof(Producto), 1, f)) {
            // AJUSTE: Usar isEliminado() y getId()
            if (!p.isEliminado() && p.getId() == idBuscado) {
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

    cout << "\n=== PRODUCTO A ELIMINAR ===\n";
    mostrarProducto(p);

    if (productoTieneTransaccionesBin(idBuscado)) {
        cout << "\nADVERTENCIA: Este producto tiene movimientos en el inventario.\n";
    }

    if (!confirmar("\n¿Esta seguro de marcar este producto como ELIMINADO? (S/N): ")) {
        cout << "Operacion cancelada.\n";
        fclose(f);
        return;
    }

    // AJUSTE: El borrado lógico ahora usa el Setter
    p.setEliminado(true);

    fseek(f, posicionRegistro, SEEK_SET);
    fwrite(&p, sizeof(Producto), 1, f);
    fclose(f);

    // Actualizar el conteo de registros activos en la cabecera
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
   					 cout << "=== AJUSTE MANUAL DE STOCK ===\n";
   					 cout << "ID Producto: "; cin >> id;
   					 cout << "Cantidad a sumar (positivo) o restar (negativo): "; cin >> cant;
    
   					 if (actualizarStockProducto(id, cant)) {
    				    cout << "\n[OK] Stock actualizado correctamente en disco.\n";
    					} else {
     					   cout << "\n[ERROR] No se pudo encontrar el producto o el stock resultaria negativo.\n";
   						}
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

    // AJUSTE: Usar Setter para el ID
    nuevo.setId(header.proximoID); 

    FILE* f = fopen(nombreArchivo, "ab");
    if (f) {
        fwrite(&nuevo, sizeof(Proveedor), 1, f);
        fclose(f);

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
    char buffer[100]; // Buffer temporal para los strings

    // --- RIF ---
    solicitarString("Ingrese RIF del proveedor (o CANCELAR): ", buffer, 20);

    if (strcmp(buffer, "CANCELAR") == 0 || strcmp(buffer, "0") == 0)
        return;

    if (rifDuplicado(buffer)) {
        cout << "ERROR: El RIF '" << buffer << "' ya esta registrado.\n";
        return;
    }
    nuevo.setRif(buffer); // AJUSTE: Usar Setter

    // --- Nombre, Email y Telefono ---
    solicitarString("Ingrese nombre: ", buffer, 100);
    nuevo.setNombre(buffer);

    solicitarString("Ingrese email: ", buffer, 100, emailValido);
    nuevo.setEmail(buffer);

    solicitarString("Ingrese telefono: ", buffer, 20);
    nuevo.setTelefono(buffer);

    // --- Campos de control ---
    nuevo.setEliminado(false); 

    // --- Resumen ---
    cout << "\n=== RESUMEN DEL PROVEEDOR ===\n";
    // Usamos Getters para mostrar el resumen
    cout << "RIF: " << nuevo.getRif() << endl;
    cout << "Nombre: " << nuevo.getNombre() << endl;
    cout << "Email: " << nuevo.getEmail() << endl;
    cout << "Telefono: " << nuevo.getTelefono() << endl;

    if (confirmar("¿Guardar proveedor en disco? (S/N): ")) {
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

    for (int i = 0; i < h.cantidadRegistros; i++) {
        posicionRegistro = ftell(f); 
        if (fread(&temp, sizeof(Proveedor), 1, f)) {
            // AJUSTE: Usar isEliminado() y getId()
            if (!temp.isEliminado() && temp.getId() == idBuscado) {
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

    Proveedor original = temp;

    cout << "\n=== PROVEEDOR ENCONTRADO ===\n";
    mostrarProveedor(original);

    if (!confirmar("\n¿Desea modificar este proveedor? (S/N): ")) {
        fclose(f);
        return;
    }

    char buffer[200];

    // --- Edición de campos ---
    cout << "\nNombre actual: " << original.getNombre() << endl;
    solicitarString("Nuevo nombre (ENTER para mantener): ", buffer, 100);
    if (strlen(buffer) > 0) temp.setNombre(buffer);

    cout << "\nRIF actual: " << original.getRif() << endl;
    solicitarString("Nuevo RIF (ENTER para mantener): ", buffer, 20);
    if (strlen(buffer) > 0) {
        if (rifDuplicado(buffer, original.getId())) {
            cout << "ERROR: El RIF ya existe en otro proveedor.\n";
            fclose(f); return;
        }
        temp.setRif(buffer);
    }

    cout << "\nTelefono actual: " << original.getTelefono() << endl;
    solicitarString("Nuevo telefono (ENTER para mantener): ", buffer, 20);
    if (strlen(buffer) > 0) temp.setTelefono(buffer);

    cout << "\nEmail actual: " << original.getEmail() << endl;
    solicitarString("Nuevo email (ENTER para mantener): ", buffer, 100);
    if (strlen(buffer) > 0) {
        if (!emailValido(buffer)) {
            cout << "ERROR: Email invalido.\n";
            fclose(f); return;
        }
        temp.setEmail(buffer);
    }

    cout << "\n=== NUEVOS DATOS DEL PROVEEDOR ===\n";
    mostrarProveedor(temp);

    if (confirmar("¿Guardar cambios en disco? (S/N): ")) {
        fseek(f, posicionRegistro, SEEK_SET); 
        fwrite(&temp, sizeof(Proveedor), 1, f);
        cout << "Proveedor actualizado exitosamente.\n";
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

    ArchivoHeader h;
    fread(&h, sizeof(ArchivoHeader), 1, f);

    if (h.cantidadRegistros == 0) {
        cout << "El archivo de proveedores esta vacio.\n";
        fclose(f);
        return;
    }

    encabezadoProveedores(); 

    Proveedor p;
    int mostrados = 0;

    while (fread(&p, sizeof(Proveedor), 1, f)) {
        // AJUSTE: Usar el getter isEliminado()
        if (!p.isEliminado()) {
            filaProveedor(p); 
            mostrados++;
        }
    }

    pieProveedores();

    cout << "\nTotal registros en disco: " << h.cantidadRegistros;
    cout << "\nProveedores activos: " << mostrados << "\n";

    fclose(f);
}

//2.3.5

bool proveedorTieneProductosBin(int idProv) {
    FILE* f = fopen("productos.bin", "rb");
    if (!f) return false;

    Producto p;
    // Saltamos el header de productos
    fseek(f, sizeof(ArchivoHeader), SEEK_SET);

    while (fread(&p, sizeof(Producto), 1, f)) {
        // AJUSTE: Usar isEliminado() e getIdProveedor()
        if (!p.isEliminado() && p.getIdProveedor() == idProv) {
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
            // AJUSTE: Usar isEliminado() e getId()
            if (!p.isEliminado() && p.getId() == idBuscado) {
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

    // 2. VERIFICACIÓN CRÍTICA
    if (proveedorTieneProductosBin(idBuscado)) {
        // AJUSTE: Usar getNombre()
        cout << "\nERROR: No se puede eliminar el proveedor '" << p.getNombre() << "'.\n";
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

    // 3. EJECUTAR BORRADO LÓGICO (AJUSTE: Usar Setter)
    p.setEliminado(true);
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

    // AJUSTE: Usar el setter setId para asignar el ID correlativo
    nuevo.setId(header.proximoID); 

    FILE* f = fopen(ruta, "ab"); // Append Binary
    if (f) {
        fwrite(&nuevo, sizeof(Cliente), 1, f);
        fclose(f);

        // Actualizamos los contadores del Header en el archivo
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
    char buffer[200]; // Buffer temporal para capturar los datos

    // --- Cedula / RIF ---
    solicitarString("Ingrese cedula/RIF del cliente (o CANCELAR): ", buffer, 20);

    if (strcmp(buffer, "CANCELAR") == 0 || strcmp(buffer, "0") == 0)
        return;

    // Validación contra el archivo clientes.bin
    if (clienteDuplicado(buffer)) {
        cout << "ERROR: La cedula/RIF '" << buffer << "' ya esta registrada.\n";
        return;
    }
    nuevo.setCedula(buffer); // AJUSTE: Usar setter

    // --- Nombre, Email, Telefono y Direccion ---
    solicitarString("Ingrese nombre del cliente: ", buffer, 100);
    nuevo.setNombre(buffer);

    solicitarString("Ingrese email: ", buffer, 100, emailValido);
    nuevo.setEmail(buffer);

    solicitarString("Ingrese telefono: ", buffer, 20);
    nuevo.setTelefono(buffer);

    solicitarString("Ingrese direccion: ", buffer, 200);
    nuevo.setDireccion(buffer);

    // --- Metadatos y Borrado Lógico ---
    char fecha[11];
    obtenerFechaActual(fecha);
    // Asumiendo que tienes un setFechaRegistro o similar, si no, puedes agregarlo
    // nuevo.setFechaRegistro(fecha); 
    nuevo.setEliminado(false); 

    // --- Resumen ---
    cout << "\n=== RESUMEN DEL CLIENTE ===\n";
    cout << "ID: (Se asignara al guardar)\n";
    cout << "Cedula/RIF: " << nuevo.getCedula() << endl; // AJUSTE: Usar getter
    cout << "Nombre: " << nuevo.getNombre() << endl;
    cout << "Email: " << nuevo.getEmail() << endl;
    cout << "Telefono: " << nuevo.getTelefono() << endl;
    cout << "Direccion: " << nuevo.getDireccion() << endl;

    if (confirmar("¿Guardar cliente en disco? (S/N): ")) {
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

    FILE* f = fopen("clientes.bin", "r+b"); 
    if (!f) {
        cout << "ERROR: El archivo de clientes no existe.\n";
        return;
    }

    ArchivoHeader h;
    fread(&h, sizeof(ArchivoHeader), 1, f);

    Cliente temp;
    long posicionRegistro = -1;
    bool encontrado = false;

    for (int i = 0; i < h.cantidadRegistros; i++) {
        posicionRegistro = ftell(f); 
        if (fread(&temp, sizeof(Cliente), 1, f)) {
            // AJUSTE: Usar isEliminado() e getId()
            if (!temp.isEliminado() && temp.getId() == idBuscado) {
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

    Cliente original = temp;

    cout << "\n=== CLIENTE ENCONTRADO ===\n";
    mostrarCliente(original);

    if (!confirmar("\n¿Desea modificar este cliente? (S/N): ")) {
        fclose(f);
        return;
    }

    char buffer[200];

    // --- Nombre ---
    cout << "\nNombre actual: " << original.getNombre() << endl;
    solicitarString("Nuevo nombre (ENTER para mantener): ", buffer, 100);
    if (strlen(buffer) > 0) temp.setNombre(buffer);

    // --- Cedula/RIF ---
    cout << "\nCedula/RIF actual: " << original.getCedula() << endl;
    solicitarString("Nueva cedula/RIF (ENTER para mantener): ", buffer, 20);
    if (strlen(buffer) > 0) {
        if (clienteDuplicado(buffer, original.getId())) {
            cout << "ERROR: La cedula '" << buffer << "' ya pertenece a otro cliente.\n";
            fclose(f); return;
        }
        temp.setCedula(buffer);
    }

    // --- Telefono ---
    cout << "\nTelefono actual: " << original.getTelefono() << endl;
    solicitarString("Nuevo telefono (ENTER para mantener): ", buffer, 20);
    if (strlen(buffer) > 0) temp.setTelefono(buffer);

    // --- Email ---
    cout << "\nEmail actual: " << original.getEmail() << endl;
    solicitarString("Nuevo email (ENTER para mantener): ", buffer, 100);
    if (strlen(buffer) > 0) {
        if (!emailValido(buffer)) {
            cout << "ERROR: Formato de email invalido.\n";
            fclose(f); return;
        }
        temp.setEmail(buffer);
    }

    // --- Direccion ---
    cout << "\nDireccion actual: " << original.getDireccion() << endl;
    solicitarString("Nueva direccion (ENTER para mantener): ", buffer, 200);
    if (strlen(buffer) > 0) temp.setDireccion(buffer);

    cout << "\n=== NUEVOS DATOS DEL CLIENTE ===\n";
    mostrarCliente(temp);

    if (confirmar("¿Guardar cambios en el archivo? (S/N): ")) {
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

    encabezadoClientes(); 

    Cliente c;
    int contadorActivos = 0;

    // Recorrido secuencial del archivo binario
    while (fread(&c, sizeof(Cliente), 1, f)) {
        // AJUSTE: Usar el getter isEliminado()
        if (!c.isEliminado()) {
            filaCliente(c); 
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

    for (int i = 0; i < h.cantidadRegistros; i++) {
        posicionReg = ftell(f); 
        if (fread(&c, sizeof(Cliente), 1, f)) {
            // AJUSTE: Usar isEliminado() e getId()
            if (!c.isEliminado() && c.getId() == idBuscado) {
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

    if (clienteTieneVentasBin(idBuscado)) {
        // AJUSTE: Usar getNombre()
        cout << "\nERROR: No se puede eliminar a " << c.getNombre() << ".\n";
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

    // AJUSTE: Usar setter setEliminado()
    c.setEliminado(true);
    fseek(f, posicionReg, SEEK_SET); 
    fwrite(&c, sizeof(Cliente), 1, f);
    fclose(f);

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

void registrarCompra() { 
    cout << "\n=== REGISTRAR COMPRA (DISCO) ===\n";

    // 1. Validar Producto
    int idProd;
    cout << "Ingrese ID del producto (0 para cancelar): ";
    cin >> idProd; cin.ignore();
    if (idProd == 0) return;

    Producto prod; 
    if (!buscarProductoPorID(idProd, &prod)) { // Asegúrate de que se llame igual a tu función de búsqueda
        cout << "Error: Producto no existe en el archivo." << endl;
        return;
    }
    cout << "Producto encontrado: " << prod.getNombre() << endl;

    // 2. Validar Proveedor
    int idProv;
    cout << "Ingrese ID del proveedor (0 para cancelar): ";
    cin >> idProv; cin.ignore();
    if (idProv == 0) return;

    Proveedor prov;
    if (!buscarProveedorPorIDBin(idProv, &prov)) {
        cout << "ERROR: El proveedor no existe o esta eliminado.\n";
        return;
    }
    cout << "Proveedor encontrado: " << prov.getNombre() << endl;

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

    // 4. Resumen (Usando Getters)
    cout << "\n=== RESUMEN DE COMPRA ===\n";
    cout << "Producto:  " << prod.getNombre() << " (Stock actual: " << prod.getStock() << ")\n";
    cout << "Proveedor: " << prov.getNombre() << "\n";
    cout << "Cantidad:  " << cantidad << "\n";
    cout << "Total:      " << total << "\n";

    if (!confirmar("¿Confirmar ingreso a almacen? (S/N): ")) return;

    // 5. OPERACIONES EN DISCO
	if (actualizarStockProducto(idProd, cantidad)) { 
        
        Transaccion t;
        t.setIdRelacionado(idProv); 
        t.setCantidad(0, cantidad); 
        t.setPrecioUnitario(0, precioUnit);
        t.setTotal(total);
        t.setTipo("COMPRA");
        t.setDescripcion("Compra a proveedor");
        
        char fecha[11];
        obtenerFechaActual(fecha);
        t.setFechaRegistro(fecha);
        t.setEliminado(false);

        registrarTransaccionEnDisco(t);

        cout << "\nInventario actualizado y compra registrada exitosamente.\n";
    }else {
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

    // AJUSTE: Usar getStock()
    if (p.getStock() <= 0) {
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
    cout << "Stock disponible: " << p.getStock() << endl;
    int cantidad = solicitarEnteroPositivo("Cantidad vendida (>0): ");

    if (cantidad > p.getStock()) {
        cout << "ERROR: Stock insuficiente.\n";
        return;
    }

    // AJUSTE: Usar getPrecio()
    float precioUnit = p.getPrecio();
    float total = precioUnit * (float)cantidad;

    // 4. Resumen (Usar Getters)
    cout << "\n=== RESUMEN DE VENTA ===\n";
    cout << "Producto: " << p.getNombre() << endl;
    cout << "Cliente:  " << cli.getNombre() << endl;
    cout << "Cantidad: " << cantidad << endl;
    cout << "Total:     " << total << endl;

    if (!confirmar("¿Confirmar venta? (S/N): ")) return;

    // 5. REGISTRO EN DISCO
    if (actualizarStockProducto(idProd, -cantidad)) { 
        
        Transaccion t;
        // Limpiamos los datos internos de la transacción mediante sus setters/métodos
        // Si tu clase Transaccion tiene un método para limpiar arreglos, úsalo aquí.

        t.setIdRelacionado(idCli);
        // Asignamos al índice 0 porque es una venta simple
        t.setIdProductoEnIndice(0, idProd);
        t.setCantidad(0, cantidad);
        t.setPrecioUnitario(0, precioUnit);
        t.setNumItems(1); 
        
        t.setTotal(total);
        t.setTipo("VENTA");
        t.setDescripcion("Venta a cliente");
        
        char fecha[11];
        obtenerFechaActual(fecha);
        t.setFechaRegistro(fecha); 
        t.setEliminado(false);

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

    if (op >= 1 && op <= 3) {
        cout << "Ingrese el ID a buscar: ";
        cin >> idBuscado; cin.ignore();
    } else if (op == 4) {
        solicitarString("Ingrese fecha (YYYY-MM-DD): ", filtro, 11);
    }

    fseek(f, sizeof(ArchivoHeader), SEEK_SET);

    Transaccion t;
    bool encontrado = false;
    cout << "\n=== RESULTADOS ENCONTRADOS ===\n";

    while (fread(&t, sizeof(Transaccion), 1, f)) {
        // AJUSTE: Usar getter isEliminado()
        if (t.isEliminado()) continue;

        bool coincide = false;

        switch (op) {
            case 1: // ID de factura (ID propio de la transacción)
                coincide = (t.getId() == idBuscado);
                break;

            case 2: // ID de producto (recorrer el arreglo interno)
                // AJUSTE: Usar getNumItems() e getIdProductoEnIndice(j)
                for (int j = 0; j < t.getNumItems(); j++) {
                    if (t.getIdProducto(j) == idBuscado) {
                        coincide = true;
                        break;
                    }
                }
                break;

            case 3: // ID Relacionado (Cliente o Proveedor)
                coincide = (t.getIdRelacionado() == idBuscado);
                break;

            case 4: // Por Fecha
                // AJUSTE: Usar getFechaRegistro()
                coincide = (strcmp(t.getFecha(), filtro) == 0);
                break;
        }

        if (coincide) {
            mostrarDetalleTransaccion(t); 
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

    ArchivoHeader h;
    fread(&h, sizeof(ArchivoHeader), 1, f);

    if (h.cantidadRegistros == 0) {
        cout << "El historial de transacciones esta vacio.\n";
        fclose(f);
        return;
    }

    encabezadoTransacciones(); 

    Transaccion t;
    int contActivas = 0;
    float recaudacionVentas = 0;
    float inversionCompras = 0;

    while (fread(&t, sizeof(Transaccion), 1, f)) {
        // AJUSTE: Usar isEliminado()
        if (!t.isEliminado()) {
            filaTransaccion(t); 
            contActivas++;

            // AJUSTE: Usar getTipo() y getTotal()
            if (strcmp(t.getTipo(), "VENTA") == 0) {
                recaudacionVentas += t.getTotal();
            } else if (strcmp(t.getTipo(), "COMPRA") == 0) {
                inversionCompras += t.getTotal();
            }
        }
    }

    pieTransacciones();

    // Resumen financiero rápido
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
        if (t.getId() == idBuscado && !t.isEliminado()) {
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
    for (int i = 0; i < t.getNumItems(); i++) {
        // AJUSTE: Usar getters para el arreglo interno
        int idP = t.getIdProducto(i);
        int cant = t.getCantidad(i);

        // AJUSTE: Usar getTipo()
        if (strcmp(t.getTipo(), "COMPRA") == 0) {
            // Cancelar compra: Restamos (el stock que entró, ahora sale)
            actualizarStockProducto(idP, -cant);
        } else {
            // Cancelar venta: Sumamos (el producto vuelve a la tienda)
            actualizarStockProducto(idP, cant);
        }
    }

    // 4. MARCAR COMO ELIMINADA (Borrado Lógico)
    t.setEliminado(true);
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
        
        cout << "1. Registrar compra\n";
        cout << "2. Registrar venta\n";
        cout << "3. Buscar transacciones\n";
        cout << "4. Listar historial completo\n";
        cout << "5. Cancelar transaccion\n";
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

    return 0;
}

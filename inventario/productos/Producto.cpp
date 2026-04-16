#include "Producto.hpp"

// 1. Constructor Vacío
Producto::Producto() {
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

// 2. Constructor Parametrizado
Producto::Producto(int _id, const char* _nombre, float _precio, int _stock) {
    *this = Producto(); // Inicializa todo en cero/nulo primero
    id = _id;
    setNombre(_nombre);
    setPrecio(_precio);
    setStock(_stock);
}

// 3. Constructor de Copia (Regla 2.1)
Producto::Producto(const Producto& otro) {
    id = otro.id;
    idProveedor = otro.idProveedor;
    precio = otro.precio;
    stock = otro.stock;
    stockMinimo = otro.stockMinimo;
    totalVendido = otro.totalVendido;
    eliminado = otro.eliminado;
    strcpy(codigo, otro.codigo);
    strcpy(nombre, otro.nombre);
    strcpy(descripcion, otro.descripcion);
    strcpy(fechaRegistro, otro.fechaRegistro);
}

// 4. Destructor
Producto::~Producto() {}

// --- Implementación de Setters con Validación ---

void Producto::setCodigo(const char* _cod) {
    strncpy(codigo, _cod, sizeof(codigo) - 1);
    codigo[sizeof(codigo) - 1] = '\0';
}

void Producto::setNombre(const char* _nom) {
    strncpy(nombre, _nom, sizeof(nombre) - 1);
    nombre[sizeof(nombre) - 1] = '\0';
}

void Producto::setDescripcion(const char* _desc) {
    strncpy(descripcion, _desc, sizeof(descripcion) - 1);
    descripcion[sizeof(descripcion) - 1] = '\0';
}

void Producto::setPrecio(float _precio) {
    precio = (_precio >= 0) ? _precio : 0.0f;
}

void Producto::setStock(int _stock) {
    stock = (_stock >= 0) ? _stock : 0;
}

void Producto::setFechaRegistro(const char* _fec) {
    strncpy(fechaRegistro, _fec, sizeof(fechaRegistro) - 1);
    fechaRegistro[sizeof(fechaRegistro) - 1] = '\0';
}

void Producto::setStockMinimo(int _min) {
    stockMinimo = (_min >= 0) ? _min : 0;
}

void Producto::setTotalVendido(int _total) {
    totalVendido = (_total >= 0) ? _total : 0;
}

// --- Métodos de Presentación ---

void Producto::mostrarInformacionBasica() const {
    cout << "[" << codigo << "] " << nombre << " - Cant: " << stock << " - Precio: $" << precio << endl;
}

void Producto::mostrarInformacionCompleta() const {
    cout << "\n-----------------------------------------" << endl;
    cout << "ID: " << id << " | Codigo: " << codigo << endl;
    cout << "Producto: " << nombre << endl;
    cout << "Descripcion: " << descripcion << endl;
    cout << "Stock Actual: " << stock << " (Minimo: " << stockMinimo << ")" << endl;
    cout << "Precio Unitario: $" << precio << endl;
    cout << "Total Vendido: " << totalVendido << " unidades" << endl;
    cout << "ID Proveedor: " << idProveedor << endl;
    cout << "Fecha Reg: " << fechaRegistro << endl;
    cout << "Estado: " << (eliminado ? "ELIMINADO" : "ACTIVO") << endl;
    cout << "-----------------------------------------" << endl;
}

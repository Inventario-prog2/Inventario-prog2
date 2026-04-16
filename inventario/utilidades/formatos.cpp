#include "formatos.hpp"
#include "../productos/Producto.hpp"
#include "../clientes/Cliente.hpp"
#include "../proveedores/Proveedor.hpp"

void Formatos::limpiar() {
    system("cls");
}

void Formatos::pausa() {
    cout << "\nPresione ENTER para continuar...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    system("pause > nul");
}

void Formatos::linea(int ancho, char simbolo) {
    for (int i = 0; i < ancho; i++) cout << simbolo;
    cout << endl;
}

void Formatos::titulo(string texto) {
    linea(80, '=');
    int espacios = (80 - (int)texto.length()) / 2;
    for(int i = 0; i < espacios; i++) cout << " ";
    cout << texto << endl;
    linea(80, '=');
}

// --- Implementación de Tablas con Bordes Dobles ---

void Formatos::encabezadoProductos() {
    // Esquina superior: +, Horizontal: -, Separador: -, Esquina final: +
    cout << (char)201 << string(4, (char)205) << (char)203 
         << string(12, (char)205) << (char)203 
         << string(25, (char)205) << (char)203 
         << string(10, (char)205) << (char)203 
         << string(8, (char)205) << (char)187 << endl;

    cout << (char)186 << " ID " << (char)186 << "   CODIGO   " << (char)186 
         << "          NOMBRE         " << (char)186 << "  PRECIO  " << (char)186 
         << " STOCK  " << (char)186 << endl;

    // Separador medio: ¦, -, +, ¦
    cout << (char)204 << string(4, (char)205) << (char)206 
         << string(12, (char)205) << (char)206 
         << string(25, (char)205) << (char)206 
         << string(10, (char)205) << (char)206 
         << string(8, (char)205) << (char)185 << endl;
}

void Formatos::filaProducto(const Producto& p, const char* nombreProv) {
    cout << (char)186 << " " << setw(2) << right << p.getId() << " " << (char)186 << " "
         << setw(10) << left << p.getCodigo() << " " << (char)186 << " "
         << setw(23) << left << p.getNombre() << " " << (char)186 << " "
         << setw(8) << right << fixed << setprecision(2) << p.getPrecio() << " " << (char)186 << " "
         << setw(6) << right << p.getStock() << " " << (char)186 << endl;
}

void Formatos::pieProductos() {
    // Esquina inferior: +, -, -, +
    cout << (char)200 << string(4, (char)205) << (char)202 
         << string(12, (char)205) << (char)202 
         << string(25, (char)205) << (char)202 
         << string(10, (char)205) << (char)202 
         << string(8, (char)205) << (char)188 << endl;
}

// ==========================================
// TABLA DE CLIENTES
// ==========================================
void Formatos::encabezadoClientes() {
    cout << (char)201 << string(5, (char)205) << (char)203 << string(15, (char)205) 
         << (char)203 << string(30, (char)205) << (char)187 << endl;
    cout << (char)186 << " ID  " << (char)186 << "    CEDULA     " 
         << (char)186 << "            NOMBRE            " << (char)186 << endl;
    cout << (char)204 << string(5, (char)205) << (char)206 << string(15, (char)205) 
         << (char)206 << string(30, (char)205) << (char)185 << endl;
}

void Formatos::filaCliente(const Cliente& c) {
    cout << (char)186 << " " << setw(3) << right << c.getId() << " " << (char)186 << " "
         << setw(13) << left << c.getCedula() << " " << (char)186 << " "
         << setw(28) << left << c.getNombre() << " " << (char)186 << endl;
}

void Formatos::pieClientes() {
    cout << (char)200 << string(5, (char)205) << (char)202 << string(15, (char)205) 
         << (char)202 << string(30, (char)205) << (char)188 << endl;
}

// ==========================================
// TABLA DE PROVEEDORES
// ==========================================
void Formatos::encabezadoProveedores() {
    cout << (char)201 << string(5, (char)205) << (char)203 << string(15, (char)205) 
         << (char)203 << string(30, (char)205) << (char)187 << endl;
    cout << (char)186 << " ID  " << (char)186 << "      RIF      " 
         << (char)186 << "            EMPRESA           " << (char)186 << endl;
    cout << (char)204 << string(5, (char)205) << (char)206 << string(15, (char)205) 
         << (char)206 << string(30, (char)205) << (char)185 << endl;
}

void Formatos::filaProveedor(const Proveedor& p) {
    cout << (char)186 << " " << setw(3) << right << p.getId() << " " << (char)186 << " "
         << setw(13) << left << p.getRif() << " " << (char)186 << " "
         << setw(28) << left << p.getNombre() << " " << (char)186 << endl;
}

void Formatos::pieProveedores() {
    cout << (char)200 << string(5, (char)205) << (char)202 << string(15, (char)205) 
         << (char)202 << string(30, (char)205) << (char)188 << endl;
}

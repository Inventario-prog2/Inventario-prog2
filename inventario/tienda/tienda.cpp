#include "Tienda.hpp"
#include <cstdio>
#include "../utilidades/Formatos.hpp"

Tienda::Tienda() {
    memset(nombre, 0, sizeof(nombre));
    memset(rif, 0, sizeof(rif));
    caja = 0.0f;
    totalVentas = 0.0f;
    totalCompras = 0.0f;
}

void Tienda::configuracionInicial(const char* _nom, const char* _rif) {
    strncpy(nombre, _nom, 99);
    strncpy(rif, _rif, 19);
    caja = 0.0f; // Puedes empezar con un saldo inicial si quieres
    guardarEnDisco();
}

bool Tienda::cargarDesdeDisco() {
    FILE* f = fopen("datos/tienda.bin", "rb");
    if (!f) return false;
    
    fread(this, sizeof(Tienda), 1, f);
    fclose(f);
    return true;
}

void Tienda::guardarEnDisco() {
    FILE* f = fopen("datos/tienda.bin", "wb");
    if (f) {
        fwrite(this, sizeof(Tienda), 1, f);
        fclose(f);
    }
}

void Tienda::sumarVenta(float monto) {
    caja += monto;
    totalVentas += monto;
    guardarEnDisco();
}

void Tienda::restarCompra(float monto) {
    caja -= monto;
    totalCompras += monto;
    guardarEnDisco();
}

void Tienda::mostrarEstado() {
    Formatos::limpiar();
    Formatos::titulo("ESTADO FINANCIERO DE LA TIENDA");
    std::cout << "Nombre:   " << nombre << "\n";
    std::cout << "RIF:      " << rif << "\n";
    std::cout << "-----------------------------------\n";
    std::cout << "Saldo en Caja:    $" << caja << "\n";
    std::cout << "Ventas Totales:   $" << totalVentas << "\n";
    std::cout << "Compras Totales:  $" << totalCompras << "\n";
    std::cout << "-----------------------------------\n";
}

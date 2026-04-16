#ifndef TIENDA_HPP
#define TIENDA_HPP

#include <iostream>
#include <cstring>

class Tienda {
private:
    char nombre[100];
    char rif[20];
    float caja;
    float totalVentas;
    float totalCompras;

public:
    Tienda();
    
    // Metodos principales
    void configuracionInicial(const char* _nom, const char* _rif);
    bool cargarDesdeDisco();
    void guardarEnDisco();
    void mostrarEstado();

    // Getters
    const char* getNombre() const { return nombre; }
    const char* getRif() const { return rif; }
    float getCaja() const { return caja; }

    // Metodos para transacciones
    void sumarVenta(float monto);
    void restarCompra(float monto);
};

#endif

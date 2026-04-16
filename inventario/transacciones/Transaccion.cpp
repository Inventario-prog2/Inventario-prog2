#include "Transaccion.hpp"

Transaccion::Transaccion() {
    id = 0;
    idRelacionado = 0;
    numItems = 0;
    total = 0.0f;
    eliminado = false;
    memset(tipo, 0, sizeof(tipo));
    memset(fechaRegistro, 0, sizeof(fechaRegistro));
    memset(descripcion, 0, sizeof(descripcion));

    for (int i = 0; i < 20; i++) {
        idProductos[i] = 0;
        cantidades[i] = 0;
        precioUnitario[i] = 0.0f;
    }
}

bool Transaccion::agregarDetalle(int _idProd, int _cant, float _precio) {
    if (numItems < 20) {
        idProductos[numItems] = _idProd;
        cantidades[numItems] = _cant;
        precioUnitario[numItems] = _precio;
        total += (_cant * _precio);
        numItems++;
        return true;
    }
    return false;
}

void Transaccion::setTipo(const char* _tipo) { strncpy(tipo, _tipo, 9); tipo[9] = '\0'; }
void Transaccion::setFecha(const char* _f) { strncpy(fechaRegistro, _f, 10); fechaRegistro[10] = '\0'; }
void Transaccion::setDescripcion(const char* _desc) { strncpy(descripcion, _desc, 199); descripcion[199] = '\0'; }

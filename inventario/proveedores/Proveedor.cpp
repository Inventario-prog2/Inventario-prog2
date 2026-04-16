#include "Proveedor.hpp"

Proveedor::Proveedor() {
    id = 0;
    eliminado = false;
    memset(nombre, 0, sizeof(nombre));
    memset(rif, 0, sizeof(rif));
    memset(telefono, 0, sizeof(telefono));
    memset(email, 0, sizeof(email));
    memset(direccion, 0, sizeof(direccion));
    memset(fechaRegistro, 0, sizeof(fechaRegistro));
}

Proveedor::Proveedor(int _id, const char* _nombre, const char* _rif) : Proveedor() {
    id = _id;
    setNombre(_nombre);
    setRif(_rif);
}

void Proveedor::setNombre(const char* n) { strncpy(nombre, n, 99); nombre[99] = '\0'; }
void Proveedor::setRif(const char* r) { strncpy(rif, r, 19); rif[19] = '\0'; }
void Proveedor::setTelefono(const char* t) { strncpy(telefono, t, 19); telefono[19] = '\0'; }
void Proveedor::setEmail(const char* e) { strncpy(email, e, 99); email[99] = '\0'; }
void Proveedor::setDireccion(const char* d) { strncpy(direccion, d, 199); direccion[199] = '\0'; }
void Proveedor::setFechaRegistro(const char* f) { strncpy(fechaRegistro, f, 10); fechaRegistro[10] = '\0'; }

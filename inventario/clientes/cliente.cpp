#include "cliente.hpp"

Cliente::Cliente() {
    id = 0;
    eliminado = false;
    memset(nombre, 0, sizeof(nombre));
    memset(cedula, 0, sizeof(cedula));
    memset(telefono, 0, sizeof(telefono));
    memset(email, 0, sizeof(email));
    memset(direccion, 0, sizeof(direccion));
    memset(fechaRegistro, 0, sizeof(fechaRegistro));
}

Cliente::Cliente(int _id, const char* _nombre, const char* _cedula) : Cliente() {
    id = _id;
    setNombre(_nombre);
    setCedula(_cedula);
}

void Cliente::setNombre(const char* n) { strncpy(nombre, n, 99); nombre[99] = '\0'; }
void Cliente::setCedula(const char* c) { strncpy(cedula, c, 19); cedula[19] = '\0'; }
void Cliente::setTelefono(const char* t) { strncpy(telefono, t, 19); telefono[19] = '\0'; }
void Cliente::setEmail(const char* e) { strncpy(email, e, 99); email[99] = '\0'; }
void Cliente::setDireccion(const char* d) { strncpy(direccion, d, 199); direccion[199] = '\0'; }
void Cliente::setFecha(const char* f) { strncpy(fechaRegistro, f, 10); fechaRegistro[10] = '\0'; }

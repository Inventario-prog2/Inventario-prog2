#ifndef PROVEEDOR_HPP
#define PROVEEDOR_HPP

#include <iostream>
#include <cstring>

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
    Proveedor();
    Proveedor(int _id, const char* _nombre, const char* _rif);

    // Getters
    int getId() const { return id; }
    const char* getNombre() const { return nombre; }
    const char* getRif() const { return rif; }
    const char* getTelefono() const { return telefono; }
    const char* getEmail() const { return email; }
    const char* getDireccion() const { return direccion; }
    const char* getFechaRegistro() const { return fechaRegistro; }
    bool isEliminado() const { return eliminado; }

    // Setters
    void setId(int _id) { id = _id; }
    void setNombre(const char* n);
    void setRif(const char* r);
    void setTelefono(const char* t);
    void setEmail(const char* e);
    void setDireccion(const char* d);
    void setFechaRegistro(const char* f);
    void setEliminado(bool estado) { eliminado = estado; }
};

#endif

#ifndef TRANSACCION_HPP
#define TRANSACCION_HPP

#include <iostream>
#include <cstring>

class Transaccion {
private:
    int id;
    char tipo[10];           // "COMPRA" o "VENTA"
    int idRelacionado;       // ID del proveedor (Compra) o cliente (Venta)
    
    int idProductos[20];    
    int cantidades[20];      
    float precioUnitario[20]; 
    int numItems;            
    
    float total;
    char fechaRegistro[11];
    char descripcion[200];
    bool eliminado;

public:
    Transaccion();

    // Lógica de negocio
    bool agregarDetalle(int _idProd, int _cant, float _precio);

    // Getters
    int getId() const { return id; }
    int getIdRelacionado() const { return idRelacionado; }
    float getTotal() const { return total; }
    int getNumItems() const { return numItems; }
    const char* getTipo() const { return tipo; }
    const char* getFecha() const { return fechaRegistro; }
    const char* getDescripcion() const { return descripcion; }
    bool isEliminado() const { return eliminado; }
    
    // Getters para los arreglos (necesitan índice)
    int getIdProducto(int indice) const { return (indice >= 0 && indice < 20) ? idProductos[indice] : -1; }
    int getCantidad(int indice) const { return (indice >= 0 && indice < 20) ? cantidades[indice] : 0; }
    float getPrecioUnitario(int indice) const { return (indice >= 0 && indice < 20) ? precioUnitario[indice] : 0.0f; }

    // Setters
    void setId(int _id) { id = _id; }
    void setTipo(const char* _tipo);
    void setIdRelacionado(int _idRel) { idRelacionado = _idRel; }
    void setEliminado(bool _e) { eliminado = _e; }
    void setFecha(const char* _f);
    void setTotal(float _t) { total = (_t >= 0) ? _t : 0; }
    void setDescripcion(const char* _desc);
    void setNumItems(int _n) { numItems = (_n >= 0 && _n <= 20) ? _n : 0; }
};

#endif

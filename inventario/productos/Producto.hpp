#ifndef PRODUCTO_HPP
#define PRODUCTO_HPP

#include <iostream>
#include <cstring>

using namespace std;

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
    // --- Constructores y Destructor (Regla 2.1) ---
    Producto();
    Producto(int _id, const char* _nombre, float _precio, int _stock);
    Producto(const Producto& otro); // Constructor de copia
    ~Producto();

    // --- Getters (const) ---
    int getId() const { return id; }
    const char* getCodigo() const { return codigo; }
    const char* getNombre() const { return nombre; }
    const char* getDescripcion() const { return descripcion; }
    int getIdProveedor() const { return idProveedor; }
    float getPrecio() const { return precio; }
    int getStock() const { return stock; }
    const char* getFechaRegistro() const { return fechaRegistro; }
    int getStockMinimo() const { return stockMinimo; }
    int getTotalVendido() const { return totalVendido; }
    bool isEliminado() const { return eliminado; }

    // --- Setters con Validación ---
    void setId(int _id) { id = _id; }
    void setCodigo(const char* _cod);
    void setNombre(const char* _nom);
    void setDescripcion(const char* _desc);
    void setIdProveedor(int _idProv) { idProveedor = _idProv; }
    void setPrecio(float _precio);
    void setStock(int _stock);
    void setFechaRegistro(const char* _fec);
    void setStockMinimo(int _min);
    void setTotalVendido(int _total);
    void setEliminado(bool _estado) { eliminado = _estado; }

    // --- Métodos de Presentación (Regla 2.1) ---
    void mostrarInformacionBasica() const;
    void mostrarInformacionCompleta() const;

    // --- Método Estático ---
    static size_t obtenerTamano() { return sizeof(Producto); }
};

#endif

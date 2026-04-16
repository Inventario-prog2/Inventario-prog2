#ifndef VALIDACIONES_HPP
#define VALIDACIONES_HPP

#include <iostream>
#include <string>
#include <cstring>
#include <ctime>
#include <limits>

using namespace std;

// Forward Declarations: Avisamos que estas clases existen
class Producto;
class Cliente;
class Proveedor;
class Transaccion;

class Validaciones {
public:
    // --- Lógica de Texto ---
    static string toLower(const char* s);
    static bool contiene(const char* campo, const string& filtro);
    static bool sinEspacios(const char* texto);
    
    // --- Validaciones de Formato ---
    static bool emailValido(const char* email);
    static bool validarFecha(const char* fecha);
    static void obtenerFechaActual(char* buffer);

    // --- Entradas de Usuario Seguras (Robustez) ---
    static bool confirmar(const char* mensaje);
    static int solicitarEnteroPositivo(const char* mensaje);
    static int solicitarEnteroNoNegativo(const char* mensaje);
    static float solicitarFloatPositivo(const char* mensaje);
    static void solicitarString(const char* mensaje, char* destino, int maxLen,
                            bool (*validador)(const char*) = nullptr,
                            const char* mensajeError = "ERROR: Valor invalido.\n");

    // --- Validaciones de Negocio (Búsquedas Rápidas) ---
    // Estas funciones llamarán al GestorArchivos internamente
    static bool productoExiste(int id);
    static bool clienteExiste(int id);
    static bool proveedorExiste(int id);
    
    // Verificación de Duplicados (Campos Únicos)
    static bool codigoProductoDuplicado(const char* codigo, int idIgnorar = -1);
    static bool cedulaClienteDuplicada(const char* cedula, int idIgnorar = -1);
    static bool rifProveedorDuplicado(const char* rif, int idIgnorar = -1);
};

#endif

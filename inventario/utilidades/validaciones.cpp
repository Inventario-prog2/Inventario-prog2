#include "validaciones.hpp"
#include "../persistencia/GestorArchivos.hpp"
#include "../productos/Producto.hpp"
#include "../clientes/Cliente.hpp" 
#include "../proveedores/Proveedor.hpp"
string Validaciones::toLower(const char* s) {
    string r = s;
    for (char& c : r) c = tolower(c);
    return r;
}

bool Validaciones::contiene(const char* campo, const string& filtro) {
    string a = toLower(campo);
    string f = toLower(filtro.c_str());
    return a.find(f) != string::npos;
}

bool Validaciones::sinEspacios(const char* texto) {
    if (!texto) return false;
    for (int i = 0; texto[i] != '\0'; i++) {
        if (isspace(texto[i])) return false;
    }
    return true;
}

bool Validaciones::emailValido(const char* email) {
    if (!sinEspacios(email)) return false;
    const char* at = strchr(email, '@');
    if (!at || at == email) return false;
    const char* punto = strchr(at, '.');
    return punto && *(punto + 1) != '\0';
}

void Validaciones::obtenerFechaActual(char* buffer) {
    time_t t = time(nullptr);
    tm* fecha = localtime(&t);
    sprintf(buffer, "%04d-%02d-%02d",
            fecha->tm_year + 1900,
            fecha->tm_mon + 1,
            fecha->tm_mday);
}

bool Validaciones::confirmar(const char* mensaje) {
    char op[10];
    cout << mensaje;
    cin >> op;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return toupper(op[0]) == 'S';
}

int Validaciones::solicitarEnteroPositivo(const char* mensaje) {
    int valor;
    while (true) {
        cout << mensaje;
        if (cin >> valor && valor > 0) {
            cin.ignore(1000, '\n');
            return valor;
        } else {
            cout << "ERROR: Ingrese un numero entero mayor a 0.\n";
            cin.clear();
            cin.ignore(1000, '\n');
        }
    }
}

int Validaciones::solicitarEnteroNoNegativo(const char* mensaje) {
    int valor;
    while (true) {
        cout << mensaje;
        if (cin >> valor && valor >= 0) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return valor;
        }
        cout << "ERROR: Ingrese un numero mayor o igual a cero.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

float Validaciones::solicitarFloatPositivo(const char* mensaje) {
    float valor;
    while (true) {
        cout << mensaje;
        if (cin >> valor && valor >= 0) {
            cin.ignore(1000, '\n');
            return valor;
        } else {
            cout << "ERROR: Ingrese un precio valido (>= 0).\n";
            cin.clear();
            cin.ignore(1000, '\n');
        }
    }
}

void Validaciones::solicitarString(const char* mensaje, char* destino, int maxLen,
                                   bool (*validador)(const char*),
                                   const char* mensajeError) 
{
    string entrada;
    bool valido = false;

    do {
        cout << mensaje;
        // Tu lógica original: lee con espacios
        getline(cin >> ws, entrada); 

        // 1. Validar que quepa en el char[] de destino
        if (entrada.length() >= (size_t)maxLen) {
            cout << "ERROR: El texto es demasiado largo (max " << maxLen - 1 << " caracteres)." << endl;
            continue;
        }

        // 2. Validar con la función externa (si existe una)
        if (validador != nullptr) {
            if (!validador(entrada.c_str())) {
                cout << mensajeError;
                continue;
            }
        }

        valido = true;
    } while (!valido);

    // Copiamos el string de C++ al char[] de C para que sea compatible con el .bin
    strncpy(destino, entrada.c_str(), maxLen);
    destino[maxLen - 1] = '\0'; // Aseguramos el fin de cadena
}

// --- Validaciones de Duplicados usando el Gestor de Archivos ---

bool Validaciones::codigoProductoDuplicado(const char* codigo, int idIgnorar) {
    ArchivoHeader h = GestorArchivos::leerHeader("datos/productos.bin");
    Producto p;
    for (int i = 0; i < h.cantidadTotal; i++) {
        if (GestorArchivos::leerPorIndice("datos/productos.bin", i, p)) {
            if (!p.isEliminado() && p.getId() != idIgnorar) {
                if (strcmp(p.getCodigo(), codigo) == 0) return true;
            }
        }
    }
    return false;
}

// --- Verificación de Clientes ---
bool Validaciones::cedulaClienteDuplicada(const char* cedula, int idIgnorar) {
    ArchivoHeader h = GestorArchivos::leerHeader("datos/clientes.bin");
    Cliente c;
    for (int i = 0; i < h.cantidadTotal; i++) {
        if (GestorArchivos::leerPorIndice("datos/clientes.bin", i, c)) {
            if (!c.isEliminado() && strcmp(c.getCedula(), cedula) == 0 && c.getId() != idIgnorar) {
                return true;
            }
        }
    }
    return false;
}

// --- Verificación de Proveedores ---
bool Validaciones::rifProveedorDuplicado(const char* rif, int idIgnorar) {
    ArchivoHeader h = GestorArchivos::leerHeader("datos/proveedores.bin");
    Proveedor p;
    for (int i = 0; i < h.cantidadTotal; i++) {
        if (GestorArchivos::leerPorIndice("datos/proveedores.bin", i, p)) {
            if (!p.isEliminado() && strcmp(p.getRif(), rif) == 0 && p.getId() != idIgnorar) {
                return true;
            }
        }
    }
    return false;
}

// --- Verificación de Existencia ---
bool Validaciones::proveedorExiste(int id) {
    ArchivoHeader h = GestorArchivos::leerHeader("datos/proveedores.bin");
    Proveedor p;
    for (int i = 0; i < h.cantidadTotal; i++) {
        if (GestorArchivos::leerPorIndice("datos/proveedores.bin", i, p)) {
            if (!p.isEliminado() && p.getId() == id) {
                return true;
            }
        }
    }
    return false;
}

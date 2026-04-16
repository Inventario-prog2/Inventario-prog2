#include <windows.h>
#include "interfaz/Interfaz.hpp"
#include "tienda/Tienda.hpp"

int main() {
    // 1. Cargamos la configuración global de la tienda (Caja, nombre, etc.)
    Tienda miTienda;
    if (!miTienda.cargarDesdeDisco()) {
    miTienda.configuracionInicial("A&S TECHNOLOGY", "J-12345678-9");
}

    // 2. Lanzamos el menú principal pasando la referencia de la tienda
    Interfaz::menuPrincipal(miTienda);
	system("pause");
    return 0;
}

#ifndef INTERFAZ_HPP
#define INTERFAZ_HPP

#include "../tienda/Tienda.hpp"

/**
 * @class Interfaz
 * @brief Clase encargada de la capa de presentacion y navegacion de los menus.
 */
class Interfaz {
public:
    /**
     * @brief Menu raiz del sistema que conecta con todos los modulos.
     * @param tienda Referencia al objeto tienda para gestionar caja y datos globales.
     */
    static void menuPrincipal(Tienda &tienda);

private:
    /**
     * @brief Dibuja el banner decorativo superior con el nombre de la empresa.
     * @param nombreTienda Nombre cargado desde el archivo de configuracion.
     */
    static void mostrarBanner(const char* nombreTienda);

    /**
     * @brief Muestra el resumen financiero rapido en la parte superior del menu.
     * @param tienda Objeto tienda para extraer el saldo actual de caja.
     */
    static void mostrarMiniDashboard(Tienda &tienda);
};

#endif // INTERFAZ_HPP

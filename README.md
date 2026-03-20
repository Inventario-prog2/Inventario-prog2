# Inventario-prog2

## Descripción General
Sistema desarrollado en C++ para la gestión completa de inventario, proveedores, clientes y transacciones de una tienda tecnológica. Utiliza archivos binarios para el almacenamiento persistente de datos.

## Características Principales

### Módulo de Productos
- Registro de productos con código único, nombre, descripción, precio y stock
- Asignación automática de ID
- Búsqueda por ID, nombre o código
- Actualización de datos y stock
- Listado completo en formato tabular
- Eliminación lógica (sin borrado físico)

### Módulo de Proveedores
- Registro con RIF único, nombre, email y teléfono
- Validación de RIF duplicado
- Búsqueda por ID, nombre o RIF
- Vinculación con productos (no se puede eliminar si tiene productos asociados)

### Módulo de Clientes
- Registro con cédula/RIF único
- Datos de contacto: email, teléfono, dirección
- Validación de formato de email
- Búsqueda por ID, nombre o cédula

### Módulo de Transacciones
- **Registro de Compras**: Entrada de productos al inventario (actualiza stock automáticamente)
- **Registro de Ventas**: Salida de productos con verificación de stock disponible
- **Búsqueda** por ID de factura, producto, cliente/proveedor o fecha
- **Cancelación de transacciones**: Revierte automáticamente el stock afectado
- Historial completo con resumen financiero

## Almacenamiento
- Archivos binarios: `productos.bin`, `proveedores.bin`, `clientes.bin`, `transacciones.bin`
- Header con metadatos (cantidad de registros, próximo ID)
- Borrado lógico (campo `eliminado`)
- Integridad referencial básica entre módulos

## Validaciones Implementadas
- Códigos y RIFs únicos
- Stock suficiente antes de vender
- Existencia de proveedor/cliente antes de transacción
- Formato de email válido
- Prevención de eliminación de registros con dependencias

## Interfaz
- Menús intuitivos por consola
- Tablas formateadas para visualización de datos
- Confirmaciones antes de operaciones críticas
- Mensajes de error claros

# Inventario-prog2
Este programa implementa un sistema de gestión de inventario para una tienda, desarrollado en C++ utilizando memoria dinámica, estructuras personalizadas y un menú interactivo. Permite administrar productos, proveedores, clientes y transacciones, cumpliendo con los requisitos de modularidad, validaciones y redimensionamiento de arreglos exigidos en el proyecto.

Inicialización:  
Se crean los arreglos dinámicos para productos, proveedores, clientes y transacciones, junto con sus contadores e IDs autoincrementales.

Menú Principal:  
El usuario selecciona una operación:
Productos (crear, buscar, actualizar, stock, eliminar, listar)
Proveedores (crear, buscar, actualizar, eliminar, listar)
Clientes (crear, buscar, actualizar, eliminar, listar)
Operaciones CRUD:  

Cada módulo permite:
Crear: Solicita datos, valida duplicados y formatos, asigna fecha y guarda.
Buscar: Por ID o coincidencia parcial (nombre, código, RIF, cédula).
Actualizar: Permite modificar campos con validaciones.
Eliminar: Solo si no tiene elementos asociados (productos, ventas, etc.).
Listar: Muestra tablas formateadas con iomanip.

Validaciones:  
El sistema verifica:
Emails válidos
Códigos, RIF y cédulas duplicadas
Existencia de IDs
Fechas en formato correcto

Redimensionamiento:  
Si un arreglo se llena, se duplica su capacidad y se copian los datos.

Salida:  
Al terminar, se libera toda la memoria dinámica.

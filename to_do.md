## Lista de tareas para implementar `PATH` y comandos en Pipex

- [ ] Obtener y procesar `PATH` desde `envp`:
  - [ ] Obtener la línea de `PATH` desde `envp`.
  - [ ] Dividir la línea de `PATH` en rutas utilizando `:` como delimitador.
  - [ ] Añadir `/` al final de cada ruta en `PATH` si no está presente.

- [ ] Parsear y preparar el comando de entrada:
  - [ ] Dividir el argumento del comando en un array de cadenas utilizando `ft_split`.
  - [ ] Concatenar cada ruta del `PATH` con el nombre del comando para generar las rutas completas del ejecutable.

- [ ] Ejecutar el comando con `execve`:
  - [ ] Probar cada ruta generada hasta encontrar el ejecutable válido.
  - [ ] Usar `execve` con el array de argumentos y `envp`.
  - [ ] Manejar errores si `execve` falla.

- [ ] Manejo de memoria y errores:
  - [ ] Liberar correctamente la memoria dinámica utilizada para las rutas de `PATH` y los argumentos del comando.
  - [ ] Implementar mensajes de error claros en cada paso (e.g., para `open`, `execve`, o funciones auxiliares).

- [ ] Verificar si se necesita algo más de `envp` además de `PATH` para el funcionamiento de `execve`.

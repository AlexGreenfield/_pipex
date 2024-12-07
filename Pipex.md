# Pipex

## Subject

`./pipex archivo 1 comando 1 comando2 archivo2`

`$> < archivo1 comando1 | comando2 > archivo2`

Básicamente hay que hacer que el primer comando actue como el segundo

El bonus tiene dos partes. La primera gestionar multiples pipes

`$> ./pipex archivo1 comando1 comando2 comando3 ... comandon archivo2`

`$> < archivo1 comando1 | comando2 | comando3 ... | comandon > archivo2`

Aceptar << y >> cuando el primer parametro es "here_doc":

`$> ./pipex here\_doc LIMIRADOR comando comando1 archivo`

`comando << LIMITADOR | comando1 >> archivo`

## Explicación

`<infile`: Redirige el contenido de un archivo `infile` como `stdin` para el comando1.

Es importante entender que aunque un comando no use el `stdin` para nada, por ejemplo `ls`, todos los procesos en UNIX tienen su `stdin`, `stdout`, `stederr`. Cuando estos no se establecen se toman  por defecto, y cuando no se usan simplemente se ignoran. En el caso de este proyecto, es necesario que te den explicitamente los 4 argumentos, no tienen que coger ninguno por defecto.

`|`: Toma el `stdout` del comando1 como `stdin` del comando2. Se encarga de la comunicación entre los dos comandos.

`>outfile` Esto redirige el `stdout` de comando2 a un archivo llamado `outfile` (se sobreescribe o crea).

## Funciones permitidas

* `open`
* `close`
* `read`
* `write`
* `perror`: Imprime mensaje personalizado seguiedo de la descripción del error `errno`.
* `strerror`: Devuelve cadena que describe el `errno` por si hay que procesarla de alguna forma.
* `access`: Verifica los permisos de acceso a un archivo (lectura, escritura o ejecución), la idea es llamarla antes de `execve` para comprobar si un archivo existe y es ejecutable.

### `dup`

**<unistd.h>** Syscall. `int dup(int oldfd);`
Crea una copia de `oldfd` usando el número de fd más bajo disponilble. Tras ejecutarse correctamente ambos file descriptors se refieren al mismo **file descrption**, es decir tienen las mismas *status flags* y el mismo *offset*.

Sin embargo ambos file descriptors no compartes las **file descriptors flags**. En este sentido el **FD_CLOEXEC** flag es off.

### `dup2`

**<unistd.h>** Syscall. `int dup2(int oldfd, int newfd);`
Lo mismo que `dup` pero devuelve `newfd`. Si `newfd` ya estaba siendo usado se cierra antes de reusarse. El proceso de cerrarse y reusarse se realizaría `atomicamente`, esto es importatnte, porque implementar la misma fucnionalidad con close y dup estaría sujeto a rece condition, ya que newfd podría ser reusado entre los dos pasos (los hilos comparten la memoria de un proceso y el mismo fd).

Si `oldfd` no es valido, la llamada falla y `newfd` no se cierra. Si `oldfd` es un descriptor valido y tiene el mismo valor que `newfd` no se hace nada y se devuelve `newfd`.

### `execve`

**<unistd.h>** Syscall. `int execve(const char *pathname, char *const argv[], char *const envp[]);`
Ejecuta el programa referido por `pathname`, que será la ruta al archivo ejecutable. Esto significa que el programa siendo ejecutado por el porceso de llamado es remplazado por un nueo programa, con una nuevo stack, heap y segmento de datos. Dicho de otra forma, se borra toda la info asociada al programa anterior y se inicializan las nuevas areas de memorias para el programa.

`argv` es un array de string con los argumentos por linea de ocmandos del programa. Aunque **no es obligatorio** normalmente el primer valor es el nombre del programa. Además este array debe acabar con `NULL`.

`envp` es un array de cadenas, convencionalmente de la forma `"key=value"`, que contiene las **variables de entorno** que el programa usará. También debe acabar en `NULL`.

Tanto `argv` como `envp` la función `main` del nuevo programa recibirá dichos argumentos. Sin embargo el uso de envp a través de main, si bien está admitido en muchos sistemas, no cumple POSIX, que recomiendo acceder al entorno usado la variable externa `environ` de `<unistd.h>`.

Una cosa chula es que en caso de exito, la función no retorna nunca al programa que le llamo. Ya que el programa anterior y toda su memoría es completamente sustituida por el nuevo programa, que no tendrá registro de como fue llamada.

`execve()` conserva los file descriptor del proceso que lo invoca.

Por últimoo si execve falla, retornará -1 y establecera `errno` con ele error especifico para que se pueda manejar el error.

### `fork()`

**<unistd.h> <sys/types.h>** Syscall. `pid_t fork(void);

Creau un nuevo proceso hijo duplicando el anterior. Ambos procesos corren en especios de memoria separados. Al momento que concluye la llamada a `fork()` ambos espacios de memoria son iguales.

* Las únicas diferencias entre el proceso padre y el hijo son:

* El proceso hijo tiene su propio ID de proceso, y este PID no encaja con ningun grupo de porcoeso o sesión existente.

* El hijo no hereda los locks de memoria del padre.

* `pipe`: Crea un canal  de comunicacióon unidireccional para dos extremos. Uno de lectura y otros de escritura.
* `unlink`: Elimina un archivo del sistema de archivos (para el manejo de archivos temporales)
* `wait`: Bloque hasata que un proceso hijo termine, devolviendo su estado.
* `waitpid`: Similar a wait pero con id.

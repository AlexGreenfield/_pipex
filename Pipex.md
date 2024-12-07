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

**Funciones que ya conocemos:**

* `open`
* `close`
* `read`
* `write`
* `malloc`
* `free`

**Nuevas funciones:**

### `perror`
Imprime mensaje personalizado seguiedo de la descripción del error `errno`.

### `strerror`


Devuelve cadena que describe el `errno` por si hay que procesarla de alguna forma.

### `access`

	int access(const char *pathname, int mode);

"Check user's permissions for a file"

Verifica que un programa puede acceder al archivo marcado por *pathname*, mientras que *mode* especifica que tipo de permiso (lectura, escritura, ejecución...) tiene el archivo. Se puede usar un bitwise OR o AND para comprobar varios valores.

En caso de éxito, devuelve 0. En error, -1.

Una implementación podría ser:

	if (access("rwfile", R_OK|W_OK) == 0)
		printf("rwfile is accessible in writing and reading mode\n");

La idea es llamarla antes de `execve` para comprobar si un archivo existe y es ejecutable.

### `dup`


	#include <unistd.h>

	int dup(int oldfd);


"Duplicate a file descriptor"

Syscall. Crea una copia de `oldfd` usando el número de fd más bajo disponible. Tras ejecutarse correctamente ambos file descriptors se refieren al mismo **file description**, es decir tienen las mismas *status flags* y el mismo *offset*.

Sin embargo ambos file descriptors no comparten las **file descriptors flags**. En este sentido el **FD_CLOEXEC** (que señala que un file descriptor se debe cerrar cuando se realiza un `exec`) es off. Es decir, que el file descriptor sobrevivirá en el nuevo programa aún después de `exec`. Para mas info, el [manual](https://www.gnu.org/software/libc/manual/html_node/Descriptor-Flags.html#:~:text=Macro%3A%20int%20FD_CLOEXEC%20%C2%B).

### `dup2`

	#include <unistd.h>

	int dup2(int oldfd, int newfd);

Syscall. Lo mismo que `dup`, pero en lugar de coger el número fd más bajo, se usa como argumento el número fd que quiera el usuario. Si `newfd` ya estaba siendo usado se cierra antes de reusarse (cuidado, porque lo hace sin avisar).

El proceso de cerrarse y reusarse se realizaría `atomicamente`, esto es importatnte, porque implementar la misma fucnionalidad con close y dup estaría sujeto a rece condition, ya que newfd podría ser reusado entre los dos pasos (los hilos comparten la memoria de un proceso y el mismo fd).

Si `oldfd` no es valido, la llamada falla y `newfd` no se cierra. Si `oldfd` es un descriptor valido y tiene el mismo valor que `newfd` no se hace nada y se devuelve `newfd`.

### `execve`

	#include <unistd.h>

	int execve(const char *pathname, char *const argv[], char *const envp[]);

Ejecuta el programa referido por `pathname`, que será la ruta al archivo ejecutable.

Esto significa que el programa siendo ejecutado por el porceso de llamado es remplazado por un nuevo programa, con una nuevo stack, heap y segmento de datos. Dicho de otra forma, se borra toda la info asociada al programa anterior y se inicializan las nuevas areas de memorias para el programa.

* `pathname` es la ruta del archivo a ejecutar. Debe ser un ejecutable binario, o un script con simbolos(!#) como los de bash

* `argv` es un array de string con los argumentos por linea de comandos del programa. Aunque **no es obligatorio** normalmente el primer valor es el nombre del programa. Además este array debe acabar con `NULL`. Vamos, que argv[argc] es `NULL`.

* `envp` es un array de strings, convencionalmente de la forma `"key=value"`, que contiene las **variables de entorno** que el programa usará. También debe acabar en `NULL`.

Un ejemplo de implementación: <!-- This is a coment outside-->


	execve(
		"./gnl", /*pathname*/
		"./gnl", <!--- Primer argumento, que es igual que el pathname -->
		"Input for GNL", <!--- Segundo argumento, que a su vez es el argv[1] del programa que vamos a ejecutar --->
		NULL, <!--- Señalamos el final de los argumentos --->
	)



Tanto `argv` como `envp` la función `main` del nuevo programa recibirá dichos argumentos. Sin embargo el uso de envp a través de main, si bien está admitido en muchos sistemas, no cumple POSIX, que recomiendo acceder al entorno usado la variable externa `environ` de `<unistd.h>`.

Una cosa chula es que, en caso de exito, la función no retorna nunca al programa que le llamó. Ya que el programa anterior y toda su memoria es completamente sustituida por el nuevo programa, que no tendrá registro de como fue llamada.

`execve()` conserva los file descriptor del proceso que lo invoca.

Por último si execve falla, retornará -1 y establecera `errno` con el error especifico para que se pueda manejar el error.

Para más info de la familia exec, este [vídeo](https://www.youtube.com/watch?v=OVFEWSP7n8c&ab_channel=CodeVault)

### `fork()`

**<unistd.h> <sys/types.h>** Syscall. `pid_t fork(void);

Crea un nuevo proceso hijo duplicando el anterior. Ambos procesos corren en especios de memoria separados. Al momento que concluye la llamada a `fork()` ambos espacios de memoria son iguales.

* Las únicas diferencias entre el proceso padre y el hijo son:

* El proceso hijo tiene su propio ID de proceso, y este PID no encaja con ningun grupo de porcoeso o sesión existente.

* El hijo no hereda los locks de memoria del padre.

* `pipe`: Crea un canal  de comunicacióon unidireccional para dos extremos. Uno de lectura y otros de escritura.
* `unlink`: Elimina un archivo del sistema de archivos (para el manejo de archivos temporales)
* `wait`: Bloque hasata que un proceso hijo termine, devolviendo su estado.
* `waitpid`: Similar a wait pero con id.

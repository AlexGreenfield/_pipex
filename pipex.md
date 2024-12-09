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

Nuestro amigo [CodeVault](https://www.youtube.com/@CodeVault) explica todo en [este video](https://www.youtube.com/watch?v=6xbLgZpOBi8&ab_channel=CodeVault).

## Funciones permitidas

**Funciones que ya conocemos:**

* `open`
* `close`
* `read`
* `write`
* `malloc`
* `free`

**Nuevas funciones:**

### `execve` y la familia `exec`

```c
#include <unistd.h>

int execve(const char *pathname, char *const argv[], char *const envp[]);
```
Ejecuta el programa referido por `pathname`, que será la ruta al archivo ejecutable.

La familia de los `exec` sirve para ejecutar un programa de C dentro de otro programa de C. P.e, ejecutar GNL como un ejecutable en lugar de una llamada a función. Esto significa que el programa siendo ejecutado por el porceso de llamado es remplazado por un nuevo programa, con una nuevo stack, heap y segmento de datos. Dicho de otra forma, se borra toda la info asociada al programa anterior y se inicializan las nuevas areas de memorias para el programa, (si todo va bien, claro).

Hay varias flags para utilizar `exec`.

* `l`: la `l` es de **lista**, indica que va a recibir una **lista de argumentos**. Es decir, que recibe los argumentos como una `va_list`, y cada uno de ellos se especifica de manera individual. Sirve cuando sabemos los argumentos de antemano y los podemos escribir a mano.

Por ejemplo, podemos ejecutar `ls` con las flags `-l` y `-a`

```c
int execl(const char *path, const char *arg0, ..., NULL);

execl("/bin/ls", "ls", "-l", "-a", NULL);
```

* `v`: la `v` es de **vector**, lo que significa que en lugar de pasar una lista de argumentos definida, pasamos un **array de strings**. Se emplea cuando no sabemos los argumenos de antemano y queremos utilizar un ejecutable de manera dinamica, de forma parecida a como funciona `[argv]`. Es excluyente a `l`.

```c
int execv(const char *path, char *const argv[]);

char *args[] = {"ls", "-l", "-a", NULL};
execv("/bin/ls", args);
```

* `p`: la `p`, como `execvp`, indica que los ejecutables se encuentran en el `PATH` del sistema o del entorno que estemos usando, por lo que ahorramos tiempo en definir el `pathname`. Por ejemplo, para ls, en lugar de pasar "/bin/ls" como argumento, basta con indicar "ls", y el programa buscara el ejecutable en la carpeta del sistema.

```c
char *args[] = {"ls", "-l", "-a", NULL};
execvp("ls", args);
```

* `e`: la `e` es de **environment** o **variables de entorno**. Mientras que normalmente usamos argumentos para darle información a un programa, también podemos pasarle una variable de entorno, compartiendo la misma información (argumentos, estados del heap, etc.) entre varios programas. Es la versión más flexible, ya que permite customizar el entorno, dar los paths especificos de los programas que queremos ejecutar, etc. La sintaxis es `KEY=VALUE`.

```c
char *args[] = {"/bin/ls", "-l", "-a", NULL};      // Argumento
char *env[] = {"HOME=/home/user", "PATH=/custom/bin", NULL}; // Variable de entorno modificada, ahora solo busca programas es en esa carpeta.
execve("/bin/ls", args, env);
```

Como vemos, hay diferentes tipos de `exec`, como `execv` o `execvp`, cada uno con sus diferencias. En este caso nos vamos a centrar en `execve`, aqui va el resumen.

* `pathname` es la ruta del archivo a ejecutar. Debe ser un ejecutable binario, o un script con simbolos(!#) como los de bash

* `argv` es un array de string con los argumentos por linea de comandos del programa. Aunque **no es obligatorio** normalmente el primer valor es el nombre del programa. Además este array debe acabar con `NULL`. Vamos, que argv[argc] es `NULL`.

* `envp` es un array de strings, convencionalmente de la forma `"key=value"`, que contiene las **variables de entorno** que el programa usará. También debe acabar en `NULL`.

Un ejemplo de implementación:

```c
char argv[] = {
	"./gnl", // Hay que pillar el pathname y la ruta hasta que lo pillas y lo devuelves. Copiar del 0 al 1, y después del 1 al 0 para sacarlo
	"Input for GNL", // Primer argumento, que es igual que el pathname. Aqui hay que sacar el comando
	NULL // Señalamos el final de los argumentos
};

execve(
	"./gnl", // Pathname del archivo a ejecutar
	argv
	);
```
Por último si execve falla, retornará -1 y establecera `errno` (más a continuación) con el error especifico para que se pueda manejar el error.

Para más info de la familia exec, este [vídeo](https://www.youtube.com/watch?v=OVFEWSP7n8c&ab_channel=CodeVault)

### `errno`, `strerror` y `perror`

Hemos dicho que si todo va bien, exec salta al nuevo proceso directamente, ignorando todo lo que hay después. Pero, ¿cómo gestionamos los casos en los que dé error? Aquí entra `errno`, `strerror` y `perror`.

```c
#include <errno.h>

int errno;
```
`errno` (*error number*) muestra un código de error de la última llamada al sistema o función a la librería. Cuando una llamada al sistema falla, generalmente devuelve -1 y asigna a la variable errno un valor que describe qué salió mal. (Estos valores se pueden encontrar en `<errno.h>`). Muchas funciones de la libreria hacen lo mismo. Por ejemplo:

```c
char *args[] = {"/nonexistent/program", NULL}; // Nonexistent program
char *envp[] = {NULL}; // Empty environment variables

execve(args[0], args, envp); // This will fail...
int error = errno; // ...we get errno value;

printf("%d", error); // ..and the value of int error will be errno flag 2, which is ENOENT (Error NO ENTry).
```
Esto nos da un número, pero para leer el mensaje completo y tener algo legible, debemos usar `strerror` (*string error*) o `perror` (*pointer error*).

```c
#include <stdio.h>

char *strerror(int errnum);

void perror(const char *s);
```

* `strerror` devuelve cadena que que describe el error de un `errno`, introducida como el argumento `errnum`. Devuelve un mensaje por defecto y preestablecido. Al devolver la cadena, hay que imprimirla por pantalla con otra funcion, como `printf`.

```c
FILE *file = fopen("/nonexistent/file", "r");

if (!file)
	printf("Error: %s\n", strerror(errno));
```

* `perror` imprime un mensaje personalizado seguido de la descripción del ultimo error `errno` (number of last error) de una llamada al sistema o de una funcion de la libreria. El mensaje que se devuelve es escrito a mano por el usuario.

```c
FILE *file = fopen("/nonexistent/file", "r");

if (!file)
	perror("Custom error message");
```

### `access`

```c
int access(const char *pathname, int mode);
```

"Check user's permissions for a file"

Verifica que un programa puede acceder al archivo marcado por *pathname*, mientras que *mode* especifica que tipo de permiso (lectura, escritura, ejecución...) tiene el archivo. Se puede usar un bitwise OR o AND para comprobar varios valores.

En caso de éxito, devuelve 0. En error, -1.

Una implementación podría ser:
```c
if (access("rwfile", R_OK|W_OK) == 0)
	printf("rwfile is accessible in writing and reading mode\n");
```

La idea es llamarla antes de `execve` para comprobar si un archivo existe y es ejecutable.

### `dup`

```c
#include <unistd.h>

int dup(int oldfd);
```


"Duplicate a file descriptor"

Syscall. Crea una copia de `oldfd` usando el número de fd más bajo disponible. Tras ejecutarse correctamente ambos file descriptors se refieren al mismo **file description**, es decir tienen las mismas *status flags* y el mismo *offset*.

Sin embargo ambos file descriptors no comparten las **file descriptors flags**. En este sentido el **FD_CLOEXEC** (que señala que un file descriptor se debe cerrar cuando se realiza un `exec`) es off. Es decir, que el file descriptor sobrevivirá en el nuevo programa aún después de `exec`. Para mas info, el [manual](https://www.gnu.org/software/libc/manual/html_node/Descriptor-Flags.html#:~:text=Macro%3A%20int%20FD_CLOEXEC%20%C2%B).

### `dup2`

```c
#include <unistd.h>

int dup2(int oldfd, int newfd);
```

Syscall. Lo mismo que `dup`, pero en lugar de coger el número fd más bajo, se usa como argumento el número fd que quiera el usuario. Si `newfd` ya estaba siendo usado se cierra antes de reusarse (cuidado, porque lo hace sin avisar).

El proceso de cerrarse y reusarse se realizaría `atomicamente`, esto es importatnte, porque implementar la misma fucnionalidad con close y dup estaría sujeto a rece condition, ya que newfd podría ser reusado entre los dos pasos (los hilos comparten la memoria de un proceso y el mismo fd).

Si `oldfd` no es valido, la llamada falla y `newfd` no se cierra. Si `oldfd` es un descriptor valido y tiene el mismo valor que `newfd` no se hace nada y se devuelve `newfd`.



### `fork()`

```c
#include <unistd.h>
#include <sys/types.h>**

pid_t fork(void);
```

Crea un nuevo proceso, denominado **hijo** duplicando el anterior, denominado **padre** (*parent and child process*). Ambos procesos corren en especios de memoria separados. Al momento que concluye la llamada a `fork()` ambos espacios de memoria son iguales.

* Las únicas diferencias entre el proceso padre y el hijo son:

* El proceso hijo tiene su propio ID de proceso, y este PID no encaja con ningun grupo de porcoeso o sesión existente.

* El hijo no hereda los locks de memoria del padre.

* `pipe`: Crea un canal  de comunicacióon unidireccional para dos extremos. Uno de lectura y otros de escritura.
* `unlink`: Elimina un archivo del sistema de archivos (para el manejo de archivos temporales)
* `wait`: Bloque hasata que un proceso hijo termine, devolviendo su estado.
* `waitpid`: Similar a wait pero con id.

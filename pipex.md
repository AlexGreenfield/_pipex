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

Nuestro amigo [CodeVault](https://www.youtube.com/@CodeVault) explica como reclear un pipe en [este video](https://www.youtube.com/watch?v=6xbLgZpOBi8&ab_channel=CodeVault).

Es un proyecto no muy grande, pero que introduce un montón de conceptos nuevos. Esta lista de reproducción abarca todo lo necesario y más sobre los procesos de UNIX. Pero si quieres ir al grano y empaparte con las nuevas funciones, usa este orden:

* [`exec` y `errno`](https://www.youtube.com/watch?v=OVFEWSP7n8c&ab_channel=CodeVault): como llamar y ejecutar otros procesos dentro de tu programa y gestionar los errores.

* [`fork`](https://www.youtube.com/watch?v=cex9XrZCU14&ab_channel=CodeVault): `exec` sustituye el proceso actual por uno nuevo, por lo que pierdes todo lo que haya después de `exec`... a no ser que dividas el proceso. Bienvenido a la gestión de procesos y cómo duplicarlos para hacer varias llamadas a otros programas.

* [`wait`](https://www.youtube.com/watch?v=tcYo6hipaSA&list=PLfqABt5AS4FkW5mOn2Tn9ZZLLDwA3kZUY&index=2) y [`waitpid`](https://www.youtube.com/watch?v=kCGaRdArSnA): ahora que tenemos procesos en paralelo, tenemos que aprender a ejecutarlos en el orden que queramos.

* [`pipe`](https://www.youtube.com/watch?v=Mqb2dVRe0uo&ab_channel=CodeVault): ahora que ya sabes todo lo necesario sobre procesos, vamos a ver manejo de *file descriptors* y cómo se comunican entre ellos.

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

### `fork()`

```c
#include <unistd.h>
#include <sys/types.h>

pid_t fork(void);
```
Crea un nuevo proceso, denominado **hijo** duplicando el anterior, denominado **padre** (*parent and child process*). Devuelve un int que es un ID diferente para cada fork o -1 si ha habido un error. El de *child* siempre será 0, por lo que tenemos que usar este dato para crear variables de control y ver qué programas queremos ejecutar. Un ejemplo.

```c
pid_t pid = fork();

if (pid < 0) // Error in fork
	perror("fork");
else if (pid == 0) // Child process
	printf("Child process\n");
else // Parent process
	printf("Parent process\n");
```
Ambos procesos corren en espacios de memoria separados, por lo que genera un nuevo proceso con el que podemos jugar, muy util para las llamadas a `exec`. Ten en cuenta que `fork` se crea desde la línea que se llama, por lo que sucesivas llamadas a `fork` irán creando diferente ramificaciones en un ratio de 2^n, un [arbol binario](https://www.geeksforgeeks.org/fork-and-binary-tree/).

### `wait` y `waitpid`

```c
#include <sys/types.h>
#include <sys/wait.h>

pid_t wait(int *wstatus);

pid_t waitpid(pid_t pid, int *wstatus, int options);
```
Ahora que tenemos varios procesos, estaría bien elegir en qué orden se van a ejecutar. Si llamamos fork directamente, ambos procesos se van a ejecutar de manera simultanea, lo que puede ser un problema. Por ejemplo, se puede intercalar un printf de un proceso con el otro, mostrando ambos resultados por pantalla a la vez.

`wait` para la ejecución de *parent* hasta que termine la ejecución de *child*, lo cual nos ayuda a escoger el orden de los procesos. `wait` por si misma va a esperar a que acabe el *child* que está activo en el momento, pero puede ser un problema si llamamos a `wait` desde el propio *child* sin otra llamada a `fork`, porque se va a quedar esperando hasta el infinito. Una solución fácil.

```c
pid_t id = fork();

if (id != 0)
	wait(NULL);
```
`wait` devuelve como valor el id del proceso por el que ha esperado y -1 si no había ningún proceso *child* por el que esperar, también útil para la gestión de los procesos, tal que así:

```c
pid_t id = fork();
int wait_id;

wait_id = wait(NULL);

if (wait_id == -1)
	printf("No children to wait");
else
	printf("%d finished execution", wait_id);
```
También podemos controlar el fin de un proceso con `errno` y la flag `ECHILD`

```c
while (wait(NULL) != -1 || errno != ECHILD)
	printf("waited for a child to finish");
```

Pero este metodo es demasiado rudimentario para cuando tenemos varios procesos a la vez, ya que `wait` espera al primer *child* que termina de procesarse, no discrimina cual. Ya que no podemos usar `getpid` ni `getppid` (que nos especificarían los id de cada proceso y lo podriamos usar como condición), para solucionarlo tenemos `waitpid`, una variante de `wait` en la que podemos especificar un id en concreto y que hasta que cambio de estado podemos esperar. Sus argumentos son:

* `pid`: la id del proceso en concreto al que queremos esperar, obtenido con `int id = fork()`. Tiene varios valores de entrada.

	* **pid > 0**: espera al id de un *children* específico.
	* **pid == 0**: espera a por cualquier *children* del mismo grupo del padre.
	* **pid == -1**: espera a cualquier *children*, independientemente del grupo, al igual que el `wait` original.
	* **pid < -1**: espera a que termine cualquier *children* del mismo grupo que el padre.

* `wstatus`: el cambio de estatus del proceso al que estamos esperando: el *children* ha terminado, ha sido parado o a reanudado. Se pueden consultar las flags en `<sys/wait.h>` y poner NULL si no se quiere marcar nada, pero algunos comunes.

	* **WIFEXITED**: devuelve distinto de 0 si *children* ha terminado con normalidad, como retorno main o exit.

	* **WIFEXITSTATUS**: devuleve código de salida de *children* si WIFEXITED es verdadero.

	* **WIFSIGNALED**: devuelve un valor si *children* acabó por una señal como SIGTERM o SIGKILL

	* **WIFSTOPPED**: devuelve un número si *children* se ha detenido con señales como SIGSTOP.

* `options`: opciones para modificar el comportamiento de `waitpid` cuando termine *children*, se puede usar 0 para no marcar nada o algunas de estas flags.

	* **WNOHANG**: *No hang*, no bloquea el programa en caso de que no haya *children* que hayan cambiado de estado. Es decir, primero verifica si algún hijo ha cambiado de estado, y si no es así retorna 0 y continua con el programa. Es la más interesante de usar, ya que se puede emplear como un simple check de si un proceso ha terminado o no.

	* **WUNTRACED**: continua con el programa en caso de que un *children* se detenga con alguna señal, como SIGSTOP o SIGSTP, que se envian cuando se pulsa Ctrl+z desde la terminal.

	* **WCONTINUED**: continua con el programa en caso de que se reanude un *children* con señales como SIGCONT.

### `pipe`

Crea un canal  de comunicacióon unidireccional para dos extremos. Uno de lectura y otros de escritura.

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






* `unlink`: Elimina un archivo del sistema de archivos (para el manejo de archivos temporales)

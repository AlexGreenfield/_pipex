# Pipex

Índice

- [Subject](#subject)
- [Explicación](#explicación)
- [Teoría file descriptors](#teoría-file-descriptors)
- [Funciones permitidas](#funciones-permitidas)
  - [Funciones que ya conocemos](#funciones-que-ya-conocemos)
  - [`execve` y la familia `exec`](#execve-y-la-familia-exec)
  - [`errno`, `strerror` y `perror`](#errno-strerror-y-perror)
  - [`fork()`](#fork)
  - [`exit` y `abort`](#exit-y-abort)
  - [`wait` y `waitpid`](#wait-y-waitpid)
  - [`pipe`](#pipe)
  - [`access`](#access)
  - [`dup` y `dup2`](#dup-y-dup2)
  - [`unlink`](#unlink)
- [Implementación básica](#implementación-básica)


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

* [`dup` y `dup2`](https://www.youtube.com/watch?v=5fnVr-zH-SE): ahora que ya tenemos todo lo necesario sobre procesos y demás, vamos a ver cómo duplicar un fd y cambiar STDOUT a un archivo en concreto.

## Teoría file descriptors.

Son números enteros que usa cada proceso para referirse a archivos.

Las **descripciones de archivo abiertas** son estrucuras internas del kernnel que contienen al informacións obre el archivo abierto como el *offset* y las flags de archivo (modo lectura/escritura).

Aunque distintos descriptores de archivo pueden apuntar al mismo open file description, luego tienen sus flags especificos, los llamadas **file descriptor flags**. Estas flags no perteneces a las descripciones de archivos así que no se comparten al hacer `dup`, por ejemplo.

Un descriptor de archivo importante para pipex es **FD_CLOEXEC** (Close On Exec) que indica si el descriptor debe cerrarse automaticamente después de que se ejecute un proceso con `execve`.

## Funciones permitidas

### Funciones que ya conocemos:

* `open`: en esta ocasión, vamos a utilizar `open` para crear el fichero en el que vamos a escribir. Para ello podemos usar la flag `O_CREAT`, que permite crear un fichero desde 0 con los permisos necesarios, como `0777`. También podemos usar un biwise `|` con `O_WRONLY` para escribir en el archivo si ya existe o crearlo si no. En resument, usar `open("somefile.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644)`.
* `close`
* `read`
* `write`: ahora tiene un giro, y es que en lugar de usar el fd 1 para escribir por pantalla, podemos escribir a otros fd. Util para redirigir output desde procesos diferentes.
* `malloc`
* `free`

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
Ambos procesos corren en espacios de memoria separados, por lo que genera un nuevo proceso con el que podemos jugar, muy util para las llamadas a `exec`. Ten en cuenta que `fork` se crea desde la línea que se llama, por lo que sucesivas llamadas a `fork` irán creando diferente ramificaciones en un ratio de 2^n, un [arbol binario](https://www.geeksforgeeks.org/fork-and-binary-tree/). Si quieres más, [aquí](https://www.geeksforgeeks.org/fork-system-call/) más info sobre cómo funciona `fork`.

### `exit` y `abort`

```c
#include <stdlib.h>

void exit(int status);
```

Termina el proceso actual y cierra todos los fd. Útil para la gestión de errores de `fork` o para controlar sus flujos. Admite dos valores como argumento de entrada:

* *0* o *EXIT_SUCCESS* para indicar que el proceso se cerró según lo esperado.

* *1* o *EXIT_FAILURE* para indicar que el proceso se cerró debido a un error.

Siguiendo con el ejemplo anterior:

```c
pid_t pid = fork();

if (pid < 0) // Error in fork
{
	perror("fork");
	exit(1); // También valdría exit(EXIT_FAILURE)
}
else if (pid == 0) // Child process
	printf("Child process\n");
else // Parent process
	printf("Parent process\n");
```

Aunque no se va a usar en este proyecto, `abort` hace lo mismo pero, a diferencia de `fork`, no cierra los fd. Esto es importante porque, en caso de dump core, `exit` puede llegar a corromper datos si se mete donde no debe.

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

```c
#include <unistd.h>

int pipe(int pipefd[2]);
```
Crea un canal  de comunicacióon unidireccional para dos extremos. Uno de lectura y otros de escritura. Necesita como input un array de dos int que contenga el fd de donde va a leer y el fd de donde va a sacar esa lectura. Podemos pensarlo igual que stdin y stdout, donde 0 es lectura y 1 es escritura.

```c
int fd[2];

// fd[0] Where to read;

// fd[1] Where to write;

```

En teoria es muy parecido a la funcion `read`, donde generabamos un fd solo de lectura a partir de un fichero determinado. Con `pipe`, lo que hacemos es conectar dos fd diferentes, uno de lectura (0) y otro de escritura (1);

Para empezar, deberíamos comprobrar que el pipe se ha abierto correctamente, devuelve -1 en caso de error.

```c
int fd[2];

if (pipe(fd) == -1)
	printf("error");
```

Una vez que tenemos el pipe abierto, podemos decirle de escribir en uno de los fd que hemos abierto a otro fd, con llamadas al sistema como `write`. Un ejemplo.

```c
int	fd[2]; // Abrimos array
int	x; // Int en desde el que vamos a escribrir
int y; // Int al que vamos a escribrir a traves del pipe

if (pipe(fd) == -1) // Lanzamos pipe y vemos que abra bien
{
	printf("error");
	return (1);
}

x = 2; // Asignamos valor a x...
write (fd[1], &x, sizeof(int)); //... y con el pipe se lo pasamos al fd[0].
close (fd[1]);// Cerramos uno de los extremos del pipe, ya que no lo vamos a usar.

read(fd[0], &y, sizeof(int)); // Asignamos el 2 de x en y leyendo a través de fd[0], gracias al pipe
close(fd[0]); // Y cerramos el pipe

printf("%d", y); // Imprimimos 2 a traves de la variable y.
```

Una vez abierto el `pipe`, no hace falta que tengamos que usar los dos extremos. Puede que solo queramos escribir en uno de los fd, por lo que podemos lanzar un `close` para el fd que no usemos.

Este manejo de `pirpe` es util en multiprocesos. Por ejemplos, en un proceso podemos hacer un write desde fd[1] a fd[0], y en el *parent* leer ese mismo fd. A la hora de hacer un `fork`, hay que tener en cuenta que el pipe se mantiene independiente. Es decir, aunque cerremos el `pipe` en un proceso children, sigue abierto en el parent. Por lo que es importante tenerlos vigilados para no dejar nada sin cerrar.

```c
int	fd[2]; // Abrimos array
int	x; // Int en desde el que vamos a escribrir
int y; // Int al que vamos a escribrir a traves del pipe
int	fork_id; // Variable para el id fork y control de procesos

if (pipe(fd) == -1) // Lanzamos pipe y vemos que abra bien
{
	perror("pipe error");
	return(1);
}

fork_id = fork(); // Dividimos el proceso en parent y child
if (fork_id == -1) // Variable de control para el fork
{
	perror("fork error");
	exit(1);
}

if (fork_id == 0) // Entramos en children para asignar el valor a x
{
	close(fd[0]); // Cerramos fd[0] ya que no lo vamos a usar en este proceso
	x = 2;
	write (fd[1], &x, sizeof(int)); // Hacemos el write en fd[1]...
	close(fd[1]); // ... y lo cerramos;
}
else // En el parent leemos el valor de x en y
{
	close (fd[1]); // Cerramos fd[1] ya que no vamos a escribir
	read (fd[0], &y, sizeof(int)); // Leemos el 2 del fd[0] en y
	close (fd[0]); // Cerramos el read
	printf("%d\n", y); // Y nuestro y vale 2!
}
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

La idea es llamarla antes de `execve` para comprobar si un archivo existe y es ejecutable. Lo mismo aplica a los ejectuables de PATH, tenemos que comprobar que existe el comando que queremos ejecutar y que ejecutable.

### `dup` y `dup2`

Para sacar el resultado de nuestro programa a un archivo en concreto en lugar de una terminal, necesitamos usar `dup` y `dup2`

```c
#include <unistd.h>

int dup(int oldfd);

int dup2(int oldfd, int newfd);
```

"Duplicate a file descriptor"

`dup` crea una copia de cualquier fd y lo asigna al usando el número de fd más bajo disponible. Es decir, que si un `open` un fd de 4, podemos duplicarlo al 5 y hacer otras cosas con el. Por ejemplo.

```c
int fd = open("example.txt", O_WRONLY | O_CREAT 0777); // Abrimos un archivo y le asignamos un fd

int fd_dup = dup(fd); // Duplicamos el fd y escribimos en el
write(fd, "Hello, ", 7);

write(fd_dup, "world!\n", 7); // Pero tambien podemos escribir en su copia

// Al acabar, example.txt contiene "Hello, world!"
```

Pero la chicha viene con `dup2`, ya que nos permite copiar un fd y asignarlo **a cualquier otro fd**, incluido `STDOUT`. Repito, incluido `STDOUT`. Por lo que si le asignamos 1 a nuestro fichero, cualquier escritura en `STDOUT` se realizará dentro del fichero que queramos, super útil para el pipex. Por ejemplo.

```c
int fd = open("output.txt", O_WRONLY | O_CREAT 0777); // Abrimos un fd

dup2(fd, STDOUT_FILENO) // Y sustituimos STDOUT por ese archivo

printf("Este mensaje se guarda en el archivo.\n");
printf("Cualquier otra salida de printf también irá al archivo.\n");

// Al acabar, output.txt contiene "Este mensaje se guarda en el archivo. Cualquier otra salida de printf también irá al archivo."
```

Si `oldfd` no es valido, la llamada falla y `newfd` no se cierra. Si `oldfd` es un descriptor valido y tiene el mismo valor que `newfd` no se hace nada y se devuelve `newfd`. Si `newfd` ya estaba siendo usado se cierra antes de reusarse (cuidado, porque lo hace sin avisar).

Tras ejecutarse correctamente ambos file descriptors se refieren al mismo **file description**, es decir tienen las mismas *status flags* y el mismo *offset*.

Sin embargo ambos file descriptors no comparten las **file descriptors flags**. En este sentido el **FD_CLOEXEC** (que señala que un file descriptor se debe cerrar cuando se realiza un `exec`) es off. Es decir, que el file descriptor sobrevivirá en el nuevo programa aún después de `exec`. Para mas info, el [manual](https://www.gnu.org/software/libc/manual/html_node/Descriptor-Flags.html#:~:text=Macro%3A%20int%20FD_CLOEXEC%20%C2%B).

El proceso de cerrarse y reusarse se realizaría `atomicamente`, esto es importatnte, porque implementar la misma fucnionalidad con close y dup estaría sujeto a rece condition, ya que newfd podría ser reusado entre los dos pasos (los hilos comparten la memoria de un proceso y el mismo fd).

### `unlink`

```c
#include <unistd.h>

int unlink(const char *pathname);
```

En caso de que algo falle, debemos borrar el archivo que hemos creado y en el que hemos intentado escribir. Normalmente usariamos `remove`, pero en este caso usaremos `unlink`. `unlink` Elimina la entrada del directorio marcada por `pathname`, y su contendio en caso de que ningún proceso esté haciendo uso del archivo. Un ejemplo de uso.

```c
// Abrir el archivo directamente
    int fd = open("example.txt", O_WRONLY | O_CREAT, 0644);

    // Redirigir stdout al archivo
    dup2(fd, STDOUT_FILENO);

    // Escribir en el archivo usando printf
    printf("Este texto se escribe en example.txt.\n");
    printf("Es un ejemplo sin gestión de errores.\n");

    // Cerrar el archivo
    close(fd);

    // Eliminar el archivo
    unlink("example.txt");
```

Devuelve 0 en caso de exito y -1 en caso de error, estableciendo la flag correspondiente de `errno`.

* `EACCES`: Permiso denegado para eliminar el archivo.
* `ENOENT`: El archivo no existe.
* `EPERM` o `EISDIR`: Intento de usar unlink en un directorio (no permitido para directorios).

## Implementación básica

Ahora que ya hemos visto las funciones principales de nuestro pipex, vamos a realizar una implementación básica. Pongamos que queremos replicar el siguiente comando.

```
ping -c 5 google.com | grep rtt
```

Vamos a entender un poco el proceso que seguiría este comando.

1. El comando `ping` lanza una llamada a Google y escribe la respuesta en STDOUT
2. En lugar de pasar por STDOUT, el simbolo `|` recoge esa salida y la pasa al siguiente comando, `grep`
3. `grep`, en lugar de leer de STDINT, lee del texto que le ha pasado `|`

Ahora, para recrear ese proceso en C, debemos hacer el siguiente orden.

1. Crear un `pipe` con dos fd diferentes, uno de lectura y otro de escritura.
2. Creamos dos procesos (*child*) diferentes
	1. Uno que ejecute el comando `ping` y escribe en fd[1] en lugar de STDOUT.
	2. Otro que ejecute el comando `grep` y lea de fd[0] en lugar de STDIN.
3. Un proceso final que espere a que los otros dos *child* se ejecuten en el orden correcto y cierre el *parent*.

Aqui hay un ejemplo con `execlp` (recuerda que debemos usar `execve`).

```c
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {
    int fd[2]; // Creamos el pipe y comprobamos el caso de error
    if (pipe(fd) == -1) {
        perror("pipe");
        return 1;
    }

    int pid1 = fork(); // Creamos el primer fork para `ping` y comprobamos el error
    if (pid1 < 0) {
        perror("fork pid1");
        return 2;
    }

    if (pid1 == 0) { // Proceso hijo 1: ejecuta `ping`
        close(fd[0]); // Cerramos el extremo de lectura del pipe
        dup2(fd[1], STDOUT_FILENO); // Redirigimos la salida estándar al extremo de escritura del pipe
        close(fd[1]); // Cerramos el extremo de escritura duplicado
        execlp("ping", "ping", "-c", "5", "google.com", NULL); // Ejecutamos `ping`
        perror("execlp ping"); // Si execlp falla
        exit(1); // Terminamos el hijo con error
    }

    int pid2 = fork(); // Creamos el segundo fork para `grep` y comprobamos el error
    if (pid2 < 0) {
        perror("fork pid2");
        return 3;
    }

    if (pid2 == 0) { // Proceso hijo 2: ejecuta `grep`
        close(fd[1]); // Cerramos el extremo de escritura del pipe
        dup2(fd[0], STDIN_FILENO); // Redirigimos la entrada estándar al extremo de lectura del pipe
        close(fd[0]); // Cerramos el extremo de lectura duplicado
        execlp("grep", "grep", "rtt", NULL); // Ejecutamos `grep`
        perror("execlp grep"); // Si execlp falla
        exit(1); // Terminamos el hijo con error
    }

    // En el proceso padre cerramos ambos extremos del pipe
    close(fd[0]);
    close(fd[1]);

    // Esperamos a que ambos procesos hijos terminen
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    return 0; // Todo ha salido bien
}
```

Ahora, si queremos lanzar este programa con un argumento desde la consola, debemos añadir las funcionalidades de `open` y tener en cuenta los argumentos (una vez más, este ejemplo usa `execlp` en lugar de `execve`).

```c
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
    int fd[2]; // Pipe entre los dos comandos
    int file_in, file_out; // Descriptores para archivo1 y archivo2
    int pid1, pid2;

    // Verificar número de argumentos
    if (argc != 5)
    {
        write(2, "Usage: ./pipex file1 cmd1 cmd2 file2\n", 36);
        return (1);
    }

    // Abrir archivo1 (modo lectura) y archivo2 (modo escritura)
    file_in = open(argv[1], O_RDONLY);
    if (file_in < 0)
    {
        perror("Error opening file1");
        return (1);
    }

    file_out = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (file_out < 0)
    {
        perror("Error opening file2");
        close(file_in);
        return (1);
    }

    // Crear el pipe y verificar errores
    if (pipe(fd) == -1)
    {
        perror("Pipe failed");
        close(file_in);
        close(file_out);
        return (1);
    }

    // Primer fork para ejecutar comando1
    pid1 = fork();
    if (pid1 < 0)
    {
        perror("Fork failed");
        close(file_in);
        close(file_out);
        return (1);
    }

    if (pid1 == 0) // Proceso hijo 1
    {
        dup2(file_in, STDIN_FILENO); // Redirigir STDIN desde archivo1
        dup2(fd[1], STDOUT_FILENO); // Redirigir STDOUT al pipe de escritura
        close(fd[0]); // Cerrar el extremo de lectura del pipe
        close(fd[1]);
        close(file_in);
        close(file_out);
        execlp("/bin/sh", "sh", "-c", argv[2], NULL); // Ejecutar comando1
        perror("Error executing cmd1");
        exit(1);
    }

    // Segundo fork para ejecutar comando2
    pid2 = fork();
    if (pid2 < 0)
    {
        perror("Fork failed");
        close(file_in);
        close(file_out);
        return (1);
    }

    if (pid2 == 0) // Proceso hijo 2
    {
        dup2(fd[0], STDIN_FILENO); // Redirigir STDIN desde el pipe de lectura
        dup2(file_out, STDOUT_FILENO); // Redirigir STDOUT hacia archivo2
        close(fd[0]);
        close(fd[1]);
        close(file_in);
        close(file_out);
        execlp("/bin/sh", "sh", "-c", argv[3], NULL); // Ejecutar comando2
        perror("Error executing cmd2");
        exit(1);
    }

    // Código del proceso padre
    close(fd[0]); // Cerrar extremos del pipe
    close(fd[1]);
    close(file_in); // Cerrar archivo1
    close(file_out); // Cerrar archivo2

    waitpid(pid1, NULL, 0); // Esperar a que termine el primer hijo
    waitpid(pid2, NULL, 0); // Esperar a que termine el segundo hijo

    return (0); // Todo ha salido bien
}
```


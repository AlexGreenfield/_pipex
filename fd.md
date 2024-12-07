# Teoría file descriptors.

Son números enteros que usa cada proceso para referirse a archivos. 

Las **descripciones de archivo abiertas** son estrucuras internas del kernnel que contienen al informacións obre el archivo abierto como el *offset* y las flags de archivo (modo lectura/escritura).

Aunque distintos descriptores de archivo pueden apuntar al mismo open file description, luego tienen sus flags especificos, los llamadas **file descriptor flags**. Estas flags no perteneces a las descripciones de archivos así que no se comparten al hacer `dup`, por ejemplo.

Un descriptor de archivo importante para pipex es **FD_CLOEXEC** (Close On Exec) que indica si el descriptor debe cerrarse automaticamente después de que se ejecute un proceso con `execve`.




# <div align="center"><img src="https://private-user-images.githubusercontent.com/74038190/271839856-3b4607a1-1cc6-41f1-926f-892ae880e7a5.gif?jwt=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJnaXRodWIuY29tIiwiYXVkIjoicmF3LmdpdGh1YnVzZXJjb250ZW50LmNvbSIsImtleSI6ImtleTUiLCJleHAiOjE3MjM3NDA1MDQsIm5iZiI6MTcyMzc0MDIwNCwicGF0aCI6Ii83NDAzODE5MC8yNzE4Mzk4NTYtM2I0NjA3YTEtMWNjNi00MWYxLTkyNmYtODkyYWU4ODBlN2E1LmdpZj9YLUFtei1BbGdvcml0aG09QVdTNC1ITUFDLVNIQTI1NiZYLUFtei1DcmVkZW50aWFsPUFLSUFWQ09EWUxTQTUzUFFLNFpBJTJGMjAyNDA4MTUlMkZ1cy1lYXN0LTElMkZzMyUyRmF3czRfcmVxdWVzdCZYLUFtei1EYXRlPTIwMjQwODE1VDE2NDMyNFomWC1BbXotRXhwaXJlcz0zMDAmWC1BbXotU2lnbmF0dXJlPWIxMzI5MTkxMjJhZGM4NDBjMmE1N2EzMDJkNTkxMzY2MTEzYWE3OTI3NTY4YmRkMTA0ZjU1ZjNjMGVjNDI5MzcmWC1BbXotU2lnbmVkSGVhZGVycz1ob3N0JmFjdG9yX2lkPTAma2V5X2lkPTAmcmVwb19pZD0wIn0.oyxPOHU8voHc-09OmNVd-rvk2ZkS_IAw_2FjHrHt_vI" width="80px" /> &nbsp; Tarea#2 | [Lab]SOPES1 &nbsp; <img src="https://private-user-images.githubusercontent.com/74038190/271839856-3b4607a1-1cc6-41f1-926f-892ae880e7a5.gif?jwt=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJnaXRodWIuY29tIiwiYXVkIjoicmF3LmdpdGh1YnVzZXJjb250ZW50LmNvbSIsImtleSI6ImtleTUiLCJleHAiOjE3MjM3NDA1MDQsIm5iZiI6MTcyMzc0MDIwNCwicGF0aCI6Ii83NDAzODE5MC8yNzE4Mzk4NTYtM2I0NjA3YTEtMWNjNi00MWYxLTkyNmYtODkyYWU4ODBlN2E1LmdpZj9YLUFtei1BbGdvcml0aG09QVdTNC1ITUFDLVNIQTI1NiZYLUFtei1DcmVkZW50aWFsPUFLSUFWQ09EWUxTQTUzUFFLNFpBJTJGMjAyNDA4MTUlMkZ1cy1lYXN0LTElMkZzMyUyRmF3czRfcmVxdWVzdCZYLUFtei1EYXRlPTIwMjQwODE1VDE2NDMyNFomWC1BbXotRXhwaXJlcz0zMDAmWC1BbXotU2lnbmF0dXJlPWIxMzI5MTkxMjJhZGM4NDBjMmE1N2EzMDJkNTkxMzY2MTEzYWE3OTI3NTY4YmRkMTA0ZjU1ZjNjMGVjNDI5MzcmWC1BbXotU2lnbmVkSGVhZGVycz1ob3N0JmFjdG9yX2lkPTAma2V5X2lkPTAmcmVwb19pZD0wIn0.oyxPOHU8voHc-09OmNVd-rvk2ZkS_IAw_2FjHrHt_vI" width="80px" />

Instrucciones<br>
1. Creación del Script: <br>
    ○ Escribe un script de Bash que genere 10 contenedores en Docker. <br>
    ○ Ejemplo utiliza la imagen alpine como base para los contenedores. <br>
    ○ Asigna nombres aleatoriamente. <br>

<br>

2. Entregables:<br>
    ○ Link del Repositorio.<br>
    ○ Script de Bash.<br>
    ○ Imagen de los contenedores.<br>




# Explicación del Script
Este script automatiza la creación de múltiples contenedores Docker con nombres aleatorios, lo cual es útil cuando se requieren múltiples instancias de contenedores.

# Shebang
Indica que el script debe ser ejecutado con bash.

<details>
<summary>Shebang </summary>

   
   #!/bin/bash 
   
</details>

<br>


# Definicion de variables 

`num_cont=10`: Define el número de contenedores que se crearán.

`image="alpine:latest"`: Especifica la imagen Docker que se usará para los contenedores (en este caso, la última versión de Alpine Linux).

<details>
<summary>Definicion de variables </summary>

    num_cont=10 #Número de iteraciones (contenedores)
    image="alpine:latest" #Imagen del docker
    


</details>
<br>

# Función para generar un nombre aleatorio

- `random_name()`: Esta función genera un nombre aleatorio para el contenedor.

- `cat /dev/urandom`: Genera una secuencia de bytes aleatorios.

- `tr -dc 'a-z0-9'`: Filtra los bytes para que solo queden caracteres alfanuméricos en minúscula.

- `fold -w 5`: Agrupa la secuencia en una longitud de 5 caracteres.

- `head -n 1`: Selecciona la primera línea generada.

- `echo "container_..."`: Devuelve un nombre con el formato container_nombreAleatorio.

<details>
<summary>Función para generar un nombre aleatorio</summary>

    random_name() {

        echo "container_$(cat /dev/urandom | tr -dc 'a-z0-9' | fold -w 5 | head -n 1)"

    }

</details>
<br>


# Comandos y permisos
- `chmod +x script.sh`: Se debe ejecutar este comando para otorgar permisos de ejecución al script.

- `docker ps -a`: Lista todos los contenedores (incluyendo los que no están en ejecución).

- `docker rm -f $(docker ps -a -q)`: Elimina todos los contenedores creados.
<br>


# Observaciones

En sistemas Unix y Linux, los permisos determinan qué acciones pueden realizarse sobre un archivo o directorio. Los permisos se dividen en tres categorías para tres tipos de usuarios: el propietario del archivo, el grupo al que pertenece el archivo, y todos los demás usuarios.

### Permisos básicos

Cada archivo o directorio tiene tres tipos de permisos:

1. **Lectura (`r`)**: Permite ver el contenido de un archivo o listar los archivos de un directorio.
2. **Escritura (`w`)**: Permite modificar el contenido de un archivo o agregar/eliminar archivos en un directorio.
3. **Ejecución (`x`)**: Permite ejecutar un archivo si es un programa o script. Para un directorio, permite entrar en él con `cd`.

### Uso de `chmod +x`

El comando `chmod` se utiliza para cambiar los permisos de un archivo o directorio. El `+x` específicamente añade el permiso de ejecución al archivo.

En el contexto de un script como `script.sh`:

```bash
chmod +x script.sh
```

### ¿Por qué usamos `+x`?

- `+x` permite que el script sea ejecutado como un programa. Sin este permiso, intentar ejecutar el script dará un error de "Permiso denegado" porque el sistema no lo reconocerá como un archivo ejecutable, incluso si su contenido es un script válido.

### Otros permisos y combinaciones

Puedes utilizar `chmod` con otras opciones para modificar diferentes permisos:

- **Añadir permisos**: 
  - `+r` para añadir permiso de lectura.
  - `+w` para añadir permiso de escritura.
  - `+x` para añadir permiso de ejecución.

- **Quitar permisos**: 
  - `-r` para eliminar permiso de lectura.
  - `-w` para eliminar permiso de escritura.
  - `-x` para eliminar permiso de ejecución.

- **Modificación con usuarios específicos**:
  - `u` para el propietario (usuario).
  - `g` para el grupo.
  - `o` para otros (todos los demás).
  - `a` para todos los usuarios (equivalente a `ugo`).

Ejemplos:

- `chmod u+x script.sh`: Añade permiso de ejecución solo para el propietario.
- `chmod g-w file.txt`: Quita el permiso de escritura al grupo.
- `chmod a+r file.txt`: Añade el permiso de lectura a todos.

### Uso con números (notación octal)

Además del uso con letras (`+x`, `-r`, etc.), los permisos se pueden definir con una notación octal de tres dígitos. Cada dígito representa un conjunto de permisos:

- **4**: Lectura (`r`)
- **2**: Escritura (`w`)
- **1**: Ejecución (`x`)

Estos valores se suman para crear combinaciones de permisos:

- **7**: Lectura, escritura y ejecución (`rwx`)
- **6**: Lectura y escritura (`rw-`)
- **5**: Lectura y ejecución (`r-x`)
- **4**: Solo lectura (`r--`)

Ejemplo:

```bash
chmod 755 script.sh
```

Esto establece:
- Propietario: `rwx` (7)
- Grupo: `r-x` (5)
- Otros: `r-x` (5)

### En Resumen

El uso de `chmod +x` es esencial para habilitar la ejecución de scripts y programas en un entorno Unix o Linux. Existen múltiples combinaciones de permisos, que se pueden ajustar según las necesidades de seguridad y acceso para diferentes usuarios y grupos.


<br>

 ![](https://user-images.githubusercontent.com/73097560/115834477-dbab4500-a447-11eb-908a-139a6edaec5c.gif)

#### <div align="center"> Desarrollado por
<div align="center">

<br>


Julio Alfredo Fernandez Rodriguez
201902416

<br>

<img src="https://user-images.githubusercontent.com/74038190/229223263-cf2e4b07-2615-4f87-9c38-e37600f8381a.gif" width="200px" />

<br>



<br>
<br>

 ![](https://user-images.githubusercontent.com/73097560/115834477-dbab4500-a447-11eb-908a-139a6edaec5c.gif)
# Quasar

El presente trabajo es la solución al ejercicio de Mercado Libre de realizar un servidor REST
que responda a una serie de mensajes POST y GET. Se encuentra implementado en C++, en su estandar 11.

Una instancia compilada se encuentra hosteada en un servidor de google cloud, y se puede interactuar
con él mediante la URL https://server-lk2keog37q-rj.a.run.app

## Compilación

Para poder compilar los fuentes se necesitan las siguientes librerías:

* [cpprest-sdk](https://github.com/microsoft/cpprestsdk) una API rest multiplataforma desarrollada por Microsoft
* [google-test](https://github.com/google/googletest) un framework de testing para correr los tests unitarios.

Estos se pueden instalar de distintas maneras. La que utilicé yo fue instalarlo mediante el repositorio de 
Linux:

```bash
$ sudo apt-get install libcpprest-dev libcpprest2.10 libgtest-dev googletest-tools googletest
```

Además de estas librerías, se requiere un framework para poder compilar código C++:

```bash
$ sudo apt-get install build-essential cmake git gcc g++ libc-dev
```

Una vez instaladas las dependencias, se utilizará `cmake` para compilar. Para ello, crearemos una carpeta de `build`.

```bash
# Clonamos el repositorio
$ git clone git@github.com:fedemgp/Quasar.git
$ cd Quasar
# Creamos la carpeta build
$ mkdir build
$ cd build
# compilamos
$ cmake ..
$ make -j4
```


## Test
Una vez compilado, se pueden correr los tests unitarios para ver que el programa esté funcionando correctamente. Para esto
se correrá el siguiente comando.

```bash
# En la carpeta 'build'
$ ctest
```

La salida del programa debería ser similar a la siguiente:

```
Test project /home/fedemgp/work/quasar/MeLi/build
    Start 1: quasar_test
1/1 Test #1: quasar_test ......................   Passed    0.02 sec

100% tests passed, 0 tests failed out of 1

Total Test time (real) =   0.02 sec
```

Se puede pasarle el flag `-VV` a ctest para tener un modo full verbose y ver cada uno de los 10 tests unitarios de la solución.

## Desarrollo

Una vez explicado como correr y compilar la aplicación, se empezará a explicar el desarrollo de la solución propuesta.
Se decidió escribir la solución en C++11 primero por comodidad (es un lenguaje en el que me siento extremadamente
cómodo), porque me dijeron que la elección del lenguaje era libre (mas allá que en el enunciado decia de hacerlo en Golang)
y porque prefería atacar primero el problema de los ejercicios antes de pelearme con un lenguaje de programación nuevo.

En la entrega final se planeaba entregar dos soluciones, una escrita en C++, y otra en Golang. Dado a las fechas festivas
que hubo en el medio, y una serie de dificultades técnicas que tuve que enfrentar (y se detallarán en la sección correspondiente)
no tuve el tiempo suficiente para aprender las sutilezas del lenguaje.

El servidor cuenta con la implementación de los tres niveles de ejercicios solicitados en el enunciado. Para ello se verá que en
[la clase server se registraron dos callbacks para los métodos POST y GET](https://github.com/fedemgp/Quasar/blob/main/src/Server.cpp#L23-L24)
con handlers de estas funciones. Cualquier otro método HTTP que se quiera correr, se devolverá un `405 method not allowed`.
Estos handlers [manejarán de forma polimórfica los request](https://github.com/fedemgp/Quasar/blob/main/src/Server.cpp#L68-L74),
en función de la uri que se se le pase. Cada path se encuentra modelado como un `HTTPController`, una interfaz que
define los métodos HTTP. En este proyecto existen dos Controladores: [TopSecretController](https://github.com/fedemgp/Quasar/blob/main/src/TopSecretController.cpp) y [TopSecretSplitController](https://github.com/fedemgp/Quasar/blob/main/src/TopSecretSplitController.cpp).
Estos controladores implementan la interfaz [HTTPController](https://github.com/fedemgp/Quasar/blob/main/src/HttpController.h)
y sobreescriben solamente los métodos que nos interesan.

Tienen como atributo una referencia a un [Decoder](https://github.com/fedemgp/Quasar/blob/main/src/Decoder.h).
Esta clase es la responsable de, dada una lista de distancias, devolver la posición estimada del emisor y también se encarga
de decodificar el mensaje secreto, mediante el merge de los mensajes decodificados de cada satélite.

La estimación de la posición del emisor se realiza mediante el proceso de la [trilateración](https://es.wikipedia.org/wiki/Trilateraci%C3%B3n).
En la página [stackexchange](https://math.stackexchange.com/questions/884807/find-x-location-using-3-known-x-y-location-using-trilateration)
se encontró el desarrollo de la fórmula de la trilateración usando 3 puntos genéricos en un plano. El decoder no es mas que 
una [implementación de C++ de esta solución](https://github.com/fedemgp/Quasar/blob/main/src/Decoder.cpp#L12-L54).

Para el descifrado del mensaje se procedió a resolverlo de una forma mas casera. Sabiendo que, salvo en el desfasaje inicial,
no puede haber strings vacíos para la misma palabra (es decir, que si el satélite x recibió un mensaje a descifrar "un mensaje secreto",
este satélite no puede enviar nunca un arreglo de la forma ["un", "", "", "secreto"]) entonces se sabe que los satélites enviarán
arreglos de tamaño `N + di`, con N el tamaño del mensaje, y di el desfasaje de ese satélite. 

Con esto en mente, se decidió eliminar los strings vacíos iniciales hasta que se encuentre un string no nulo (una especie de strip).
Es decir: Si me llegó ["", "", "", "un", "mensaje", "secreto"] luego del strip quedaría ["un", "mensaje", "secreto"]. Habiendo hecho esto,
Se que el arreglo de mayor tamaño será igual a N (salvo que ninguno de los tres satélites haya podido descifrar la primer palabra, es
un error que no se podrá detectar con el input dado). Sabiendo N, agrego strings vacíos al principio en los arreglos que tengan
tamaño menor a N (no pudieron descifrar las primeras palabras). Una vez que tengo los 3 arreglos de tamaño N, procedo a hacer una
compración 1 a 1. Para el i-ésimo elemento del arreglo, me quedo con aquel string no nulo y lo appendedo en el mensaje final. 
En la [función getString](https://github.com/fedemgp/Quasar/blob/main/src/Decoder.cpp#L55-L96) puede verse la implementación
de esta explicación. Además puede verse que el método lanza una excepción si es que ninguno de los tres satélites pudo descifrar
la palabra i-ésima, con i > 0 (nuevamente, la única palabra que no puede determinarse si no se pudo descifrar correctamente es
la primera).

### Concurrencia
La librería cpprest-sdk maneja internamente un threadpool para manejar los request de los clientes. Por lo tanto hay una serie
de elementos a proteger para así evitar Race conditions. Los elementos compartidos entre los distintos request son:

* El decoder (cada controlador tiene una referencia a este decoder)
* En el caso de TopSecretSplit, un mapa que contiene la información de cada satélite que realiza un POST

El primer caso, el del decoder, no hay por qué preocuparse de la concurrencia. Al tener [métodos constantes](https://github.com/fedemgp/Quasar/blob/main/src/Decoder.h#L38-L48), si se intentara modificar la instancia, el compilador nos informará de un error y 
el ejecutable no se compilará. Por lo tanto estos métodos son _stateless_. Si se acceden
concurrentemente desde varios hilos no es necesario aplicar ninguna contención que pueda ralentizar nuestro servidor. 
Es cierto que el método addSatelite si modifica la instancia de Decoder, pero dado que el enunciado establece que los
sátelites no variarán en el tiempo, se [agregaron los satélites en el constructor del servidor antes de que se empezara
a escuchar los request](https://github.com/fedemgp/Quasar/blob/main/src/Server.cpp#L18-L29) por lo que no hay problema
de concurrencia.

Para el segundo caso, si hay concurrencia. El controlador TopSecretSplit si tiene un objeto que tiene un estado,
que se va a ir actualizando a medida que se vayan haciendo los POST y GET correspondientes. Por esto, este controlador
tiene un mutex que protegerá el acceso del mapa compartido.

### Dificultades técnicas

El mayor problema al que me tuve que enfrentar a la hora de resolver este trabajo es el deployment a un servidor en la nube.
Tuve que pelearme bastante con google cloud para entender su CLI y como subir una instancia a un proyecto. Además surgieron
inconvenientes con Docker, ya que quería hacer un entorno de compilación de multiples etapas, en la que primero se compilara en un docker ubuntu 20.04, y otra en blanco (mucho mas ligero) que solo corra el ejecutable.

No se bien por qué, la copia del ejecutable de un container al otro me fallaba por lo que descarté el multi-stage deployment. 
Una vez solucionado lo del docker, me fallaba el deploy a google cloud porque mi servidor estaba apuntando a un
puerto distinto al default de google cloud. Una vez solucionado esto (que me llevó este ultimo fin de semana darme cuenta) faltaba
escribir la documentación. Dado que habia estipulado entregar la solución para el día de la fecha (28/12/2020) no me alcanzó el tiempo
de investigar sobre Go y solucionarlo también en ese lenguaje.

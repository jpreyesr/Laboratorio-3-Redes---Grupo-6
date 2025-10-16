# Laboratorio-3-Redes---Grupo-6

A continuación se explicara el flujo de trabajo en lo que respecta los archivos .c
y la elaboracion de los codigos que permitieron el correcto desarrollo del laboratorio.

# TCP
- Broker TCP

    En este codigo se aprecia inicialmente, la definicion del puerto que se utilizara, el numero maximo de clientes permitidos y el maximo tamaño del buffer. Dentro de la función principal, el broker crea un socket TCP y lo configura para poder reutilizar el puerto si fuera necesario. Después, asocia el socket a la dirección IP local y al puerto mediante bind(), y comienza a escuchar nuevas conexiones con listen(). A partir de ese momento, el broker queda listo para aceptar clientes. El funcionamiento principal se da dentro de un bucle infinito, donde el broker utiliza la función select() para manejar múltiples conexiones al mismo tiempo. Esto le permite saber qué sockets tienen actividad (por ejemplo, si un cliente quiere enviar o recibir datos) sin bloquear el programa.

    Cuando un cliente nuevo se conecta, el broker lo acepta y lee un caracter inicial que indica su rol. Inicialmente, esta "P" si es publisher o en segundo lugar, "S" si es subscriber. Luego de eso, el broker guarda esa información en un arreglo, junto con el identificador del cliente. Ahora bien, si un cliente publisher envía un mensaje, el broker lo recibe y lo reenvia a todos los clientes que sean subscribers. En cambio, si el mensaje proviene de un subscriber (lo cual no debería pasar), simplemente se ignora.



- Subscriber TCP

    Como bien se sabe, el subscriber es el cliente receptor del sistema, encargado de recibir los mensajes que luego seran distribuidos a traves del broker. inicialmente, se incluyen las librerías necesarias para manejar la entrada y salida estándar, las funciones de red, la creación de sockets y la conexión mediante TCP. Luego se definen constantes como el puerto (8080) y el tamaño del buffer (1024 B), que se usara para recibir los mensajes. Para empzar, el programa determina la dirección IP del broker que en este caso es 127.0.0.1. Posteriormente, crea un socket TCP, que es el canal de comunicación confiable por el cual se intercambiarán los datos. Despues, el broker configura la dirección del servidor y establece la conexión usando connect().

    Ahora bien, una vez establecida la conexión, el subscriber envía un solo caracter "S" para indicar al broker que su rol es el de suscriptor. De esta forma, el broker puede diferenciar entre los clientes que publican mensajes y los que reciben. Finalmente, el sistema entra en un bucle, donde espera mensajes que llegan desde el broker a través de la función recv(). Todo eso para que cada mensaje recibido se imprima en la consola. 

- Publisher TCP

    En este experimento, y como se sabe desde la parte de arquitectura, el publisher es el cliente emisor. Basicamente se conecta al broker TCP y manda los mensajes que este reenviará a los subscribers previamente definidos. Al ejecutarse, el programa crea un socket y se conecta a la dirección IP del broker, que por configuración dentro de la implementación, 127.0.0.1 en el puerto 8080. Posteriormente, establece la conexión mediante connect(), lo que crea un canal de comunicación confiable con el broker. Una vez conectado, el publisher envía un caracter "P" para indicarle al broker que su rol es el de publicador. Después de esa identificación, el programa entra en un bucle continuo donde el usuario puede escribir mensajes directamente. Cada linea se manda al broker a traves del socket utilizando la función send(). Si ocurre algún error o el usuario termina la entrada, el programa finaliza sin problmas.


# UDP
- Broker UDP

    En este caso, evidentemente el broker UDP cumple con la misma funcion que el de TCP, solo que esta vez cambia el protocolo. 

    Al inicio, se definen las librerías necesarias para manejar sockets, direcciones IP, errores y memoria. Luego se establecen algunas constantes, como el puerto del broker (5000), el máximo número de suscriptores (10) y el tamaño del buffer (1024 bytes), que sirven para controlar los límites del programa. Después se crea una estructura llamada subscriber, que almacena la dirección IP del suscriptor y el nombre del tema al que está suscrito. De esta forma, el broker puede saber a qué dirección debe reenviar los mensajes que correspondan a ese tema.

    Posteriormente, el programa crea un socket UDP, lo configura para reutilizar el puerto y lo asocia (con bind) a la dirección local del broker. Una vez hecho esto, el broker queda escuchando a los mensajes que lleguen a ese puerto y muestra un mensaje en pantalla confirmando que esta funcionando. A partir de ahí, el broker entra en un ciclo infinito donde espera datagramas. Cuando recibe uno, revisa si el mensaje comienza con la palabra "SUBSCRIBE". En caso de ser así, lo interpreta como una solicitud de suscripción y guarda la dirección del cliente junto con el tema que pidió. Si el mensaje no es una suscripción, el broker lo trata como una publicación y lo reenvía a todos los suscriptores cuyo tema aparezca dentro del mensaje.

- Subscriber UDP

    Al igual que fue establecido antes, el subscriber es aquel que recibe todos los mensajes que son enviados dentro del sistema. Precisamente, para la gracia del ejercicio, su funcion es suscribirse a un topic específico y asi poder recibir los mensajes que el broker reenvie sobre ese tema. En ejecución, se incluyen las librerías necesarias para manejar sockets, direcciones IP, entrada y salida estándar, y funciones básicas del sistema. Adicionalmente, se definen constantes como el puerto del broker y el tamaño máximo del buffer (1024 bytes), que son necesarios para almacenar y manejar los mensajes.

    Dentro del programa, se crea un socket UDP, que permite la comunicación sin conexión(por el protocolo) con el broker. Despues de eso, se configura una dirección local (local_addr) que le permite al subscriber recibir mensajes en cualquier puerto disponible, y se asocia el socket a esa dirección mediante la función bind(). Posteriormente, se prepara la dirección del broker con su IP (en este caso, 192.168.64.3) y el puerto donde está escuchando. El programa luego le pide al usuario que ingrese el topic al que desea suscribirse , y construye un mensaje del tipo “SUBSCRIBE <tema>”, que es enviado al broker usando sendto().
    
    Ya finalmente, el programa entra en un bucle en donde utiliza recvfrom() para escuchar los mensajes que el broker le envía. Cada vez que llega un datagrama, lo imprime inmediatament.


- Publisher UDP

    Como ya se ha mencionado en todos los casos anteriores, el programa incluye las llibrerias para anejo de salida y entrada de datos, asi como de memoria y funciones de red. Además de definir el puerto y tamaño del buffer. Dentro del main, se crea un socket, que como ya se sab es el medio por el cual el publisher enviara los datagramas al broker. Luego se configura la estructura broker_addr, que contiene la dirección IP del broker y el puerto donde está escuchando.

    Luego de eso, el programa muestra un mensaje indicando que está listo para enviar información. A partir de ese momento, entra en un ciclo en donde el usuario puede escribir mensajes. Cada uno que el usuario escriba se lee con fgets(), se limpia de saltos de línea y se envía al broker usando la función sendto(). El broker, al recibir estos mensajes, los manda a los suscriptores que estén registrados en el tema correspondiente. 


    # LIBRERIAS / DIRECTIVAS EMPLEADAS

    #include <stdio.h> - Proporciona funciones estandar de C, las cuales sirven para depurar el codigo, reportandoselo en forma de errores al usuario

    #include <stdlib.h> - Proporciona utilidades generales como exit(), malloc, free y EXIT_FAILURE(), es necesaria para teminaciones necesarias del programa.

    #include <string.h> - Es la que proporciona funciones de manejo de cadenas y memoria como memset, strcpy y strlen. Basicamente es de las mas utiles desde que colabora en la manipulacion de buffers y estructuras.

    #include <unistd.h> - Essta es indispensable para operaciones basicas en el sistema linux, ya que permite el uso de funciones como close(), read, write, sleep, etc.

    #include <arpa/inet.h> - Proporciona funciones y estructuras para manipular direcciones de red: inet_ntoa, inet_pton, htons, ntohs, y la definición de struct in_addr. En este caso es sumamente necesaria para convertir entre formatos de red y host y trabajar con estructuras de dirección IPv4.

    #include <sys/socket.h> - esta es absolutamente esencial para cualquier programación de sockets en C, ya que define las llamadas y los tipos asociados. Precisamente contiene definiciones del API de sockets como socket(), bind(), etcc

    #include <sys/types.h> - En este caso fue incluida ya que provee compatibilidad para que tipos como ssize_y esten disponbles.

    #include <sys/select.h> - esta es fundamental ya que ceclara select() y macros/estructuras relacionadas. En este caso, dicha funcion se empela para multiplexar entre varios sockets.


# PARA CORRER EL CÓDIGO IMPLEMENTADO

    Lo que se llebo a cabo fue el ingreso a máquinas virtuales Linux, las cuales permitieran sin problemas la ejecucion
    de los codigos en conjunto. Se ingreso a un directorio cada grupo dependiendo del protocolo (TCP o UDP), para luego
    compilar cada archivo haciendo uso de "gcc", y luego llevando a cabo las pruebas propuestas dentro del enunciado. 

    Para que las pruebas se realicen de forma exitosa, igualmente se tienen que ejecutar los archivos en el siguiente orden: 
        1. broker
        2. subscribers
        3. publishers
    
    Esto es asi ya que el broker actua como punto de encuentro o conexion principal, ya que escucha a los puertos y recibe conexiones y mensajes de los demás. Luego los subscribers ya que son clientes receptores, y son encargados de recibir todo aquello que luego sera impreso, y ademas, no tiene sentido enviar algo, sin que algo lo pueda recibir. Precisamente es por eso que los publishers se ejecutan de últimas, ya que son los que emiten la informacion.
    

    SE HACE ENTREGA DE LOS CODIGOS Y ARCHIVOS PCAP DENTRO DEL GIT. IGUALMENTE ESTA EL COMPRIMIDO DENTRO DE LA ENTREGA EN BLOQUE NEON.



# Laboratorio-3-Redes---Grupo-6

A continuación se explicara el flujo de trabajo en lo que respecta los archivos .c
y la elaboracion de los codigos que permitieron el correcto desarrollo del laboratorio.

# TCP
- Broker TCP

    En este codigo se aprecia inicialmente, la definicion del puerto que se utilizara, el numero maximo de clientes permitidos y el maximo tamaño del buffer. Posteriormente se hace uso de la funcion DIE para imprimir error en caso de ocurrir algun fallo. Al iniciar, crea un socket y escucha en el puerto 8080, esperando conexiones entrantes. Cada cliente que se conecta , manda un carácter para indicar su rol: publisher si envia mensajes o subscriber si los recibe. En el codigo se crea una lista de todos los clientes, para que luego con la función select(), se detecte cuando llegan datos sin necesidad de usar varios hilos. Cuando un publisher envía un mensaje, este broker lo recibe y lo reenvía automáticamente a todos los subscribers conectados. No pasa nada si un cliente se desconecta, ya que este itera constantemente.

- Subscriber TCP

    Como bien se sabe, el subscriber es el cliente receptor del sistema, encargado de recibir los mensajes que luego seran distribuidos a traves del broker. inicialmente, se crea un socket que se conecta al broker en el puerto 8080 con IP 127.0.0.1. Una vez establecida la conexión, envía un carácter "S" para aclarar su rol como subscriber. Luego entra en un bucle permanente donde espera mensajes provenientes del broker usando la función recv(). Ahora bien, cada vez que llega un mensaje, lo muestra directamente en pantalla, como bien fue apreciado dentro en el flujo de trabajo. Cabe decir que si el broker cierra la conexion o se interrumpe la comunicación, el programa lo detecta, muestra un aviso y finaliza. En pocas palabras, este cliente permanece a la escucha de publicaciones, funcionando como un receptor PASIVO que muestra en tiempo real los mensajes enviados.

- Publisher TCP

    En este experimento, y como se sabe desde la parte de arquitectura, el publisher es el cliente emisor. Basicamente se conecta al broker TCP y manda los mensajes que este reenviará a los subscribers previamente definidos. Al ejecutarse, el programa crea un socket y se conecta a la dirección IP del broker, que por configuración dentro de la implementación, 127.0.0.1 en el puerto 8080. Una vez establecida la conexión, envía un carácter "P" para indicar su rol de publisher. Esta parte se definió previamente en la explicacion del broker. Luego entra en un bucle donde lee texto desde la consola y lo manda al broker usando TCP. Como se sabe, es gracias a este protocolo que los mensajes llegan de forma ordenada y segura. Ahora bien, una vez el usuario termina, el programa cierra la conexión y finaliza. 


# UDP
- Broker UDP

    En este caso, evidentemente el broker UDP cumple con la misma funcion que el de TCP, solo que esta vez cambia el protocolo. Inicialmente el programa crea un socket UDP y lo asocia al puerto 5000 para poder recibir mensajes de cualquier direccion. Cabe destacar que por problemas en la conexión se quiso probar el uso de este numero de puerto. Además, cabe destacar que a diferencia del TCP, aquí no hay una conexión establecida, sino que cada mensaje llega como un datagrama independiente. Para empezar el proceso, este entra en un ciclo continuo donde espera mensajes. Si recibe uno que comienza con la palabra "SUBSCRIBE", interpreta que ese cliente desea suscribirse a un topic específico. Luego de eso almacena su dirección IP y el nombre del tema en una lista. Si en cambio recibe un mensaje normal, lo toma como una publicacion corriente y lo reenvía a todos los suscriptores cuyos temas coincidan con el contenido del mensaje. Finalmente, el proceso es sencillo desde el hecho de que no hay garantía de entrega ni de orden en el envio de mensajes.

- Subscriber UDP

    Al igual que fue establecido antes, el subscriber es aquel que recibe todos los mensajes que son enviados dentro del sistema. Precisamente, para la gracia del ejercicio, su funcion es suscribirse a un topic específico y asi poder recibir los mensajes que el broker reenvie sobre ese tema. En ejecución, el programa crea un socket y lo asocia a un puerto local disponible. Esto para posteriormente, configurar la dirección del broker (en este caso con IP 192.168.64.3 y puerto 5000). Como tal, la idea es que el usuario o publisher ingresa el nombre del topic al que desea suscribirse y luego se manda un mensaje al broker con el formato SUBSCRIBE -topic-. De esta manera, el broker registra la dirección del suscriptor junto con el tema indicado.

    Posteriormente, el programa entra en un bucle en donde utiliza recvfrom() para escuchar los mensajes que el broker le envía. Cada vez que llega un datagrama, lo imprime inmediatament.


- Publisher UDP

    Finalmente, cabe decir nuevamente que el publisher envia mensajes hacia el broker, para posteriormente enseñarlos en pantalla al usuario. INicialmente, al ejecutarse, el programa crea un socket UDP y configura la dirección del broker que es con el mismo puerto e IP de antes: 5000 y 192.168.64.3. Una vez conectado (sin establecer sesion debido al protocolo), el publisher entra en ciclo infinito, en el cual el usuario puede escribir cuantos mensajes quiera, para que luego estos se envien hacia el broker usando la funcion sendto().


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

    SE HACE ENTREGA DE LOS CODIGOS Y ARCHIVOS PCAP DENTRO DEL GIT. IGUALMENTE ESTA EL COMPRIMIDO DENTRO DE LA ENTREGA EN BLOQUE NEON.



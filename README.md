# Laboratorio-3-Redes---Grupo-6

A continuación se explicara el flujo de trabajo en lo que respecta los archivos .c
y la elaboracion de los codigos que permitieron el correcto desarrollo del laboratorio.

# TCP
- Broker TCP

    En este codigo se aprecia inicialmente, la definicion del puerto que se utilizara, el numero maximo de clientes
    permitidos y el maximo tamaño del buffer. Posteriormente se hace uso de la funcion DIE para imprimir error en caso de ocurrir algun fallo.

    Al iniciar, crea un socket y escucha en el puerto 8080, esperando conexiones entrantes. Cada cliente que se conecta , manda un carácter para indicar su rol: publisher si envia mensajes o subscriber si los recibe. En el codigo se crea una lista de todos los clientes, para que luego con la función select(), se detecte cuando llegan datos sin necesidad de usar varios hilos. Cuando un publisher envía un mensaje, este broker lo recibe y lo reenvía automáticamente a todos los subscribers conectados. No pasa nada si un cliente se desconecta, ya que este itera constantemente.

- Publisher TCP

    En este experimento, y como se sabe desde la parte de arquitectura, el publisher es el cliente emisor. Basicamente se conecta al broker TCP y manda los mensajes que este reenviará a los subscribers previamente definidos. Al ejecutarse, el programa crea un socket y se conecta a la dirección IP del broker, que por configuración dentro de la implementación, 127.0.0.1 en el puerto 8080. Una vez establecida la conexión, envía un carácter "P" para indicar su rol de publisher. Esta parte se definió previamente en la explicacion del broker. Luego entra en un bucle donde lee texto desde la consola y lo manda al broker usando TCP. Como se sabe, es gracias a este protocolo que los mensajes llegan de forma ordenada y segura. Ahora bien, una vez el usuario termina, el programa cierra la conexión y finaliza. 

- Subscriber TCP

    Como bien se sabe, el subscriber es el cliente receptor del sistema, encargado de recibir los mensajes que luego seran distribuidos a traves del broker. inicialmente, se crea un socket que se conecta al broker en el puerto 8080 con IP 127.0.0.1. Una vez establecida la conexión, envía un carácter "S" para aclarar su rol como subscriber. Luego entra en un bucle permanente donde espera mensajes provenientes del broker usando la función recv(). Ahora bien, cada vez que llega un mensaje, lo muestra directamente en pantalla, como bien fue apreciado dentro en el flujo de trabajo. Cabe decir que si el broker cierra la conexion o se interrumpe la comunicación, el programa lo detecta, muestra un aviso y finaliza. En pocas palabras, este cliente permanece a la escucha de publicaciones, funcionando como un receptor PASIVO que muestra en tiempo real los mensajes enviados.





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


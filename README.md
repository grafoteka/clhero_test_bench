# clhero_test_bench
# Banco de pruebas instrumentado para robots móviles terrestres
CLHeRo test bench source control

    @InProceedings{DeLeon2019,
    author    = {Jorge {De León} and José Ángel López and Jaime Del Cerro and Antonio Barrientos},
    title     = {Banco de pruebas instrumentado para robots móviles terrestres},
    booktitle = {Spanish Robotics Conference},
    year      = {2019},
    pages     = {126-133},
    month     = jun,
    keywords  = {test bench; clhero; },
    }

Dicho sea, previamente, para poner en funcionamiento el banco de pruebas móviles es necesario disponer del programa de Arduino IDE, así como de Ros y los paquetes que continuación se muestran:  

    control_cinta  

    escritor_txt  

    Rosserial  

    Vrpn-client_ros  

 

Para poner en funcionamiento el banco de pruebas se deben realizar los siguientes pasos:  

  1.- Encender el sistema optitrack y proceder a su calibración. 
  
  2.- Seleccionar en los cuatro elementos marcadores que se encuentran en las esquinas del banco de pruebas, y crear un objeto rígido con el nombre: "cinta". De igual manera se seleccionarán los tres o más marcadores que se encuentren sobre el robot y se convertirán en un objeto rígido con el nombre: "robot".  
  
  3.- En la plataforma de tracking tools, se debe figurar la comunicación con el ordenador dónde está instalado Ros, para lo cual se debe ir al menú principal /view/streaming Pane dentro de este menú se deberá tildar dentro de optitrack streaming engine la casilla broadcast frame data, y dentro de VRPN streaming engine la casilla broadcast frame data.  
  
  4.- Una vez configurada la comunicación con tracking tools y ya ubicados en el ordenador que contiene Ros se deben ejecutar los siguientes comandos:  
  
    4.1.- Este comando se encarga de realizar la comunicación entre el optitrack y el paquete vrpn. Consiguiéndose así que el ordenador qué contiene Ros disponga de la información que proporciona el hospital tanto en posición como en orientación. 
    
    $ roslaunch vrpn_client_ros sample.launch server:=192.168.1.2 

    4.2.- Este comando se encarga de leer el puerto serial del Arduino, obteniéndose así la comunicación entredicho Arduino y Ros. 
  
    $ rosrun rosserial_python serial_node.py 

    4.3.- Este comando permite al usuario ejecutar un código que guarda los datos de posición y velocidad del robot, así como el pwm que el motor tiene en cada momento entre 0 y 255. Los archivos que este código genera se guardan en la carpeta dónde se ubica el código: src/ escritor_txt/src. 

    $ rosrun escritor_txt escritor_txt  

    4.4.- Este comando ejecuta el programa asociado a la cinta, este programa se encarga de gestionar todo el movimiento, de la transformación de cuaternios (obtenidos del optitrack) a ángulos de Euler, y de del control PID y adaptativo.

    rosrun control_cinta control_cinta 
 
Además de todo esto podemos ejecutar también comandos que permiten visualizar el funcionamiento de la cinta, entre estos topics uno fundamental es el que muestra el comportamiento del pwm del motor en tiempo real, al que podemos acceder utilizando el comando: rostopic echo /pwm 

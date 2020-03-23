/********************************JALB********************************/
// Nombre: Escritor de datos
// Descripcion: A traves de este codigo se recopila las velocidades tanto del robot como de la cinta y se almacenan convenientemente en un fichero .txt para su tratamiento posterior.
// Autor: Jose Angel Lopez Belloso
/********************************JALB********************************/
#include "ros/ros.h"
#include "std_msgs/String.h"
#include <std_msgs/Int32.h>
#include "std_msgs/Float64.h"

#include <iostream>
#include <fstream>

using namespace std;
//using namespace ros;

ofstream ficheroSalida;

int velocidad_robot = 0;
int velocidad_cinta = 0;
float Diferencia_centro_robot = 0;


/********************************JALB********************************/
// Nombre: void cinta(const std_msgs::Int32& dato)
// Descripcion: Recibe los datos del topic "/pwm" y los pasa a una varialbe llamada velocidad_cinta.
// Valor devuelto: N/A
// Autor: Jose Angel Lopez Belloso
/********************************JALB********************************/
void cinta(const std_msgs::Int32& dato)
{
    velocidad_cinta = dato.data;
}


/********************************JALB********************************/
// Nombre: void robot(const std_msgs::Int32& dato)
// Descripcion: Recibe los datos del topic "/vel_robot" y los pasa a una varialbe llamada velocidad_robot.
// Valor devuelto: N/A
// Autor: Jose Angel Lopez Belloso
/********************************JALB********************************/
void robot(const std_msgs::Int32& dato)
{
    velocidad_robot = dato.data;
}


/********************************JALB********************************/
// Nombre: void diferencia(const std_msgs::Float64& dato)
// Descripcion: Recibe los datos del topic "/dif_pose" y los pasa a una varialbe llamada Diferencia_centro_robot.
// Valor devuelto: N/A
// Autor: Jose Angel Lopez Belloso
/********************************JALB********************************/
void diferencia(const std_msgs::Float64& dato)
{
    Diferencia_centro_robot = dato.data;
}

/********************************JALB********************************/
// Nombre: void escribir(int vel_cinta, int vel_robot)
// Descripcion: Recibe los datos las variables vel_cinta, vel_robot y dif_pose y lo escribe en un fichero.
// Valor devuelto: N/A
// Autor: Jose Angel Lopez Belloso
/********************************JALB********************************/
void escribir(int vel_cinta, int vel_robot, float dif_pose)
{
    //ofstream ficheroSalida;
    ficheroSalida.open("/home/oem/cinta_ws/src/escritor_txt/src/datos.txt", ios::app);
    ficheroSalida << vel_robot << "\t" << vel_cinta << "\t" << dif_pose << endl;
    ficheroSalida.close();

}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "escritor_txt");
    ros::NodeHandle nh;

    printf("vel_cinta %d   vel_robot %d  diferencia: %f \n", velocidad_cinta, velocidad_robot,Diferencia_centro_robot);
    escribir(velocidad_cinta, velocidad_robot, Diferencia_centro_robot);

    ros::Subscriber sub1 = nh.subscribe("/pwm", 1, cinta);
    ros::Subscriber sub2 = nh.subscribe("/vel_robot", 1, robot);
    ros::Subscriber sub3 = nh.subscribe("/dif_pose", 1, diferencia);

    while (ros::ok())
    {
         printf("vel_cinta %d   vel_robot %d  diferencia: %f \n", velocidad_cinta, velocidad_robot,Diferencia_centro_robot);
        escribir(velocidad_cinta, velocidad_robot, Diferencia_centro_robot);
        ros::Duration(0.1).sleep();
        ros::spinOnce();
    }

    ROS_INFO("Cierro archivo");
    ficheroSalida.close();
    return 0;
}


/********************************JALB********************************/
// Nombre: Control de la cinta
// Descripcion: A traves de este codigo se recopila la informacion proveniente del optitrack, y se trasforma y adapta para tener el PID que mantiene al robot en todo momneto en el centro de la cinta independientemente de sus cambios de velocidad.
// Autor: Jose Angel Lopez Belloso
/********************************JALB********************************/

#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sstream>
#include "geometry_msgs/PoseStamped.h"
#include "std_msgs/Float64.h"
#include "std_msgs/Header.h"

using namespace std;

const double PI = 3.141592653;
double error_old = 0;	//Inicializacion del error antiguo a 0.

float dt = 3;	//valor incial variacion de tiempo.


ros::Publisher Publicador1;
ros::Publisher Publicador2;
geometry_msgs::PoseStamped pose_cinta;
geometry_msgs::PoseStamped pose_robot;
ros::Time old_time;
ros::Time last_time;

//Estructura del  cuaternio
struct cuaternio {
  double x;
  double y;
  double z;
  double w;
};
using namespace std;

/********************************JALB********************************/
// Nombre: double PID(double referencia, double diferencia, double & error_old)
// Descripcion: Rebide el valor de referencia que se quiere alcanzar para el PID (que la distancia entre el centro de la cinta y el robot sea 0) y la distancia entre el robot y el dentro de la cinta.
// Valor devuelto: salida_vel (valor optenido para la posicion siguiente del PID)
// Autor: Jose Angel Lopez Belloso
/********************************JALB********************************/
double PID(double referencia, double diferencia, double  & error_old) {
  double Ki = 0.01;
  double Kd = 0.05;
  double Kp = 50;
  double max = 100;
  double min = -100;
  double integral = 0;

  // Error nuevo: referencia (0) menos la diferencia de las posiciones
  double error = referencia - diferencia;

  // Termino proporcional
  double Pout = Kp * error;

  // Termino integral
  integral += error * dt;
  //double Iout = Ki * integral;

  // termino derivativo
  double derivative = (error - error_old) / dt;
  double Dout = Kd * derivative;

  // Consigna de velocidad de salida del PID
  double salida_vel = Pout + Iout + Dout;
    double salida_vel = Pout;

  // Restricciones de valor maximo y minimo de salida
  //if (salida_vel > max)
   // salida_vel = max;
  //else if (salida_vel < min)
  //  salida_vel = min;

  // Guardar el error antiguo.
  error_old = error;
  return salida_vel;
}


/********************************JALB********************************/
// Nombre: void angulo_euler(cuaternio q, double & roll, double & pitch, double & yaw)
// Descripcion: Rebide el las cuatro coordenadas correspondientes a los angulos en cuaternio, y los trasforma en los tres angulos de euler: roll, pitch y yaw.
// Valor devuelto:  N/A
// Autor: Jose Angel Lopez Belloso
/********************************JALB********************************/
void angulo_euler(cuaternio q, double & roll, double & pitch, double & yaw) {
    // roll (x-axis rotation)
    double sin_roll = +2.0 * (q.w * q.x + q.y * q.z);
    double cos_roll = +1.0 - 2.0 * (q.x * q.x + q.y * q.y);
    roll = (atan2(sin_roll, cos_roll)) * -1; //* -1 valor por defecto !!

    // pitch (y-axis rotation)
    double sin_pitch = +2.0 * (q.w * q.y - q.z * q.x);
    if (fabs(sin_pitch) >= 1)
        pitch = copysign(PI / 2, sin_pitch);
    else
        pitch = asin(sin_pitch);

    // yaw (z-axis rotation)
    double sin_yaw = +2.0 * (q.w * q.z + q.x * q.y);
    double cos_yaw = +1.0 - 2.0 * (q.y * q.y + q.z * q.z);
    yaw = atan2(sin_yaw, cos_yaw);
}


/********************************JALB********************************/
// Nombre: void callback_cinta(const geometry_msgs::PoseStamped::ConstPtr & msg)
// Descripcion: Recibe los datos del topic "/vrpn_client_node/cinta/pose" y los pasa a una varialbe llamada pose_cinta.
// Valor devuelto: N/A
// Autor: Jose Angel Lopez Belloso
/********************************JALB********************************/
void callback_cinta(const geometry_msgs::PoseStamped::ConstPtr & msg) {
    pose_cinta = *msg;
    //posicion de la cinta variable global (pose_cinta).
    //fin callback
}


/********************************JALB********************************/
// Nombre: void callback_robot(const geometry_msgs::PoseStamped::ConstPtr & msg)
// Descripcion: Recibe los datos del topic "/vrpn_client_node/robot/pose" imprime por pantalla la informacion relevante ademas de llamar a la funcion que calcula el PID, y a la funcion que devuelve el tiempo entre iteraciones.
// Valor devuelto: N/A
// Autor: Jose Angel Lopez Belloso
/********************************JALB********************************/

void callback_robot(const geometry_msgs::PoseStamped::ConstPtr & msg) {
    double zPos_r;
    double zPos;
    double roll, pitch, yaw;
    struct cuaternio q;
    double tiempo;
    zPos_r = msg->pose.position.z;
    //  zPos = msg->pose.position.z;

    tiempo = msg->header.seq;
    angulo_euler(q, roll, pitch, yaw);

    printf("the position value of X is %lf m and %lf cm \n ", zPos_r, pose_cinta.pose.position.z);

    double diferencia = zPos_r - pose_cinta.pose.position.z;
    double referencia = 0;

    last_time = ros::Time::now();	// empiezo a contar el tiempo

    double salida_vel = PID(referencia, diferencia, error_old);	//calculo el PID
    ros::Duration(0.1).sleep();	//espero 3 segundos entre muestras
    old_time = ros::Time::now();	//cuento el tiempo
    ros::Duration diff = old_time - last_time;	// resto los tiempos

    //trasformo el tiempo diff de tipo duracion en un float.
    int sec = diff.sec;
    int nano_sec = diff.nsec;
    dt = (nano_sec * pow(10, -9)) + sec;
    printf("\nRefresh time: %3.10f\n", dt);

    printf("offset:% 7.10f speed_out:% 7.10f\n\n", diferencia, salida_vel);
    diferencia += salida_vel;
    std_msgs::Float64 consigna_vel;
    std_msgs::Float64 dif_pose;

    dif_pose.data = diferencia;
    consigna_vel.data = salida_vel;

    //Publicador de consigna.
    Publicador1.publish(consigna_vel);
    Publicador2.publish(dif_pose);
}

int main(int argc, char * * argv) {
    ros::init(argc, argv, "nodo_cinta");
    //nodo cinta
    ros::NodeHandle n;

    Publicador1 = n.advertise < std_msgs::Float64 >("/consig_vel", 1000);	//publicador
    Publicador2 = n.advertise < std_msgs::Float64 >("/dif_pose", 1000);	//publicador
    ros::Subscriber sub_cinta = n.subscribe("/vrpn_client_node/cinta/pose", 1, callback_cinta);	//Suscriptor cinta
    ros::Subscriber sub_robot = n.subscribe("/vrpn_client_node/robot/pose", 1, callback_robot);	//Suscriptor robot


    ros::Rate loop_rate(10);
    ros::spin();
    return 0;
}

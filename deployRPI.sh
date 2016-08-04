#!/bin/bash

# @file deployRPI.sh
# @author Pablo Ingaramo
# @date 31 may 2015
# @brief script que copia el entorno de trabajo a la RPI, compila en compilador ARM y hace el deploy en la misma (compilar.sh)
# @params $1:direccion ip de la RPI, 

IP=$1
echo "Eliminando archivos anteriores" 
ssh pi@"$IP" 'sudo service mysql stop'
ssh pi@"$IP" 'sudo service SCAIService stop'
ssh pi@"$IP" 'sudo rm /home/pi/code/webserver/cgi-bin/*.cgi'
ssh pi@"$IP" 'sudo rm /home/pi/code/webserver/cgi-bin/*.fcgi'
ssh pi@"$IP" 'sudo rm -r /home/pi/code/webserver/html/*'
ssh pi@"$IP" 'sudo rm -r /var/www/pi/cgi-bin/*'
ssh pi@"$IP" 'sudo rm -r /var/www/pi/html/*'
ssh pi@"$IP" 'sudo rm -r /home/pi/code/javadaemon/runnableJar/*'
echo "Transfiriendo archivos..."
scp -r /home/paul/code/RPI2/webserver/* pi@"$IP":/home/pi/code/webserver/
scp -r /home/paul/code/RPI2/compilar.sh pi@"$IP":/home/pi/code/
scp -r /home/paul/code/RPI2/startVNC.sh pi@"$IP":/home/pi/code/
scp -r /home/paul/code/RPI2/makefile pi@"$IP":/home/pi/code/
scp -r /home/paul/code/RPI2/javadaemon/runnableJar/* pi@"$IP":/home/pi/code/javadaemon/runnableJar/
scp -r /home/paul/code/RPI2/BerryIMU pi@"$IP":/home/pi/code/

ssh pi@"$IP" 'sudo cp /home/pi/code/javadaemon/runnableJar/SCAIService /etc/init.d/'
ssh pi@"$IP" 'sudo chmod +x /etc/init.d/SCAIService'


echo "Compilando y deployando en RPI..."
ssh pi@"$IP" 'sudo make -C /home/pi/code/webserver/cgi-bin/imu_query_src'
ssh pi@"$IP" 'sudo cp /home/pi/code/webserver/cgi-bin/imu_query_src/imuQuery.cgi /home/pi/code/webserver/cgi-bin/'
ssh pi@"$IP" 'sudo make -C /home/pi/code/webserver/cgi-bin/gps_query_src'
ssh pi@"$IP" 'sudo cp /home/pi/code/webserver/cgi-bin/gps_query_src/gpsQuery.fcgi /home/pi/code/webserver/cgi-bin/'
ssh pi@"$IP" 'sudo usermod -a -G i2c www-data' 
ssh pi@"$IP" 'sudo /home/pi/code/compilar.sh /home/pi/code/'

ssh pi@"$IP" 'sudo service SCAIService start'



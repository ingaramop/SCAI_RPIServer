#!/bin/bash

# @file deployRPI.sh
# @author Pablo Ingaramo
# @date 31 may 2015
# @brief script que copia el entorno de trabajo a la RPI, compila en compilador ARM y hace el deploy en la misma (compilar.sh)
# @params $1:direccion ip de la RPI, 

IP=$1
echo "Transfiriendo archivos..."
scp -r /home/paul/code/RPI2/* pi@"$IP":/home/pi/code/
echo "Eliminando archivos anteriores" 
ssh pi@"$IP" 'sudo rm /home/pi/code/webserver/cgi-bin/*.cgi'
ssh pi@"$IP" 'sudo rm /home/pi/code/webserver/cgi-bin/*.fcgi'
ssh pi@"$IP" 'sudo rm /home/pi/code/webserver/html/*'
ssh pi@"$IP" 'sudo rm /var/www/pi/cgi-bin/*'
ssh pi@"$IP" 'sudo rm /var/www/pi/html/*'
echo "Compilando y deployando en RPI..."
ssh pi@"$IP" 'sudo make -C /home/pi/code/webserver/cgi-bin/imu_query_src'
ssh pi@"$IP" 'sudo cp /home/pi/code/webserver/cgi-bin/imu_query_src/imuQuery.cgi /home/pi/code/webserver/cgi-bin/'
ssh pi@"$IP" 'sudo /home/pi/code/compilar.sh /home/pi/code/'
ssh pi@"$IP" 'sudo usermod -a -G i2c www-data' 


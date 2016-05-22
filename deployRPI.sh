#!/bin/bash

# @file deployRPI.sh
# @author Pablo Ingaramo
# @date 31 may 2015
# @brief script que copia el entorno de trabajo a la RPI, compila en compilador ARM y hace el deploy en la misma (compilar.sh)
# @params $1:direccion ip de la RPI, 

IP=$1
echo "Transfiriendo archivos..."
scp -r /home/paul/code/RPI2/* pi@"$IP":/home/pi/code/
echo "Compilando y deployando en RPI..."
ssh pi@"$IP" 'sudo /home/pi/code/compilar.sh /home/pi/code/'

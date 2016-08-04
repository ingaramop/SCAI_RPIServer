Los codigos fuente y archivos de configuracion se encuentran en esta carpeta.
El script "compile.sh" realiza la compilacion de los mismos.
El script "deploy.sh" los coloca en los directorios correspondientes y reinicia los servicios de modo que queden funcionando.

DIRECTORIOS APACHE
https://www.digitalocean.com/community/tutorials/how-to-configure-the-apache-web-server-on-an-ubuntu-or-debian-vps
/etc/apache2 -> PRINCIPALES ARCHIVOS DE CONFIGURACION
/etc/apache2/sites-available/ -> ARCHIVOS DE CONFIGURACION INDIVIDUAL DE CADA UNO DE LOS SITIOS
/var/log/apache2 -> LOGS

CONFIGURACION FASTCGI
http://stackoverflow.com/questions/14596199/run-a-c-program-on-a-linux-server
1) sudo apt-get install libfcgi-dev -> INSTALA EL PAQUETE DE FASTCGI
2) sudo apt-get install libapache2-mod-fcgid -> INSTALA EL MODULO FCGI DE APACHE
3) sudo a2enmod fcgid (en carpeta /etc/apache2/mods-available) -> HABILITA EL MODULO
4) sudo gcc example.c -lfcgi -o /usr/lib/cgi-bin/example.fcgi -> COMPILA PROGRAMAS EN C CON FCGI
5) sudo service apache2 restart -> REINICIA EL SERVICIO APACHE2 PARA APLICAR LOS CAMBIOS

CONFIGURACION SSH
Para poder realizar deploys automaticos en la RPI2, necesitamos configurar un login sin password, sino a traves de par clave publica/privada
Los pasos para esto son los siguientes:
https://www.raspberrypi.org/documentation/remote-access/ssh/passwordless.md
1) ssh-keygen -t rsa -> GENERA PAR DE CLAVES EN LA COMPUTADORA CLIENTE (DEFAULT EN /home/paul/.ssh/id_rsa)
2) install -d -m 700 /home/pi/.ssh -> CREO EL DIRECTORIO .ssh EN LA RPI, PARA PODER PONER LA CLAVE PUBLICA
3) cat ~/.ssh/id_rsa.pub | ssh <USERNAME>@<IP-ADDRESS> 'cat >> .ssh/authorized_keys' -> COPIO LA CLAVE PUBLICA DESDE EL CLIENTE A LA RPI
4) ssh <USERNAME>@<IP-ADDRESS> -> DEBERIA INGRESAR POR SSH CON ESTE COMANDO, SIN PASSWORD

CONFIGURACION htaccess + htpasswd (CONTROL DE ACCESO AL CONTENIDO WEB)
https://www.digitalocean.com/community/tutorials/how-to-set-up-password-authentication-with-apache-on-ubuntu-14-04
1) sudo nano /etc/apache2/apache2.conf -> MODIFICA LA CONFIGURACION DEL DIRECTORIO RAIZ DONDE SE ENCUENTRE EL SITIO:
		<Directory /var/www/>
			Options Indexes FollowSymLinks
			AllowOverride All
			Require all granted
		</Directory>
2) sudo nano /var/www/html/.htaccess -> CREAR EL ARCHIVO:
		AuthType Basic
		AuthName "Restricted Content"
		AuthUserFile /etc/apache2/.htpasswd
		Require valid-user
3) sudo apt-get install apache2 apache2-utils -> INSTALA LAS UTILIDADES APACHE, QUE INCLUYEB httpsswd
4) sudo htpasswd -c /etc/apache2/.htpasswd <USER NAME> -> CREA EL ARCHIVO DE PASSWORD
5) sudo service apache2 restart -> REINICIA EL SERVICIO
6) (user: admin passwd: raspberry)

INSTALACION SISTEMA OPERATIVO
https://www.raspberrypi.org/downloads/raspbian/ -> DESCARGA DE RASPBIAN JESSE LITE
sha1sum <NOMBRE ARCHIVO> -> COMPRUEBA EL CHECKSUM Y LA CORRECTA DESCARGA DE LA IMAGEN
https://www.raspberrypi.org/documentation/installation/installing-images/linux.md -> MONTAR RASPBIAN EN TARJETA SD

FORMULARIOS EN PHP
sudo apt-get install php5 libapache2-mod-php5 php5-mcrypt -> INSTALAR PHP
http://www.phpform.org/

CONFIGURACION DIRECTORIOS EN RPI (PREVIO AL PRIMER DEPLOY)
1) DESHABILITAR TODOS LOS SITIOS QUE SE ENCUENTREN HABILITADOS EN /etc/apache2/sites-enabled, utilizando el comando a2dissite <NOMBRE SITIO>
2) sudo rm /var/www/*
3) sudo mkdir /var/www/pi
4) sudo mkdir /var/www/pi/html -> AQUI SE COLOCAN LOS SITIOS WEB HTML
5) sudo mkdir /var/www/pi/cgi-bin -> AQUI SE COLOCAN LOS SCRIPTS PUBLICOS
6) sudo mkdir /home/pi/code -> AQUI SE COLOCAN LOS CODIGOS DE FUENTE CUANDO SE HACE LA TRANSFERENCIA REMOTA POR SSH

CONFIGURACION DEL SERVIDOR VNC
https://www.raspberrypi.org/documentation/remote-access/vnc/

CONFIGURACION DEL WIRELESS DESDE COMMAND LINE
https://www.raspberrypi.org/documentation/configuration/wireless/wireless-cli.md

SENSOR BERRYIMU
http://ozzmaker.com/product/berryimu-accelerometer-gyroscope-magnetometer-barometricaltitude-sensor/
	ACTIVAR LA INTERFAZ I2C EN RPI2:
	http://ozzmaker.com/i2c/
	1) sudo apt-get update 
	2) sudo apt-get install i2c-tools libi2c-dev python-smbus -> INSTALA LOS PAQUETES NECESARIOS DE I2C
	3) sudo nano /etc/modules -> AGREGAR LAS SIGUIENTES LINEAS AL FINAL PARA QUE CARGUE LOS MODULOS I2C AL INICIO DEL OS:
	i2c-dev
	i2c-bcm2708
	4) sudo nano /boot/config.txt -> AGREGAR LO SIGUIENTE:
	dtparam=i2c_arm=on
	dtparam=i2c1=on
	5) sudo reboot -> REINICIAR LA RPI PARA APLICAR LOS CAMBIOS
	6) sudo i2cdetect -y 1 -> DETECTA AUTOMATICAMENTE LOS DISPOSITIVOS CONECTADOS POR I2C Y LOS VISUALIZA

	INTERFACEAR GYRO Y ACELEROMETRO CON LA RPI
	http://ozzmaker.com/berryimu/
	(falta completar)
	Una vez creados los archivos .cgi que consultan el IMU, para poder ejecutarlos desde apache, debo otorgarle permisos al usuario de apache (www-data) para que pueda acceder al puerto i2c. Esto se hace como sigue:
		- Edito el archivo /etc/group y modifico la linea: i2c:x:998:pi,www-data
		- ver https://www.raspberrypi.org/forums/viewtopic.php?f=91&t=113549
		- http://www.sunspot.co.uk/Projects/raspberrypi_LAN_master/webpage_CGI_control/CGI_control.html

CREAR BACKUP SD CARD
https://thepihut.com/blogs/raspberry-pi-tutorials/17789160-backing-up-and-restoring-your-raspberry-pis-sd-card
	1)


INSTALACION MYSQL
https://www.stewright.me/2014/06/tutorial-install-mysql-server-on-raspberry-pi/
	1) sudo apt-get install mysql-server mysql-client php5-mysql -y -> Instala mysql server, mysqlclient y su modulo PHP
	root password: raspberry
	2) mysql -uroot -hlocalhost -p -> Open mysql and login
	3) CREATE DATABASE scai; -> Crea base de datos
	4) CREATE USER 'pi'@'localhost' IDENTIFIED BY 'raspberry'; -> Creo usuario: pi pass: raspberry
	5) GRANT ALL PRIVILEGES ON scai.* TO 'pi'@'localhost'; -> Doy permisos al usuario sobre la nueva db
	6) FLUSH PRIVILEGES; -> Refresco privilegiso para que hagan efecto los cambios
	7) mysql -upi -hlocalhost scai -p -> Salgo y vuelvo a ingresar con el nuevo usuario
	8) use scai -> Pongo en uso la nueva db
	9) create table history (
id int not null auto_increment primary key,
date timestamp default now(),
tipperInclination int,
sideInclination int,
compass int,
temperature double,
pressure double,
positionX double,
positionY double,
timestamp bigint,
image blob);

	Select ALL: select id,date,tipperInclination,sideInclination ,compass,temperature,pressure,positionX,positionY,timestamp from history;
	Delete ALL but latest X: DELETE from history WHERE id < (SELECT id FROM (SELECT id FROM history ORDER BY id DESC LIMIT 50) t ORDER BY id ASC LIMIT 1); 

	11) To configure Java Eclipse: Right Click the project -- > build path -- > configure build path. In Libraries Tab press Add External Jar and Select your jar.


GPS
	1) sudo raspi-config -> Disable serial console
	2) sudo systemctl disable serial-getty@ttyAMA0.service -> Disable serial (this might be redundant)
	3) sudo nano /boot/config.txt -> set 'enable_uart=1'
	4) sudo apt-get install gpsd gpsd-clients -> Installs daemon gps receiver more: http://catb.org/gpsd/
	5) Configure gpsd:
		sudo nano /etc/default/gpsd
			START_DAEMON="true"
			GPSD_OPTIONS="n"
			DEVICES="/dev/ttyAMA0"
			USBAUTO="false"
			GPSD_SOCKET="/var/run/gpsd.sock"
	5) sudo reboot
	6) stty -F /dev/ttyAMA0 raw 9600 cs8 clocal -cstopb -> Configures serial device so that it can be listened in ttyAMA0
	7) cat /dev/ttyAMA0 -> check if it is coming data from uart
	8) gpsmon /dev/ttyAMA0 (or cgps -s) -> Real-time console gps viewer
	9) sudo apt-get install libgps-dev -> Libraries necessary to programmatically access gpsd

	troubleshoot:
		- https://www.raspberrypi.org/forums/viewtopic.php?f=28&t=128028
		- https://www.raspberrypi.org/forums/viewtopic.php?f=45&t=18115
		





DATABASE UPDATER SERVICE
http://www.jcgonzalez.com/linux-java-service-wrapper-example
https://www.raspberrypi.org/documentation/linux/usage/rc-local.md
	1) El programa java Daemon.jar se encuentra en /home/pi/code/javadaemon/runnableJar/Daemon.jar
	2) En el mismo directorio se coloca un script wrapper ServiceStarter que se encarga de que funcione como servicio
	3) Cuando se ejecuta Deploy.sh, ServiceStarter es copiado al directorio /etc/init.d/
	4) Deploy.sh además le da permisos de ejecución sudo chmod +x /etc/init.d/ServiceStarter
	5) Finalmente, se puede ejecutar los comandos:
		Test that it runs ex. sudo service mytestserv start
		Test that it stops ex. sudo service mytestserv stop
		Test that it restarts ex. sudo service mytestserv restart
	6) In Ubuntu use sudo update-rc.d mytestserv defaults if you want to run the service when the SO starts or sudo update-rc.d mytestserv disable to remove from startup.



		
		





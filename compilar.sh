
#!/bin/bash

# @file compilar.sh
# @author Pablo Ingaramo
# @date 31 feb 2016
# @brief script de automatizacion para el desarrollador. Compila, acomoda archivos, elimina innecesarios, reinicia el servicio apache. NO EJECUTAR EN OTRA MÁQUINA.
# @params $1 direccion absoluta de la carpeta donde estan los archivos del codigo (si se lo esta llamando desde afuera del directorio)


DIR=$1 # parametro con el directorio donde se encuentra el codigo del proyecto. Si está vacío, lo busca en el directorio donde se está parado al momento de ejecutar este script.
cd "$DIR"
echo "Print PWD..."

echo "Deteniendo servicio apache2..."
service apache2 stop

echo "Compilando..."
make
echo "Acomodando archivos..."
rm -f webserver/cgi-bin/*.o
cp webserver/sites-available/default.conf /etc/apache2/sites-available/
cp -R webserver/cgi-bin/*.fcgi /var/www/pi/cgi-bin/
cp -R webserver/cgi-bin/*.cgi /var/www/pi/cgi-bin/
cp -R webserver/cgi-bin/*.sh /var/www/pi/cgi-bin/
cp -R webserver/cgi-bin/systemInfo.cgi /var/www/pi/html/
cp -R webserver/html/* /var/www/pi/html/
#cp webserver/apache2.conf /etc/apache2/
cp webserver/passwd/htpasswd /etc/apache2/.htpasswd
cp webserver/passwd/htaccess /var/www/pi/html/.htaccess
echo "Dando permisos..."
chmod -R 755 /var/www/pi/cgi-bin

echo "Habilitando modulos..."
a2enmod cgid.load
a2enmod fcgid.load

echo "Habilitando sitio..."
a2ensite default.conf

echo "Iniciando servicio apache2..."
service apache2 start

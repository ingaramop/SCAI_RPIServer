/**
 * @file systeminfo.c
 * @author Pablo Ingaramo
 * @date 31 may 2015
 * @brief Accede a los archivos uptime, version, cpuinfo y meminfo del /proc, les da formato e imprime su contenido.
 */

#include <stdio.h>
#include <stdlib.h>
 
int main(void)
{
    FILE *file1;
	char buffer[100];
	printf( "Content-Type: text/plain\n\n" );
	
	printf("\n*****TIEMPO*****\n");
    file1 = fopen("//proc//uptime", "r");
    if (file1 == NULL)
    {
        perror("file1");
        exit(1);
    }	
	float uptime, idletime;
	int uptime_i, idletime_i;
	fscanf(file1, "%f %f", &uptime, &idletime);
	uptime_i = (int) uptime;
	idletime_i = (int) idletime;
    printf("Uptime: %dseg = %dd%dh%dm%ds\n", uptime_i,(uptime_i/86400), ((uptime_i%86400)/3600), ((uptime_i%1440)/60), (uptime_i%60));
	printf("Idletime: %dseg = %dd%dh%dm%ds\n", idletime_i,(idletime_i/86400), ((idletime_i%86400)/1440), ((idletime_i%1440)/60), (idletime_i%60));
    fclose(file1);
	
    printf("\n*****SISTEMA OPERATIVO*****\n");
    file1 = fopen("//proc//version", "r");
    if (file1 == NULL)
    {
        perror("file1");
        exit(1);
    }
    while (fgets(buffer, sizeof(buffer), file1) != NULL)
        printf("%s\n", buffer);
    fclose(file1);
	
	    printf("\n*****CPU*****\n");
    file1 = fopen("//proc//cpuinfo", "r");
    if (file1 == NULL)
    {
        perror("file1");
        exit(1);
    }   
    while (fgets(buffer, sizeof(buffer), file1) != NULL)
        printf("%s", buffer);
    
    fclose(file1);
	
		    printf("\n*****MEMORIA*****\n");
    file1 = fopen("//proc//meminfo", "r");
    if (file1 == NULL)
    {
        perror("file1");
        exit(1);
    }   
    while (fgets(buffer, sizeof(buffer), file1) != NULL)
        printf("%s", buffer);
    
    fclose(file1);
	
	
    return 0;
}
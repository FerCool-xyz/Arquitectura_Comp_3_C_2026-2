/*Programa que calcula e imprime secuencialmente miembros de la serie de fibonacci
Fernando Vicente Munoz ISC 3C, Arquitectura de computadoras
Actividad 1*/

#include <stdio.h>

int main(void)
{
	 int actual = 0, sig = 1, suma = 0, num_limite = 0; //Enteros que utilizaremos para el programa

	printf("Hasta que numero de miembro quieres llegar: ");
	scanf("%d",&num_limite);

	if(num_limite < 1)
	{
		printf("No se vale valores menores a uno.\n");
		return 1;
	}

	for (int i = 0; i <= num_limite; i++)
	{
		printf("Miembro %d: %d\n", i, actual);
		suma = actual + sig; /*Guardamos en variable suma para poder sustituir actual por sig sin perder 
		la secuencia*/
		actual = sig;
		sig = suma;
	}

	printf("Se ha terminado el programa exitosamente!!!\n");
	return 0;
}


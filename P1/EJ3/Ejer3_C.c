/*
 *programa Ejer3_C.c
 * Diseñado para 4 procesos en total. 
 * Puede funcionar con mayores dimensiones de array si se indica en la constante.
 *
 * Para que la carga de trabajo sea distribuida, cuando un proceso termine su cómputo, lo mandará al siguiente,
 * si existiesen mas procesos esclavos evidentemente. La idea es que un determinado proceso, además de calcular 
 * su correspondiente suma, no deba calcular también la del anterior para hacer la diferencia y obtener la suma correcta que se 
 * debe mandar al master. La otra opción sería que el master hiciese dichas diferencias entre las sumas que recibe pero no sería 
 * muy eficiente a gran escala.
 *
 *
 *
 */


#include <stdio.h>
#include <string.h>
#include <mpi.h>
#include <stdlib.h>

#define SIZE_ARRAY 9 
#define NUM_PROC 3

int main(int argc, char **argv){
	int me; //rank of process
	int p; //number of processes
	MPI_Status status; //return status

	
	MPI_Init (&argc, &argv); //start rup MPI
	MPI_Comm_rank (MPI_COMM_WORLD, &me); //get process rank
	MPI_Comm_size (MPI_COMM_WORLD, &p); //get number of processes
	
	
	if (me == 3){ //master
	/*PSEUDOCODIGO 1:
		1. Recibir tantos resultados como procesos haya
		2. Hacer la suma conjunta
		3. Imprimir los resultados
		*/
	
	int suma_global = 0;
	int suma_individual = 0;
	
	for (int i = 0; i < NUM_PROC; i++){
		MPI_Recv(&suma_individual, 1, MPI_INT, i, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		suma_global += suma_individual;
		
		printf ("Suma individual recibida de proceso esclavo: %d es = %d\n", status.MPI_SOURCE, suma_individual);
	}
	printf ("El resultado global de la suman es: %d\n", suma_global);
	
	
	
	} else { //slave
		
	
	/*PSEUDOCODIGO 1:
		1. Inicializar array entero (N/p-1)
		2. Seleccionar la zona que haya tocado sumar
		3. Enviar la suma al master
		4. Mensajes por pantalla para debug
		
	*/
	
	int reparto = SIZE_ARRAY / NUM_PROC;
	if (me == 0){ //primer esclavo
		int suma = 0;
		
		//computa
		suma = ((reparto - 1)*reparto) / 2;
		
		MPI_Send (&suma, 1, MPI_INT, me+1, me, MPI_COMM_WORLD); //enviar a esclavo +1
		MPI_Send (&suma, 1, MPI_INT, 3, me, MPI_COMM_WORLD); //enviar a master
		printf ("Sender: %d acaba de terminar.\n", me);
		
	
	} else if (me == 1) { //segundo escalvo
		int suma = 0;
		int suma_anterior = 0;
		int N = 0;
		
		//suma de esclavo -1
		MPI_Recv(&suma_anterior, 1, MPI_INT, me-1 , MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		
		//computa
		N = (me + 1) * reparto;
		suma = (((N-1) * N) / 2);
		MPI_Send (&suma, 1, MPI_INT, me+1, me, MPI_COMM_WORLD); //enviar a esclavo suma de N primeros números naturales
		
		suma = suma - suma_anterior;
		MPI_Send (&suma, 1, MPI_INT, 3, me, MPI_COMM_WORLD); //enviar a master suma correcta
		printf ("Sender: %d acaba de terminar.\n", me);
		
	} else{ //tercer esclavo me==2
		int suma = 0;
		int suma_anterior = 0;
		int N = 0;
		
		//suma de esclavo -1
		MPI_Recv(&suma_anterior, 1, MPI_INT, me-1 , MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		
		//computa
		N = (me + 1) * reparto;
		suma = (((N-1) * N) / 2) - suma_anterior;
		
		MPI_Send (&suma, 1, MPI_INT, 3, me, MPI_COMM_WORLD); //enviar a master
		printf ("Sender: %d acaba de terminar.\n", me);
		
	}
	  
	  
	}
	
	MPI_Finalize(); //shutdown MPI
	return 0;
}


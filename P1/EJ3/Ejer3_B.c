/*
 *programa Ejer3_B.c
 * Diseñado para 4 procesos en total. 
 * Puede funcionar con mayores dimensiones de array si se indica en la constante.
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
		int *array = malloc (reparto * sizeof(int)); 
		
		//inicializamos array de dimension N/p-1
		for (int i = 0; i < reparto; i++){
			array[i] = me*reparto +i;
		}
		
		//computa
		for (int i = 0; i < reparto; i++){
			suma += array[i];
		}
		MPI_Send (&suma, 1, MPI_INT, 3, me, MPI_COMM_WORLD); //enviar suma
		printf ("Sender: %d acaba de terminar.\n", me);
		free (array);
		
		
	
	} else if (me == 1) { //segundo escalvo
		int suma = 0;
		int *array = malloc (reparto * sizeof(int)); 
		
		//inicializamos array de dimension N/p-1
		for (int i = 0; i < reparto; i++){
			array[i] = me*reparto +i;
		}
		
		//computa
		for (int i = 0; i < reparto; i++){
			suma += array[i];
		}
		MPI_Send (&suma, 1, MPI_INT, 3, me, MPI_COMM_WORLD); //enviar suma
		printf ("Sender: %d acaba de terminar.\n", me);
		free (array);
		
	
	} else{ //tercer esclavo me==2
		int suma = 0;
		int *array = malloc (reparto * sizeof(int)); 
		
		//inicializamos array de dimension N/p-1
		for (int i = 0; i < reparto; i++){
			array[i] = me*reparto +i;
		}
		
		//computa
		for (int i = 0; i < reparto; i++){
			suma += array[i];
		}
		MPI_Send (&suma, 1, MPI_INT, 3, me, MPI_COMM_WORLD); //enviar suma
		printf ("Sender: %d acaba de terminar.\n", me);
		free (array);
		
		
	}
	  
	  
	}
	
	MPI_Finalize(); //shutdown MPI
	return 0;
}


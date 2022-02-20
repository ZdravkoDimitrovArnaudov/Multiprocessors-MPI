/*
 *programa Ejer3_A.c
 * Diseñado para 4 procesos en total. 
 * Puede funcionar con mayores dimensiones de array si se indica en la constante.
 * 
 * Hay dos versiones del programa:
 	1. La que está comentada, es donde los slaves inicializan ellos mismos el array (directo)
 	2. El master envía a los esclavos el array inicializado (Buffereado)
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
		//REF: MPI_Recv(integer_buffer_receive, buffer_size, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		
	/*PSEUDOCODIGO 1:
		1. Recibir tantos resultados como procesos haya
		2. Hacer la suma conjunta
		3. Imprimir los resultados
	
	int suma_global = 0;
	int suma_individual = 0;
	
	for (int i = 0; i < NUM_PROC; i++){
		MPI_Recv(&suma_individual, 1, MPI_INT, i, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		suma_global += suma_individual;
		printf ("Suma individual recibida de proceso esclavo: %d es = %d\n", status.MPI_SOURCE, suma_individual);
	}
	printf ("El resultado global de la suman es: %d\n", suma_global);
	
	*/
		
	  /*PSEUDOCODIGO 2:
	  	1. Inicializar el array
	  	2. Enviar el array al resto de procesos
	  	3. Esperar a los resultados
	  	4. Sumarlos e imprimirlos.	
	 */
	 
	int suma_global = 0;
	int suma_individual = 0;
	 
	int b_size = (MPI_BSEND_OVERHEAD + (SIZE_ARRAY*sizeof(int)));
        char* buffer = malloc(b_size);
	MPI_Buffer_attach(buffer, b_size); //buffer attach
	
	//inicializamos array
	int *array = malloc (SIZE_ARRAY * sizeof(int));
	for (int i = 0; i < SIZE_ARRAY; i++){
		array[i] = i;
	
	}
	
	//enviamos array a los slaves
	for (int i = 0; i < NUM_PROC; i++){
		MPI_Bsend (array, SIZE_ARRAY, MPI_INT, i, me, MPI_COMM_WORLD);
		printf ("Master ha enviado array a slave: %d\n", i);
	}
	
	//recibimos las sumas de los slaves
	for (int i = 0; i < NUM_PROC; i++){
		MPI_Recv(&suma_individual, 1, MPI_INT, i, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		suma_global += suma_individual;
		printf ("Suma individual recibida de proceso esclavo: %d es = %d\n", status.MPI_SOURCE, suma_individual);
	}
	printf ("El resultado global de la suman es: %d\n", suma_global);

	
		

	
	} else { //slave
		
	
	/*PSEUDOCODIGO 1:
		1. Inicializar array entero
		2. Seleccionar la zona que haya tocado sumar
		3. Enviar la suma al master
		4. Mensajes por pantalla para debug
		
	
	
	int reparto = SIZE_ARRAY / NUM_PROC;
	
	//todos los esclavos inicializan el array entero
	int *array = malloc (SIZE_ARRAY * sizeof(int));
	for (int i = 0; i < SIZE_ARRAY; i++){
		array[i] = i;
	}
	if (me == 0){ //primer esclavo
		int suma = 0;
		for (int i = 0; i < reparto; i++){
			suma += array[me*reparto+i];
		}
		MPI_Send (&suma, 1, MPI_INT, 3, me, MPI_COMM_WORLD); //enviar suma
		printf ("Sender: %d acaba de terminar.\n", me);
		
	
	} else if (me == 1) { //segundo escalvo
		int suma = 0;
		for (int i = 0 ; i < reparto; i++){
			suma += array[me*reparto+i];
		}
		MPI_Send (&suma, 1, MPI_INT, 3, me, MPI_COMM_WORLD);
		printf ("Sender: %d acaba de terminar.\n", me); 
		
	
	} else { //tercer esclavo me==2
		int suma = 0;
		for (int i = 0; i < reparto; i++){
			suma += array[me*reparto+i];
		}
		MPI_Send (&suma, 1, MPI_INT, 3, me, MPI_COMM_WORLD);
		printf ("Sender: %d acaba de terminar.\n", me);
	
	
	}
	
	*/
	
	  /*PSEUDOCODIGO 2:
	  	1. Recibir array inicializado de master
	  	2. Computar la zona que haya tocado sumar
	  	3. Enviar la suma al master
	  	4. Mensajes pantalla
	  */
	  if (me == 0){
	  	int suma = 0;
	  	int reparto = SIZE_ARRAY / NUM_PROC;
	  	int *array = malloc (SIZE_ARRAY * sizeof(int));
	  	
	  	//recibimos array desde master
	  	MPI_Recv(array, SIZE_ARRAY, MPI_INT, 3, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
	  	
	  	//computamos y enviamos
	  	for (int i = 0; i < reparto; i++){
			suma += array[me*reparto+i];
		}
		MPI_Send (&suma, 1, MPI_INT, 3, me, MPI_COMM_WORLD);
		printf ("Sender: %d acaba de terminar.\n", me);
	  	
	  	
	  
	  
	  } else if (me == 1){
	  	int suma = 0;
	  	int reparto = SIZE_ARRAY / NUM_PROC;
	  	int *array = malloc (SIZE_ARRAY * sizeof(int));
	  	
	  	MPI_Recv(array, SIZE_ARRAY, MPI_INT, 3, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
	  	
	  	for (int i = 0; i < reparto; i++){
			suma += array[me*reparto+i];
		}
		MPI_Send (&suma, 1, MPI_INT, 3, me, MPI_COMM_WORLD);
		printf ("Sender: %d acaba de terminar.\n", me);
	  
	  
	  } else {
	  	int suma = 0;
	  	int reparto = SIZE_ARRAY / NUM_PROC;
	  	int *array = malloc (SIZE_ARRAY * sizeof(int));
	  	
	  	MPI_Recv(array, SIZE_ARRAY, MPI_INT, 3, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
	  	
	  	for (int i = 0; i < reparto; i++){
			suma += array[me*reparto+i];
		}
		MPI_Send (&suma, 1, MPI_INT, 3, me, MPI_COMM_WORLD);
		printf ("Sender: %d acaba de terminar.\n", me);
	  
	  }
	}
	
	MPI_Finalize(); //shutdown MPI
	return 0;
}


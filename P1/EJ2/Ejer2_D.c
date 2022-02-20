/*
 * Ejer1_D.c
 * 
 * 
 * 
 */


#include <stdio.h>
#include <string.h>
#include <mpi.h>

#define NUM_MESSAGES 10

int main(int argc, char **argv){
	int me; //rank of process
	int p; //number of processes
	int dest;
	//int tag = 0; //tag for messages
	int number; //number to send 
	int buff_number[2];
	MPI_Status status; //return status
	
	MPI_Init (&argc, &argv); //start rup MPI
	MPI_Comm_rank (MPI_COMM_WORLD, &me); //get process rank
	MPI_Comm_size (MPI_COMM_WORLD, &p); //get number of processes
	
	if (me == 0){ //receiver
		/*
		printf ("Receiver: %d printing messages received:\n", me);
		
		
		for (int num_pc = 1; num_pc < p; num_pc++) {
			for (int tag = 9; 0 <= tag; tag--){
			 	MPI_Recv(buff_number, 1, MPI_INT, num_pc, tag+100, MPI_COMM_WORLD, &status);
			 	MPI_Recv(&buff_number[1], 1, MPI_INT, num_pc, tag, MPI_COMM_WORLD, &status);
				printf ("Message %d received from %d: %d\n", tag+100, num_pc, buff_number[0]);
				printf ("Message %d received from %d: %d\n", tag, num_pc, buff_number[1]);
			}
		}
		printf ("Receiver ends printing messages.\n"); */
		
	} else { //sender
	
		dest = 0;
		number = 100;
		
		for (int i = 0; i < NUM_MESSAGES; i++){
			MPI_Rsend (&number, 1024, MPI_INT, dest, i, MPI_COMM_WORLD);
			MPI_Rsend (&number, 1024, MPI_INT, dest, i+100, MPI_COMM_WORLD);
			
			number++;
		}
		printf ("Sender %d has just finished.\n", me);
		
		
	}
	
	MPI_Finalize(); //shutdown MPI
	
	
	return 0;
}


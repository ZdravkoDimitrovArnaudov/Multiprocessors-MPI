#!/bin/bash
#SBATCH --partition=lsciv       #especifica la partición grupo de nodos

#SBATCH -o mpi.%j.out   #Nombre del archivo de salida
#SBATCH -J mpiJob       #Nombre del trabajo
#SBATCH --nodes=10    #Numero de nodos para correr el trabajo
#SBATCH --ntasks=10      #Numero de procesos
#SBATCH --tasks-per-node=1 #Numero de tareas por nodo

#Prepara el ambiente de trabajo(no es necesario)
export I_MPI_PMI_LIBRARY=/usr/local/slurm/lib/libpmi.so
ulimit -l unlimited
#Ejecuta el programa paralelo (en este cluster no se usa srun)
prun ./a.out

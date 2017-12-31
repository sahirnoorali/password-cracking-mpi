/*
 * Author: Sahir
 * Code: Password Matching from a very large password file using MPI
 */
 
//*****************************************************************************************
// Libraries:
//***************************************************************************************** 
#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
//*****************************************************************************************
/*
 * FindPassword finds the password (pass) from the input MPI file and also requiring the 
 * MPI Comm size, MPI Comm rank character overlap (olap) 
 */
int FindPassword(MPI_File *input, char* pass,int length, const int rank, const int size, const int olap) {
   
   	//Init the needed variables
	MPI_Offset starting_offset;
	int mysize;
	char *buff;

	MPI_Offset ending_offset;
	MPI_Offset file_size;

	//Fetch the file size
	MPI_File_get_size(*input, &file_size);

	//Removing the EOF
	file_size--;  

	//File size to process by each process
	mysize = file_size/size;
	
	//Starting offset of each process
	starting_offset = rank * mysize;

	//Ending offset of each process
	ending_offset   = starting_offset + mysize - 1;

	if (rank == size-1) 
	    ending_offset = file_size-1;

	//Add overlap to the end of everyone's buff except last proccess
	if (rank != size-1)
	    ending_offset += olap;

	mysize =  ending_offset - starting_offset + 1;

	//Allocate Memory
	buff = malloc( (mysize + 1)*sizeof(char));

	//Everyone reads in their part 
	MPI_File_read_at_all(*input, starting_offset, buff, mysize, MPI_CHAR, MPI_STATUS_IGNORE);
	buff[mysize] = '\0';

	//Set up the reading positions rank wise
	int locstart=0, locend=mysize-1;
	if (rank != 0) {
		while(buff[locstart] != '\n') locstart++;
			locstart++;
	}//close if
	if (rank != size-1) {
		locend-=olap;
		while(buff[locend] != '\n') locend++;
	}//close if

	//Searching:
	int i = 0, j = 0;
	for (i=locstart; (i<=locend); i++) {

		char c = buff[i];
		if(isspace(c)){
		     j = 0;
		     continue;	
		}//close if 

		if((j < length) && (c == pass[j])){
		    j++;
		}//close if

		if(j == length){
		   printf("Password Found!\n");
		   return 1;
		}//close if

	}//close for
	return 0;
    
}//close FindPassword
//*****************************************************************************************
int main(int argc, char **argv) {

    //Declare the MPI File variable
    MPI_File input;
	
    //Declare the rank(id of process) and size (total processes)
    int rank, size;
	
    //Declare the error code
    int error_code;
	
    //And the characte overlap
    const int olap = 100;
	
    //Init MPI and fetch the input arguments
    MPI_Init(&argc, &argv);
	
    //Init the rank according to arguments
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
    //Init the size according to arguments
    MPI_Comm_size(MPI_COMM_WORLD, &size);
	
    //Open the file via MPI and deal with any errors
    error_code = MPI_File_open(MPI_COMM_WORLD, "passwords.txt", MPI_MODE_RDONLY, MPI_INFO_NULL, &input);
    if (error_code) {
        if (rank == 0) 
		fprintf(stderr, "%s: Can't Open the Password File: %s\n", argv[0], argv[1]);
        MPI_Finalize();
        exit(2);
    }//close if

    //Setting a hardcoded password to search for
    char * pass = "dobies"; int length = 6;
	
    //Start recording time
    clock_t t = clock();
	
    //Find the password
    int success = FindPassword(&input, pass,length, rank, size, olap);
    
    //Calculate time taken
    t = clock() - t;
    
    //If successful then print the time taken
    if(success == 1)
	printf("Time Taken: %f\n",(float)t/CLOCKS_PER_SEC);
    
    //Close the MPI file
    MPI_File_close(&input);

    //Terminate all the processes systematically via MPI Finalize
    MPI_Finalize();
	
    return 0;

}//close main

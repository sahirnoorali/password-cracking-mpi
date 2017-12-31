# Password Cracking using MPI

This project is all about cracking/matching passwords from a large list of passwords with High Performance Computing or parallel processing via MPI (Message Passing Interface).

## MPI
MPI (Message Passing Interface) is a standard Application Programming Interface (API) for message passing, supporting various parallel
computing architectures to develop large scale parallel applications (https://en.wikipedia.org/wiki/Message_Passing_Interface).

The load is distributed by spawning various processes and the communication is done via MPI and thus, a de facto standard for distributed
memory systems.

## Password Cracking
The procedure of matching passwords from the list is divided amongst the number of processes specified
(given as input via MPI command) thus, dividing the searching load and ensuring faster matching than a serial counterpart.

## Password File
The password file used can be downloaded from: http://dazzlepod.com/site_media/txt/passwords.txt

/*
# ------------------------------
# starter.c -- a starter program file for CMPUT 379
#     This file includes the following:
#     - Some commonly used header files
#     - Functions WARNING and FATAL that can be used to report warnings and
#       errors after system calls
#     - A function to clear a VT100 screen (e.g., an xterm) by sending
#        the escape sequence "ESC[2J" followed by "ESC[H"
#     - A function to test the usage of an array of structures
#
#    
#  Compile with:  gcc starter.c -o starter         (no check for warnings)
#  	          gcc -Wall starter.c -o starter   (check for warnings)
#		  gcc -ggdb starter.c -o starter   (for debugging with gdb)
#
#  Usage:  starter  stringArg  intArg	 (e.g., starter abcd 100)
#
#  Author: Prof. Ehab Elmallah (for CMPUT 379, U. of Alberta)
# ------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <time.h>
#include <assert.h>
#include <string.h>
#include <stdarg.h>             //Handling of variable length argument lists
#include <sys/time.h>

#include <unistd.h>		
#include <sys/types.h>
#include <sys/stat.h>	
#include <sys/wait.h>
#include <sys/times.h>
#include <fcntl.h>
// ------------------------------

#define MAXLINE 256
#define MAXSIZE 100

typedef struct { char city[MAXLINE]; int temp; } cityTemp_t;
// ------------------------------
// The WARNING and FATAL functions are due to the authors of
// the AWK Programming Language.

void WARNING (const char *fmt, ... )
{
    va_list  ap;
    fflush (stdout);
    va_start (ap, fmt);  vfprintf (stderr, fmt, ap);  va_end(ap);
}

void FATAL (const char *fmt, ... )
{
    va_list  ap;
    fflush (stdout);
    va_start (ap, fmt);  vfprintf (stderr, fmt, ap);  va_end(ap);
    fflush (NULL);
    exit(1);
}

// ------------------------------------------------------------
// clrScreen() -- send VT100 escape sequence to clear the screen
// 	          (works fine in some cases)

void clrScreen () {
     char   ESC=033;		// the ESC character
     printf ("%c[2J", ESC); printf ("%c[H", ESC);
}
// ------------------------------------------------------------

void test_struct_array ()
{
    unsigned int        i, count, ct_size;
    cityTemp_t		ct[MAXSIZE];

    ct_size= 0;		 // Reset the number of elements in array ct to zero

    cityTemp_t  data[] = { {"Victoria", 16}, {"Vancouver", 15}, {"Prince George", 8},
    		           {"Edmonton", 5},  {"Calgary", 12}, {"Saskatoon", 9},
			   {"Regina", 11}, {"Winnipeg", 11} };


    count=  sizeof(data)/sizeof(cityTemp_t); 			   

    for (i=0 ; i < count; i++) {
       if (ct_size >= MAXSIZE - 1)
           FATAL("test_struct_array: array capacity exceeded (size= %d)\n",
	   	  ct_size);	   			     
       ct[ct_size++]= data[i];
    }       

    printf ("test_struct_array: city-temperature table: ");
    for (i=0; i < ct_size; i++)
        printf("[%s, %d], ", ct[i].city, ct[i].temp);
    printf("\n");

}

// ------------------------------ 
int main(int argc, char *argv[]) {

    char     strArg[MAXLINE];
    int	     intArg;

    strcpy (strArg, "");    intArg= 0;

    clrScreen();

    // read argv[1] (a text string) and argv[2] (an integer)
    //
    if (argc >= 2)  strcpy (strArg, argv[1]);  	// string copy
    if (argc >= 3)  intArg= atoi(argv[2]);
    printf ("%s:  strArg= %s, intArg= %d \n", argv[0], strArg, intArg);


    test_struct_array();
  
    WARNING ("testing function WARNING [%s] [%d]\n", strArg, intArg);
}



#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define iterations 500
#define mainthread 0       
// timing
double CLOCK() {
        struct timespec t;
        clock_gettime(CLOCK_MONOTONIC,  &t);
        return (t.tv_sec * 1000)+(t.tv_nsec*1e-6);
}

double dartboard(int number_throws)
{
        long random(void);
        // coordinates and score cariables
        double X, Y, pi, r;
        int totaltotalscore = 0;
        int n = 0;
        // start throwing darts
        for (n = 1; n <= number_throws; n++)  {
                //generate X
                X = (2.0 * (double)random()/(double)RAND_MAX) - 1.0;
                // generate Y
                Y = (2.0 * (double)random()/(double)RAND_MAX) - 1.0;
                // if lands in the circle, increment score
                if ((X*X) + (Y*Y) <= 1.0)
                        totaltotalscore++;
        }
        //return the pie estimate 
        return(4.0 * (double)totaltotalscore/(double)number_throws);
}

int main (int argc, char *argv[])
{
        //value of pi calculated by current task 
        double   localpi;      
        // sum of tasks' pi values
        double  pisum;         
        // average of pi after "number_throws" is thrown 
        double  pi;            
        //average pi value for all iterations 
        double  avepi;          

        int number_throws = 100;
        // timing variables
        double start;
        double finish;
        double total;
        // mpi variables 
        int     taskid;         
        int     numtasks;       
        int     reduced;            
        int     i;

        MPI_Status status;
        //Initialize MPI
        MPI_Init(&argc,&argv);
        MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
        MPI_Comm_rank(MPI_COMM_WORLD,&taskid);
        printf ("MPI task %d has started...\n", taskid);
        printf ("%d\n", numtasks);
        srandom (taskid);
        // mainthread start the time
        if (taskid == mainthread) {

                start = CLOCK();
        }

        avepi = 0;
        // main loop
        for (i = 0; i < 20; i++) {
                //start the throws
                localpi = dartboard(number_throws);
                // sum up all the pi estimates from tasks into one ariable
                reduced = MPI_Reduce(&localpi, &pisum, 1, MPI_DOUBLE, MPI_SUM,mainthread, MPI_COMM_WORLD);
                //printf("%d\n", taskid);
                if (taskid == mainthread) {
                        pi = pisum/numtasks;
                        avepi = ((avepi * i) + pi)/(i + 1);
                }
        }
        MPI_Finalize();
        // print final time
        if (taskid == mainthread) {
                finish = CLOCK();
                total = finish - start;
                printf("Time for the loop = %f\n", total);
        }
        return 0;
}


                                                                                                                                                107,0-1       Bot

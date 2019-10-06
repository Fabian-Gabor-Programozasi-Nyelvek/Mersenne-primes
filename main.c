#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <pthread.h>
#include <math.h>
#include <windows.h>
#define LIMIT 30

// Lucas–Lehmer
// Determine if Mp = 2p − 1 is prime for p > 2
int is_mersenne_prime(unsigned long int m, int p) {
    unsigned long int s = 4;
    for (int i=0; i<p-2; i++) {
        s = ((s * s) - 2) % m;
    }
    if (s == 0) return 1;
    else return 0;
}

void print_prime (unsigned long int i, unsigned long int *out) {
    printf("%lu ",out[i]);
}

int main() {
    unsigned long int m;
    double startCalc, endCalc;
    double runTime;
    int prime_count = 0, limit = LIMIT;
    unsigned long int *out;
    out = (unsigned long int *) malloc ( ceil(pow(2,limit)) * (sizeof(unsigned long int)));

    printf("Calculating all prime numbers under 2^%lu.\n",limit);

    startCalc = omp_get_wtime();

    if (limit >= 2) {
        out[0] = 2;
        prime_count++;
    }

    #pragma omp parallel
    {
        #pragma omp for schedule (runtime)
            for (unsigned long int i=1; i<limit; i++) {
                m = pow(2,i)-1;
                if (is_mersenne_prime(m, i)) {
                    out[(int)i] = m;
                    #pragma omp critical
                        prime_count++;
                }
                else {
                    out[(int)i]=0;
                }
            }
    }
    endCalc = omp_get_wtime();
    runTime = endCalc - startCalc;

    printf("Calculated all %lu prime numbers under %lu in %g seconds\n\n",prime_count, limit, runTime);

    /*
    char yesno = 'n';
    printf("Do you want to print the primes up to %lu? (y/n) ", limit);
    scanf("%c", &yesno);
    */
    //if ((yesno == 'y') || (yesno == 'Y')) {
        double startPrint = omp_get_wtime();
        for (unsigned long i=0; i<limit; i++){
            if (out[i]!=0)
                printf("%lu ",out[i]);
        }

        double endPrint = omp_get_wtime();
        double printTime = endPrint - startPrint;

        printf("\n\nCalculated all %lu prime numbers under 2^%lu in %g seconds\n",prime_count, limit, runTime);
        printf("Printing all %lu prime numbers under 2^%lu took %g seconds to print\n\n\n",prime_count, limit, printTime);
    //}

    printf("\n");


/*
    FILE *f = fopen("primes.txt", "w");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    for (unsigned long i=0; i<floor(limit/2); i++){
        if (out[i]!=0) fprintf(f, "%lu\n", out[i]);
    }

    fclose(f);
*/


    return 0;
}

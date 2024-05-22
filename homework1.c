#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <ctype.h>


int nrDigits2(int num) {
    return floor(log10(abs(num))) + 1;
}

int nrDigits(int number) {
    static int totalNumbers = 0, digits1 = 0, digits2 = 0, digits3 = 0, digits4 = 0, digits5 = 0;

    // Read and analyze each number in the file

        totalNumbers++;

        // Calculate the number of digits
        int numDigits = nrDigits2(number);
        switch (numDigits) {
            case 1:
                digits1++;
                break;
            case 2:
                digits2++;
                break;
            case 3:
                digits3++;
                break;
            case 4:
                digits4++;
                break;
            case 5:
                digits5++;
                break;
            default:
                break;
        }
    if(totalNumbers==1000){
    printf("1 digits - %d\n", digits1);
    printf("2 digits - %d\n", digits2);
    printf("3 digits - %d\n", digits3);
    printf("4 digits - %d\n", digits4);
    printf("5 digits - %d", digits5);
    return totalNumbers;
    }
    return 0;
}

int isPrime2(int num) {
    if (num <= 1)
        return 0;
    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0)
            return 0;
    }
    return 1;
}

int isPrime(int number) {
 static int total = 0;
 static int primes = 0;
 static int nonprimes=0;
        
        if (isPrime2(number)){
            total++;
            primes++;
        }else{
            total++;
            nonprimes++;
        }

    if(total==1000){
    printf("Primes - %d\n", primes);
    printf("Nonprimes - %d\n", nonprimes);
    return total;
    }
    return 0;
}

int main(int argc, char *argv[]) {
    int status1;
    int status2;
    FILE *file;        
    FILE *file2;

    pid_t pid1;
    pid_t pid2;
    int pfd1[2];
    int pfd2[2];
    int pfd3[2];
    int pfd4[2];


    pipe(pfd1);
    pipe(pfd2);
    pipe(pfd3);
    pipe(pfd4);
printf("Input file: %s\n", argv[1]);
printf("\n");

    pid1 = fork();		//create first child

    if (pid1 == 0) {	//first child process	
        close(pfd1[0]);		//close read side of pipe
        dup2(pfd1[1], 1);	//close stdout and dup into pipe
        file = fopen(argv[1], "r");
        if (file == NULL) {
            perror("File open failed");
            exit(EXIT_FAILURE);
        }
        int value;

        while(fscanf(file,"%d\n",&value) != EOF){
        nrDigits(value);
        }
        fclose(file);
        close(pfd4[0]);
        close(pfd1[1]); // close write side of pipe
        exit(0);
    } else if (pid1 > 0) {	//parent process
        waitpid(pid1, &status1, 0);	//wait for child finish
        close(pfd1[1]);
        close(pfd4[1]);

        close(0);	//close stdin
        dup(pfd1[0]);	//dup into read side of pipe
            char buffer1[100];
        while (read(0, &buffer1, sizeof(buffer1)) > 0) {	//read from pipe
            if (buffer1[0] != '\0') //if it's not empty
                printf("%s\n", buffer1);
        }
        close(pfd1[0]); // close read side of pipe
    } else {
        perror("fork");
        return -1;
    }
    pid2 = fork();	//create second child
    if (pid2 == 0) {
        close(pfd2[0]);		//close read side of pipe
    	                        //close stdout	
        dup2(pfd2[1],1);		//dup into pipe write
     file2 = fopen(argv[1], "r");
        if (file2 == NULL) {
            perror("File open failed");
            exit(EXIT_FAILURE);
        }
        int value2;
     while(fscanf(file2,"%d\n",&value2) != EOF){
        isPrime(value2);
        }
        
        fclose(file2);
        close(pfd2[1]); 
        close(pfd3[0]);
// Pipe close
        exit(0);
    } else if (pid2 > 0) {
        waitpid(pid2, &status2, 0);
        close(pfd2[1]);
        close(pfd3[1]);

        close(0);
        dup(pfd2[0]);
        char buffer2[100];
        while (read(0, &buffer2, sizeof(buffer2)) > 0) {	//read from pipe
            if (buffer2[0] != '\0') // if it's not empty
                printf("%s\n", buffer2);
        }
        
        close(pfd2[0]); // Pipe close
        close(pfd4[1]);

    } else {
        perror("fork");
        return -1;
    }

    return 0;
}

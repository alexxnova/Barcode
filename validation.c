#include "validation.h"

const char signalCode[NUMCODE][CHARSIZE] = {
    "00001", "10001", "01001", "11001", "00101", "10100", "01100", "00011", "10010", "10000", "00100", "00110"};


int checkNoise(char * rawSignal, int i){
    if((rawSignal[i+2] != '0') && (rawSignal[i+3] != '0')){
        if((rawSignal[i-2] != '0') && (rawSignal[i-3] != '0')){
            return 0;
        }else{
            return 1;
        }
    }
}

int checkStartStop(char * signal){

    // Read the first and last five digits from the signal
    char first[6];
    char last[6];
    
    int lenSignal = strlen(signal);
    int j = lenSignal-5;
    int i;
    for(i=0; i<5; i++,j++){
        first[i] = signal[i];
        last[i] = signal[j];            
    } 
    first[i] ='\0';
    last[i] = '\0';

    // Compare the signal for start/stop and reverse
    if(!strcmp(first, signalCode[11])){
        if(!strcmp(first, last)){   //check do we have a regular start/stop pair
            return 0;
        }else{      //only start code is scanned
            printf("\nBarcode truncated. Move the scanner to the right.\n");
            exit(1);
        }
    }else if(!strcmp(last, signalCode[11])){ //only stop code is scanned
            printf("\nBarcode truncated. Move the scanner to the left.\n");
            exit(1);
    }else if(!strcmp(first, signalCode[6])){
        if(!strcmp(first, last)){   //check do we have a reverse start/stop pair
            return 1;
        }else{      //only stop code is scanned
            printf("\nBarcode truncated. Move the scanner to the left.\n");
            exit(1);
        }
    }else if(!strcmp(last, signalCode[6])){ //only start code is scanned
            printf("\nBarcode truncated. Move the scanner to the right.\n");
            exit(1);
    }else{  // code without start or stop signal is scanned
        printf("\nTry scanning again.\n");
        exit(1);
    }
}

int checkConsistency(char * signal){
    int lenSignal = strlen(signal) - 10; // without the signal for start/stop code
    if(lenSignal%5 != 0){   // check for clear signal for each five digit sequence
        printf("\nBarcode inconsistent. Try scanning again.\n");
        exit(1);
    }else if ((lenSignal/5) > 1){   // more than one five sequence (if only start is scanned)
        return 1;
    }else{
        printf("\nBarcode too short. Try scanning again.\n");
        exit(1);
    }
    return 0;
}

int validateCode(int numSymbols,int * arr){
    int n = numSymbols-1; // n-> position index starting from 0
    int C=arr[n-1];
    int K=arr[n]; // version 2.0 arr[n-2]
    int checkC = equationC(n,arr);
    int checkK = equationK(n,arr);
    if(C==checkC && K==checkK){
        return 0;
    }
    return 1; // version 2.0
}

static int equationC(int n, int * arr){
    int i;
    int sum=0;
    for(i=1; i<=n;i++){
        sum= (((n-i)%10 + 1) * arr[i-1]);
    }
    return sum%11;
}

static int equationK(int n, int * arr){
    int i;
    int sum=0;
    for(i=1; i <= n+1; i++){
        sum= (((n-i+1)%9 + 1) * arr[i-1]);
    }
    return sum%11;
}

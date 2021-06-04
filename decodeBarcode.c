#include "decodeBarcode.h"
#include "validation.h"

char * readScannerSignal(char * fileName){
    int i = 0;
    int j = 0;
    // int counter = 0;
    int num;
    float signalScanner;
 
    FILE * file = fopen(fileName, "r");
    if (file == NULL) { 
        printf("File does not exist"); 
        exit(1); 
        }
 
    // Fisrt number from the file - number of raw signals
    fscanf(file, " %d", &num); 

    // Scan the raw signal from the input file
    char * rawSignal = (char*)malloc(sizeof(char)*num);
    if(rawSignal == NULL){
        perror("ERROR in readScannerSignal()");
    }

    while (fscanf(file, " %f", &signalScanner) == 1) {
        rawSignal[i++] = roundNo(signalScanner) +'0';
    }
    rawSignal[i]='\0';

    // version 2.0
    fclose(file);

    return rawSignal;
}

char * decodeRawSignal(char * rawSignal){
    int i = 0;
    int j = 0;
    //int counter = 0;
    int lenRawSignal = strlen(rawSignal);
    char * signal = (char*)malloc(sizeof(char) * lenRawSignal);
    if(signal == NULL){
        perror("ERROR in decodeRawSignal()");
    }
    
    for(; i<lenRawSignal; i++){
        if(rawSignal[i] == '0'){
            if(checkNoise(rawSignal,i)){
                if(rawSignal[i+1] == '0'){
                    signal[j++] = '1';
                    i++;
                }else if(rawSignal[i]){
                    signal[j++] = '0';
                }
            }else{
                break; // continue
            }
        }else if(rawSignal[i] == '1'){
            // continue;
        }// else {prtinf("ERROR"); exit(1);}
    }
    signal[j]='\0';

    return signal;
}

int decodeSignal(char * signal){

    // 0-regular start/stop ; 1-reverse start/stop
    if(checkStartStop(signal)){
        // int lenSignal = strlen(signal)-1;
        // char reverse[lenSignal];
        // reverseSignal(signal,reverse);
        reverseSignal(signal);
    }
    
    // calculate number of simbols without the signal for start/stop code
    int numSymbols;
    if(checkConsistency(signal)){
        numSymbols = (strlen(signal) - 10)/5; 
    }
 
    // Decode signal
    char code[numSymbols][CHARSIZE];
    int weight[numSymbols];
    int i;
    int j;
    int pos = 5; // position after the start signal

    for(i=0; i < numSymbols; i++){
        for(j=0;j<5;j++){
            code[i][j] = signal[pos++];
        }
        code[i][j] ='\0';
        for (int h=0; h<NUMCODE; h++){    
            if (!strcmp(code[i], signalCode[h])){
                weight[i]=h;
            }
        }
    }

    if(validateCode(numSymbols,weight)){    // Validation of check digits
        printf("ERROR Code is not valid.");
        exit(1);
    }else printResult(numSymbols, weight); // Final result

    return 0;
}

void reverseSignal(char * str){
    char * endStr = str + strlen(str)-1;

    while (str<endStr){
        char temp = *str;
        *str = *endStr;
        *endStr = temp;
        str++;
        endStr--;
    }
}

static int roundNo(float num){
    return (int)num+0.52;
}

static int printResult(int numSymbols, int weight[]){
    for (int i=0;i<numSymbols-2;i++){
        if(weight[i]==10){
            printf("-");
        }else printf("%d",weight[i]);
    }
return 0;
}

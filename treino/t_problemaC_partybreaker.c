#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

#define MAX_MEN 255
#define MAX_WOMEN 255
#define MAX_TEMP 1024

int main(){

    char temp[MAX_TEMP];

    while(fgets(temp, MAX_TEMP, stdin) != NULL){
        if (strcmp(temp, "")==0 || strcmp(temp, " ")==0 || strcmp(temp, "\n")==0) {
            continue;
        }
        
        int menNumbers[MAX_MEN];
        int menCount = 0;
        int womenNumbers[MAX_WOMEN];
        int womenCount = 0;
        int n;
        int digit;
        int i, j, k, m;
        int fair = 0;
        
        try{
            n = atoi(temp);
        } catch(int e){
            continue;
        }
        if (n==0) {
            continue;
        }
        

        for(i=0; i<n; i++){ /*While there are numbers to be read*/
            fgets(temp, MAX_TEMP, stdin);
            char *token = strtok(temp, " ");
            digit = atoi(token);
            if (digit < 0){
                menNumbers[menCount] = abs(digit);
                ++menCount;
            }
            else{
                womenNumbers[womenCount] = digit;
                ++womenCount;
            }
        }

        /*Test with 2 men*/
        for(j=0; j<menCount; j++){
            for (k=j+1; k<menCount; k++){
                for(m=0; m<womenCount; m++){
                    if (menNumbers[j] + menNumbers[k] == womenNumbers[m]) {
                        fair = 1;
                        break;
                    }
                    
                }
            }
        }

        /*Already found solution?*/
        if (fair == 1) {
            printf("Fair\n");
        }
        else{ /*Test with 2 women*/
            for(j=0; j<womenCount; j++){
                for (k=j+1; k<womenCount; k++){
                    for(m=0; m<menCount; m++){
                        if (womenNumbers[j] + womenNumbers[k] == menNumbers[m]) {
                            fair = 1;
                            break;
                        }
                    
                    }
                }
            }

            if (fair == 1) {
                printf("Fair\n");
            }
            else printf("Rigged\n");
        }
    }

    return 0;
}

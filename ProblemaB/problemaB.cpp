#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_TEMP 2024

typedef struct Event{
    int deadline;
    int duration;
    int profit;
} Event;

int max(int a, int b){ return a>b?a:b; }
int min(int a, int b){ return a>b?b:a; }





/********************************************************************
 * Function that prints resulting sum matrix, DP. For debugging only.
 *
 ********************************************************************/
void print_DP(int **DP, int num_events, int max_deadline, int width){
    /*-----------FOR NICE PRINTING ONLY----------*/
    //Print deadline numbers
    for (int i=0; i<width+3; i++) printf(" ");
    for (int i=0; i<=max_deadline; i++){
        printf("%*d ", width, i);
    }
    printf("\n");
    for (int i=0; i<width+3; i++) printf(" ");
    for (int i=0; i<(width+1)*max_deadline+width; i++){
        printf("-");
    }
    printf("\n");
    /*-------END OF FOR NICE PRINTING ONLY--------*/

    for(int i=0; i<=num_events; i++){
        printf("%*d | ", width, i);
        for(int j=0; j<=max_deadline; j++){
            printf("%*d ", width, DP[i][j]); //Print with adjust for different size numbers
        }
        printf("\n");
    }

    printf("\n");
}





/**********************************************************************
 * Simple function that prints received input. For debug purposes only.
 * 
 **********************************************************************/
void print_input(Event event_list[], int num_events){
    printf("Received %d events: \n", num_events);
    for (int i=0; i<num_events; i++){
        printf("%d: Deadline-%d Duration-%d Profit-%d\n", 
            i+1, event_list[i].deadline, event_list[i].duration, event_list[i].profit);
    }
    printf("Done\n");
}





/*********************************************************
 * Comparison function for qsort, smaller deadlines first.
 *
 *********************************************************/
int comparison(const void *a, const void *b){
    Event *eventA = (Event *)a;
    Event *eventB = (Event *)b;

    return eventA->deadline - eventB->deadline;
}





/*****************************************************************************************************
 * Validation function that prints index of each chosen event (starting from 0) and its info
 * 
 * Parameters:
 *      event_list[]: List of received events, struct Event, containing deadline, duration and profit;
 *      DP[][]: Sum matrix, in order to find which events were chosen.
 *      num_events: Number of received events, plus one in order to count from 0;
 *      max_deadline: Maximum deadline found in received events, plus one in order to count from 0;
 * 
 *****************************************************************************************************/
void print_solution(Event event_list[], int **DP, int num_events, int max_deadline){
    int i=num_events, j=max_deadline;
    while (1){ //Move back to first 'best' value in last row
        if (DP[i][j-1] == DP[i][j]) j -= 1;
        else break;
    }
    while(i>0 && j>0){
        if (DP[i-1][j] == DP[i][j]){ //Value came from above
            i -= 1;
        }
        else{
            printf("Event index: %d | Start time: %d | End time: %d | Deadline: %d | Profit: %d\n", 
                i-1, j - event_list[i-1].duration, j, event_list[i-1].deadline, event_list[i-1].profit);

            j -= event_list[i-1].duration;
            i -= 1;
        }
    }
}






/*****************************************************************************************************
 * Main problem function
 * Adapted from Knapsack problem bottom up algorithm: https://pastebin.com/i4aeQnmg.
 * 
 * Parameters:
 *      event_list[]: List of received events, struct Event, containing deadline, duration and profit;
 *      num_events: Number of received events, plus one in order to count from 0;
 *      max_deadline: Maximum deadline found in received events, plus one in order to count from 0;
 *      DP[][]: Sum matrix, to save subproblems answers.
 * 
 * Returns:
 *      Single integer, representing largest possible profit.
 *****************************************************************************************************/
int problemB(Event event_list[], int num_events, int max_deadline, int **DP){
    for (int i=0; i<=num_events; i++){ //Initializes DP's first column to 0
        DP[i][0] = 0;
    }

    for (int j=1; j<=max_deadline; j++){ //Initializes DP's first row (counting from second column) to 0
        DP[0][j] = 0;
    }

    for (int i=1; i<=num_events; i++){
        for (int j=1; j<=max_deadline; j++){
            if (event_list[i-1].duration > min(j, event_list[i-1].deadline)) //If duration is bigger than the smallest deadline, then event can't fit
                DP[i][j] = DP[i-1][j];
            else 
                DP[i][j] = max(DP[i-1][j], DP[i-1][min(j, event_list[i-1].deadline) - event_list[i-1].duration] + event_list[i-1].profit);
        }
    }

    return DP[num_events][max_deadline];
}





/*****************************************************************
 * Main function. Receives input and fills event list with events
 * 
 *****************************************************************/
int main(){

    char temp[MAX_TEMP];

    /*----------INPUT CODE----------*/
    while(fgets(temp, MAX_TEMP, stdin) != NULL){ /*While EOF isn't reached*/
        /*Previous input was number of events*/
        int num_events = atoi(temp);

        Event event_list[num_events];

        /*Read individual event data*/
        for (int i=0; i<num_events; i++){
            fgets(temp, MAX_TEMP, stdin);
            char *token = strtok(temp, " "); /*Split received input by whitespace*/

            Event event;
            event.deadline = atoi(token); token = strtok(NULL, " ");
            event.duration = atoi(token); token = strtok(NULL, " ");
            event.profit = atoi(token);

            event_list[i] = event;
        }

        /*Sort by deadline (smallest first)*/
        qsort(event_list, num_events, sizeof(Event), comparison);

        /*Get largest deadline*/
        int max_deadline = event_list[num_events-1].deadline;

        /*Test received input (for debugging only)*/
        //print_input(event_list, num_events);

        /*Allocate dynamic programming subproblem result sum matrix*/
        int **DP = (int**)malloc((num_events+1) * sizeof *DP);
        int *data = (int*)malloc((num_events+1) * (max_deadline+1) * sizeof *data);
        for (int i=0; i<num_events+1; i++, data+=max_deadline+1) DP[i] = data;

        /*Main problem function*/
        printf("%d\n", problemB(event_list, num_events, max_deadline, DP));

        /*Print validation*/
        //print_solution(event_list, DP, num_events, max_deadline);

        /*Print DP (for debugging only)*/
        /*printf("\n");
        int maxval = DP[num_events][max_deadline]; //Adjust for different sized numbers in output
        int width = round(1+log(maxval)/log(10)); //Adjust for different sized numbers in output
        printf("Max deadline: %d\n", max_deadline);
        printf("Num of events: %d\n", num_events);
        print_DP(DP, num_events, max_deadline, width);*/

        /*Free DP*/
        free(*DP);
        free(DP);
    }

    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TEMP 2024
#define MAX_EVENTS 10000

typedef struct Event{
    int deadline;
    int duration;
    int profit;
} Event;

/*Simple function that prints received input. For debug purposes only.*/
void print_input(Event event_list[], int num_events){
    printf("Received %d events: \n", num_events);
    for (int i=0; i<num_events; i++){
        printf("%d: Deadline-%d Duration-%d Profit-%d\n", 
            i+1, event_list[i].deadline, event_list[i].duration, event_list[i].profit);
    }
    printf("Done\n");
}

void problemB(Event event_list[], int num_events){
    
}

int main(int argc, char **argv){

    char temp[MAX_TEMP];

    /*----------INPUT CODE----------*/
    while(fgets(temp, MAX_TEMP, stdin) != NULL){ /*While EOF isn't reached*/
        /*Previous input was number of events*/
        int num_events = atoi(temp);

        Event event_list[MAX_EVENTS];

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

        /*Test received input*/
        print_input(event_list, num_events);

        /*Main problem function*/
        problemB(event_list, num_events);
    }

    return 0;
}
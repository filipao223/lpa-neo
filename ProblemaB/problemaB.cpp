#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TEMP 2024

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

int comparison(const void *a, const void *b){
    Event *eventA = (Event *)a;
    Event *eventB = (Event *)b;

    return (eventA->deadline - eventB->deadline);
}

/*
    Ideia 1:
        1-Ordenar os eventos por deadline;
        2-Fazer uma arvore como no ProblemaA do treino2
            (Escolher a maior soma possivel)
            Ver: https://pastebin.com/pbC8zins
        3-Cada nivel tem o mesmo deadline
        4-Escolher o maior profit de cada nivel e passar para o nivel
            equivalente a nivel_atual + duration, p .ex:
            2, tem deadline 2, o evento com maior profit tem duraçao
            5, entao passa para o nivel 7, deadline 7, ...
            (Esta parte é duvidosa)
        5- ???
        6- Profit
    
    Ideia 2:
        1-Ordenar os eventos por deadline;
        2-Não fazer uma arvore, mas usar o array dos eventos
        3-Usar outro array para guardar somas
        4-Usar um contador que guarde a duraçao atual e lucro atual
        5-Entrar na funçao recursivamente, aumentando o index
        6-Se duraçao for superior a deadline, proseguir
        7-Se nao, aumentar a duraçao e lucro
        8-Se chegar ao fim, voltar recursivamente e guardar soma em sum_array

    Ideia 3 (best ideia) (USAR ESTA):
        Tabela do stor
        Deadline:       1 2 3 4 5 6 7 8 9     Lista eventos: Duraçao
      Numero Eventos
            |
            0           T F F F F F F F F       Evento 1: 3
            1           T F T T T F F F F       Evento 2: 2
            2           T ...                   Evento 3: 4
            3           T ...                   Evento 4: 6
            4           T ...                   Evento 5: 2

        Em vez de T ou F, usar o profit de cada evento, como no exemplo Knapsack


*/
int problemB(Event event_list[], int num_events, int index, int sum_array[], int duration, int profit){
    if (index >= num_events) return 0;
    else if (duration > event_list[index].deadline){
        sum_array[index] = problemB(event_list, num_events, index+1, sum_array, duration, profit);
    }
}

int main(int argc, char **argv){

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

        /*Sort by deadline*/
        qsort(event_list, num_events, sizeof(Event), comparison);

        /*Test received input*/
        print_input(event_list, num_events);

        /*Array that holds recursive steps sum results*/
        int sum_array[num_events] = {0};

        /*Main problem function*/
        problemB(event_list, num_events, 0, sum_array, 0, 0);
    }

    return 0;
}
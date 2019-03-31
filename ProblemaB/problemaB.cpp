#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TEMP 2024
#define MAX_DEADLINE 10000

typedef struct Event{
    int deadline;
    int duration;
    int profit;
} Event;

int max(int a, int b){ return a>b?a:b; }

/**/
void print_DP(int DP[][MAX_DEADLINE], int num_events, int max_deadline){
    for(int i=0; i<=num_events; i++){
        for(int j=0; j<=max_deadline; j++){
            printf("%d  ", DP[i][j]);
        }
        printf("\n");
    }
}

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
        Ver: https://pastebin.com/i4aeQnmg

*/
int problemB(Event event_list[], int num_events, int max_deadline, int DP[][MAX_DEADLINE]){
    for (int i=0; i<=num_events; i++){ //Inicia primeira coluna de DP a 0
        DP[i][0] = 0;
    }

    for (int j=1; j<=max_deadline; j++){ //Inicia primeira linha (a contar da segunda coluna) de DP a 0
        DP[0][j] = 0;
    }

    for (int i=1; i<=num_events; i++){
        for (int j=1; j<=max_deadline; j++){
            if (event_list[i-1].duration>=j) 
                DP[i][j] = DP[i-1][j];
            else 
                DP[i][j] = max(DP[i-1][j], DP[i-1][j-event_list[i-1].duration] + event_list[i-1].profit);
        }
    }

    return DP[num_events][max_deadline];
}

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

        /*Sort by deadline*/
        qsort(event_list, num_events, sizeof(Event), comparison);

        /*Get largest deadline*/
        int max_deadline = event_list[num_events-1].deadline;

        /*Test received input*/
        //print_input(event_list, num_events);

        /*Array that sum results*/
        int DP[num_events][MAX_DEADLINE]; //+1 to include zero events and zero deadline at beggining. See lpa_15_mar.txt line 32

        /*Main problem function*/
        printf("%d\n", problemB(event_list, num_events, max_deadline, DP));

        /*Print DP*/
        //printf("Max deadline: %d\n", max_deadline);
        //printf("Num of events: %d\n", num_events);
        print_DP(DP, num_events, max_deadline);
    }

    return 0;
}
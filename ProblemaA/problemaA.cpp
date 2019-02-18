#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TEMP 1024

/*
	6 (6 posiçoes possiveis)
	4 5 - pos1
	6 4 - pos2
	2 5 - pos3
	4 1 - pos4
	3 5 - pos5
	1 5 - pos6
	3(num de devices) 3(numero de interseções)
	1 2 - ligação entre o device 1 e 2
	1 3 - ligação entre o device 1 e 3
	2 3 - ligação entre o device 2 e 3

	A resolução do problema é conseguir arranjar as posições que com essas ligações dêm o minimo de interseções possiveis
*/

void print_input(int num_pos, int coord[], int num_dev, int num_int, int conn[]){
	printf("Num positions: %d\n", num_pos);
	printf("Coord: \n");
	for(int i=0; i<num_pos*2; i+=2){
		printf("%d %d\n", coord[i], coord[i+1]);
	}
	printf("Num of devices: %d\tNum of intersections: %d\n", num_dev, num_int);
	printf("Connections: \n");
	for(int i=0; i<num_int*2; i+=2){
		printf("%d %d\n", conn[i], conn[i+1]);
	}
}

int main(){

	char temp[MAX_TEMP];
	char *token = NULL;


	while(fgets(temp, MAX_TEMP, stdin) != NULL){ //Keep reading until EOF (end-of-file (null))
		int num_pos = atoi(temp); 	//Numero de posiçoes possiveis
		int coord[num_pos*2]; 		//Coordenadas das posiçoes [x1, y1, x2, y2, ..., xNum_pos, yNum_pos]

		for(int i=0; i<num_pos*2; i+=2){
			fgets(temp, MAX_TEMP, stdin);

			token = strtok(temp, " "); //Divide string by " "
			coord[i] = atoi(token);

			token = strtok(NULL, " ");	//Get next token in divided string
			coord[i+1] = atoi(token);
		}

		fgets(temp, MAX_TEMP, stdin);
		token = strtok(temp, " ");
		int num_devices = atoi(token);
		token = strtok(NULL, " ");
		int num_intersect = atoi(token);

		int connections[num_intersect*2];
		for(int i=0; i<num_intersect*2; i+=2){
			fgets(temp, MAX_TEMP, stdin);
			token = strtok(temp, " ");
			connections[i] = atoi(token);
			token = strtok(NULL, " ");
			connections[i+1] = atoi(token);
		}

		print_input(num_pos, coord, num_devices, num_intersect, connections);
	}

	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TEMP 1024

typedef struct Point{
	int x, y;
	int available;
} Point;

//Global variables
Point *coord;
int *connections;
Point *device_coord;
int num_pos, num_coord, num_devices, num_intersect;

/*
	Simple function that returns the largest of two integers
*/
int max(int num1, int num2){
	return num1>num2?num1:num2;
}

/*
	Simple function that returns the smallest of two integers
*/
int min(int num1, int num2){
	return num1>num2?num2:num1;
}

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

/*
	p1-------------q1

			p2
			 \
			  \
			   \
			    \
				 \
				  q2
 */

void print_input(int num_pos, Point coord[], int num_dev, int num_int, int conn[]){
	printf("Num positions: %d\n", num_pos);
	printf("Coord: \n");
	for(int i=0; i<num_pos; i++){
		printf("%d %d\n", coord[i].x, coord[i].y);
	}
	printf("Num of devices: %d\tNum of intersections: %d\n", num_dev, num_int);
	printf("Connections: \n");
	for(int i=0; i<num_int*2; i+=2){
		printf("%d %d\n", conn[i], conn[i+1]);
	}
}

/*
	If the orientation of both segments is equal, then one may be partially or
		completely overlapping another.
	I.e.:
		p1-------p2===q1-------q2
	The segment (p2,q2) overlaps segment (p1,q1)
	If p2.x is between (or is equal to) largest and smallest x-coord of (p1,q1) and p2.y is also
		between largest and smallest y-coord of (p1,q1), then they intersect
		(test for all points) TODO: optimize

	Returns:
		1, if they overlap
		0, elsewise
*/
int check_overlapping(Point p1, Point q1, Point p2, Point q2){
	//Point p1
	if (p1.x < max(p2.x, q2.x) && p1.x > min(p2.x, q2.x) 
		&& p1.y < max(p2.y, q2.y) && p1.y > min(p2.y, q2.y))
		return 1;
	//Point q1
	else if (q1.x < max(p2.x, q2.x) && q1.x > min(p2.x, q2.x) 
			&& q1.y < max(p2.y, q2.y) && q1.y > min(p2.y, q2.y))
		return 1;
	//Point p2
	else if (p2.x < max(p1.x, q1.x) && p2.x > min(p1.x, q1.x) 
			&& p2.y < max(p1.y, q1.y) && p2.y > min(p1.y, q1.y))
		return 1;
	//Point q2
	else if (q2.x < max(p1.x, q1.x) && q2.x > min(p1.x, q1.x) 
			&& q2.y < max(p1.y, q1.y) && q2.y > min(p1.y, q1.y))
		return 1;
	//Not overlapping
	else return 0;
}

/*
	Source: http://www.dcs.gla.ac.uk/~pat/52233/slides/Geometry1x1.pdf
	Slope of segment (p1,p2): sigma = (y2-y1) / (x2-x1)
	Slope of segment (p2,p3): tau = (y3-y2) / (x3-x2)
	The orientation depends on whether the expression
		exp = (y2−y1)*(x3−x2) − (y3−y2)*(x2−x1)
		is positive, negative, or null.

	Returns:
		1, if counterclockwise -> exp < 0
	   -1, if clockwise -> exp > 0
		0, if collinear -> exp = 0
*/
int orientation_test(Point p1, Point p2, Point p3){
	int value = (p2.y-p1.y)*(p3.x-p2.x) - (p3.y-p2.y)*(p2.x-p1.x);
	if (value < 0) return 1;
	else if (value > 0) return -1;
	else return 0;
}

/*
	Source: http://www.dcs.gla.ac.uk/~pat/52233/slides/Geometry1x1.pdf
	Two segments (p1,q1) and (p2,q2) intersect if and only if
	ONE of the following two conditions is verified

	general case:
		- (p1,q1,p2) and (p1,q1,q2) have different orientations
				AND
		- (p2,q2,p1) and (p2,q2,q1) have different orientations

	special case
		- (p1,q1,p2), (p1,q1,q2), (p2,q2,p1), and (p2,q2,q1) are all collinear
				AND
		- the x-projections of (p1,q1) and (p2,q2) intersect
		- the y-projections of (p1,q1) and (p2,q2) intersect

	Para se intersectarem, OU o caso geral OU o caso especial têm de ser verificados

	Returns:
		1, if lines intersect
		-1, if they don't intersect
		0, if they are collinear
*/
int check_intersection(Point p1, Point q1, Point p2, Point q2){
	//Check if all points are the same
	//if (p1.x==q1.x && p1.x==p2.x && p1.x==q2.x && p1.y==q1.y && p1.y==p2.y && p1.y==q2.y) return -1;
	//General case
	if ((orientation_test(p1, q1, p2) != orientation_test(p1,q1,q2))
		&& (orientation_test(p2,q2,p1) != orientation_test(p2,q2,q1)))
	{
		//Lines intersect
		return 1;
	}
	//Special case
	else if((orientation_test(p1,q1,p2)==0) && (orientation_test(p1,q1,q2)==0)
		&& (orientation_test(p2,q2,p1)==0) && (orientation_test(p2,q2,q1)==0)
		&& check_overlapping(p1,q1,p2,q2)==1){
			//Lines collinear (overlapping)
			return 1;
		}

	//Lines do not intersect
	return 0;
}

int check_same_coord(Point device_coord[], int num_devices){
	for (int i=0; i<num_devices; i++){
		for (int j=i+1; j<num_devices; j++){
			if (device_coord[i].x == device_coord[j].x 
				&& device_coord[i].y == device_coord[j].y) return 1;
		}
	}

	return 0;
}

/*
	Ideia: recursao
	D1 = Device 1

	D1 <- recebe coordenada
	⮡ D2 <- recebe coordenada
		⮡ D3 <- recebe coordenada
			⮡ ...
				⮡ Dn <- ultimo ponto, ciclo atraves do resto das coord
	:Inicia-se a recursao com uma coordenada para o Device 1, depois entra-se na funçao para o Device 2 e dá-lhe uma coord, ...
	:Ao chegar ao ultimo device, calcular a interseçao de todos os pontos
	:Guardar o numero de interseçoes, se o calculo ultrapassar o menor numero de interseçoes, guardar esse e cancelar
		o resto dos calculos para estas coordenadas
	:Se chegar ao fim dos calculos e não for maior que o menor numero, guardar esse como menor (melhor ate agora)
	:Ao calcular todas as coordenadas para um determinado device (nivel recursivo), sair desse nivel, e incrementar o nivel acima
		para a proxima coordenada, voltar a descer até ao ultimo device e recomeçar.
	:TODO: como seria obvio, otimizar este processo (depois de a implementação ingenua estiver completa
			(10 pontos são melhores que 0))
	:TODO: verificar se ha mais que um device com o mesmo par de coordenadas
*/
int _problemaA(int device_num, int num_devices, int num_intersections, int num_coord, int best){
	if (device_num >= num_devices) return -1;
	else if (device_num == num_devices-1){
		//Calculate intersections
		/*printf("====Device num: %d====", device_num);*/
		int values = 0;
		//Check if two or more devices have the same coordinates
		if(check_same_coord(device_coord, num_devices) == 1) return best;
		for(int j=0; j<num_intersections; j+=2){
			for(int m=j+2; m<num_intersections; m+=2){
				int rc = check_intersection(device_coord[connections[j]-1], device_coord[connections[j+1]-1],
						device_coord[connections[m]-1], device_coord[connections[m+1]-1]);

				if (rc>-1) values+=rc; //All points have same coordinates

				//If we already have a best number of intersections and number of current intersections already surpasses best number, cancel
				if(values > best) return best;
			}
		}

		return values;
	}
	else{
		for(int i=0; i<num_coord; i++){
			//Update this device's coordinates
			device_coord[device_num] = coord[i];
			
			//More recursion
			int value = _problemaA(device_num+1, num_devices, num_intersections, num_coord, best);

			//If return value is -1, ignore
			if ((value!=-1) && (value < best)) best=value;
		}

		return best;
	}
}

int main(){

	char temp[MAX_TEMP];
	char *token = NULL;


	while(fgets(temp, MAX_TEMP, stdin) != NULL){ //Keep reading until EOF (end-of-file (null))
		num_pos = atoi(temp); 	//Numero de posiçoes possiveis
		//Point coord[num_pos]; 		//Coordenadas das posiçoes [p1,p2,p3,...,pN] p1 -> x1,y1
		coord = (Point*)malloc(num_pos * sizeof(Point));

		for(int i=0; i<num_pos; i++){
			fgets(temp, MAX_TEMP, stdin);

			token = strtok(temp, " "); //Divide string by " "
			coord[i].x = atoi(token);

			token = strtok(NULL, " ");	//Get next token in divided string
			coord[i].y = atoi(token);

			coord[i].available = 1;
		}

		fgets(temp, MAX_TEMP, stdin);
		token = strtok(temp, " ");
		num_devices = atoi(token);
		token = strtok(NULL, " ");
		num_intersect = atoi(token);

		//int connections[num_intersect*2];
		connections = (int*)malloc(num_intersect*2 * sizeof(int));
		for(int i=0; i<num_intersect*2; i+=2){
			fgets(temp, MAX_TEMP, stdin);
			token = strtok(temp, " ");
			connections[i] = atoi(token);
			token = strtok(NULL, " ");
			connections[i+1] = atoi(token);
		}

		//Array que contem as coordenadas dos pontos de cada device
		//device_coord[device1_Point, device2_Point, device3_Point, ..., deviceN_Point]
		//Este array é mudado em cada nivel recursivo, de forma a atualizar as coordenadas
		//Point device_coord[num_devices];
		device_coord = (Point*)malloc(num_devices * sizeof(Point));

		//Test input
		print_input(num_pos, coord, num_devices, num_intersect, connections);

		printf("Best result: %d\n", _problemaA(0, num_devices, num_intersect, num_pos, __INT_MAX__));

		//Test intersection detection
		/*Point p1 = {.x = 1, .y = 1}, q1 = {.x = 10, .y = 1};
		Point p2 = {.x = 1, .y = 2}, q2 = {.x = 10, .y = 2};
		printf("Return value: %d\n", check_intersection(p1,q1,p2,q2));
		p1 = {.x=10, .y=0}, q1 = {.x=0, .y=10}; 
    	p2 = {.x=0, .y=0}, q2 = {.x=10, .y=10};
		printf("Return value: %d\n", check_intersection(p1,q1,p2,q2));
		p1 = {.x=-5, .y=-5}, q1 = {.x=0, .y=0}; 
    	p2 = {.x=1, .y=1}, q2 = {.x=10, .y=10};
		printf("Return value: %d\n", check_intersection(p1,q1,p2,q2));*/

		//Free memory
		free(coord);
		free(device_coord);
		free(connections);
	}

	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TEMP 1024

typedef struct Point{
	int x, y;
	int available;
} Point;

typedef struct Device{
    Point p;
    int id;
    int available;
}Device;

typedef struct Linha{
    Point p1,p2;
    int inicio,fim;
}Linha;

FILE *output;

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

//void print_solution(Point temp[], )

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
	if (p1.x <= max(p2.x, q2.x) && p1.x >= min(p2.x, q2.x)
		&& p1.y <= max(p2.y, q2.y) && p1.y >= min(p2.y, q2.y))
		return 1;
	//Point q1
	else if (q1.x <= max(p2.x, q2.x) && q1.x >= min(p2.x, q2.x)
			&& q1.y <= max(p2.y, q2.y) && q1.y >= min(p2.y, q2.y))
		return 1;
	//Point p2
	else if (p2.x <= max(p1.x, q1.x) && p2.x >= min(p1.x, q1.x)
			&& p2.y <= max(p1.y, q1.y) && p2.y >= min(p1.y, q1.y))
		return 1;
	//Point q2
	else if (q2.x <= max(p1.x, q1.x) && q2.x >= min(p1.x, q1.x)
			&& q2.y <= max(p1.y, q1.y) && q2.y >= min(p1.y, q1.y))
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
	//Check if all points are the sam

	if((p1.x == p2.x && p1.y == p2.y) || (p1.x == q2.x && p1.x == q2.y) || (q1.x == p2.x && q1.y == p2.y) || (q1.x == q2.x && q1.y == q2.y)) return 0;
	//General case
	else if ((orientation_test(p1, q1, p2) != orientation_test(p1,q1,q2))
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
int _problemaA(int device_num, int num_devices, int num_intersections, int num_coord, Point coord[], int best, int connections[], Point device_coord[]){
	if (device_num > num_devices) return -1;
	else if (device_num == num_devices){
		//Calculate intersections
		int values = 0;
		for(int j=0; j<num_intersections; j+=4){
			int rc = check_intersection(device_coord[connections[j]-1], device_coord[connections[j+1]-1],
						device_coord[connections[j+2]-1], device_coord[connections[j+3]-1]);

			if (rc==-1) return best; //All points have same coordinates
			else values+=rc;

			//If we already have a best number of intersections and number of current intersections already surpasses best number, cancel
			if(values > best) return best;
		}

		/*if (values==0){
			printf("====7
5 9
7 8
7 5
6 2
3 3
2 5
3 8
5 4
1 2
1 3
3 5
2 4====\nValues is 0\n");
			for(int k=0; k<num_devices; k++){
				printf("Device %d coordinates: x=%d; y=%d\n", k+1, device_coord[k].x, device_coord[k].y);
			}
			printf("========\n");
		}*/

		return values;
	}
	else{
		for(int i=0; i<num_coord; i++){
			//Update this device's coordinates
			//if(coord[i].available == 1){
                device_coord[device_num] = coord[i];
              //  coord[i].available =0;
			//}
			//More recursion
			int value = _problemaA(device_num+1, num_devices, num_intersections, num_coord, coord, best, connections, device_coord);
			//If return value is -1, ignore
			if ((value!=-1) && (value < best)){
                best=value;
                printf(" Best temp %d ", value);
			}
		}

		return best;
	}
	return best;
}


int line_segment(int coord_x1, int coord_y1, int coord_x2, int coord_y2, int coord_x3, int coord_y3){
    if(coord_x2 <= max(coord_x1, coord_x3) && coord_x2 >= min(coord_x1, coord_x3) && coord_y2 <= max(coord_y1, coord_y3) && coord_y2 >= min(coord_y1,coord_y3)){
        return 1;
    }
    return 0;
}

int orientacao(int coord_x1, int coord_y1, int coord_x2, int coord_y2, int coord_x3, int coord_y3){
    int aux = (coord_y2 - coord_y1)*(coord_x3 - coord_x2) - (coord_x2-coord_x1)*(coord_y3-coord_y2);
    if(aux == 0){   //se e colinear
        return 0;
    }
    if(aux > 0){
        return 1;
    }
    else{
        return 2;
    }
}


int check_interception(int coord_x1, int coord_y1, int coord_x2, int coord_y2, int coord_x3, int coord_y3, int coord_x4, int coord_y4){

    int o1 = orientacao(coord_x1, coord_y1, coord_x2, coord_y2, coord_x3, coord_y3);
    int o2 = orientacao(coord_x1, coord_y1, coord_x2, coord_y2, coord_x4, coord_y4);
    int o3 = orientacao(coord_x3, coord_y3, coord_x4, coord_y4, coord_x1, coord_y1);
    int o4 = orientacao(coord_x3, coord_y3, coord_x4, coord_y4, coord_x2, coord_y2);

	if(coord_x1 == coord_x3 && coord_y1 == coord_y3){
        return 0;
    }

    if(coord_x1 == coord_x4 && coord_y1 == coord_y4){
        return 0;
    }

    if(coord_x2 == coord_x3 && coord_y2 == coord_y3){
        return 0;
    }

    if(coord_x2 == coord_x4 && coord_y2 == coord_y4){
        return 0;
    }

    //caso geral
    if( o1 != o2 && o3 != o4)
        return 1;


    if(o1 == 0 && line_segment(coord_x1, coord_y1, coord_x3, coord_y3, coord_x2, coord_y2))
        return 1;
    if(o2 == 0 && line_segment(coord_x1, coord_y1, coord_x4, coord_y4, coord_x2, coord_y2))
        return 1;
    if(o3 == 0 && line_segment(coord_x3, coord_y3, coord_x1, coord_y1, coord_x4, coord_y4))
        return 1;
    if(o4 == 0 && line_segment(coord_x3, coord_y3, coord_x2, coord_y2, coord_x4, coord_y4))
        return 1;
    return 0;
}

int combination(int device_num, int num_devices,int num_intersect,int num_coord,int best,int connections[],Point device_coord[],Point temp[],Linha linha[], Point best_points[]){
    if (device_num > num_devices) return -1;
    else if( device_num>2 ){

        int values = 0;
        /*for(int i=0;i<num_devices;i++){
            printf("%d %d, ",temp[i].x,temp[i].y);
		}*/
		//printf("\n");
		for(int i=0;i<num_intersect;i++){
                //printf("Inicio %d %d ", temp[linha[i].inicio].x,temp[linha[i].inicio].y);
                //printf("Fim %d %d \n", temp[linha[i].fim].x,temp[linha[i].fim].y);
                //printf("Inicio %d Pontos %d %d ",i, temp[linha[i].inicio-1].x,temp[linha[i].inicio-1].y);
                //printf("Fim %d Pontos %d %d ",i, temp[linha[i].fim-1].x,temp[linha[i].fim-1].y);
                linha[i].p1.x = temp[linha[i].inicio-1].x;
                linha[i].p1.y = temp[linha[i].inicio-1].y;
                linha[i].p2.x = temp[linha[i].fim-1].x;
                linha[i].p2.y = temp[linha[i].fim-1].y;
                //printf("L1 %d P1 %d , %d ",i,linha[i].p1.x,linha[i].p1.y);
                //printf("L2 %d P2 %d , %d ",i,linha[i].p2.x,linha[i].p2.y);
		}
		//printf("\n");
		for(int i=0;i<num_intersect;i++){
            for(int j=i+1;j<num_intersect;j++){
                //int rc = check_in(linha[i].p1,linha[i].p2,linha[j].p1,linha[j].p2);
                int rc = check_interception(linha[i].p1.x,linha[i].p1.y,linha[i].p2.x,linha[i].p2.y,linha[j].p1.x,linha[j].p1.y,linha[j].p2.x,linha[j].p2.y);
                //printf("Intersecoes %d\n",rc);
                //printf("%d , %d \n",i,j);
                values+=rc;

                if(values > best) return best;
            }
		}

		if (device_num==num_devices && values < best) printf("-6Interseçoes: %d\n", values);
		
		if (device_num==num_devices && values < best) return values;
		if (device_num==num_devices && values == best) return best;
		else goto resume;
		//printf("Intersecoes %d\n", values);
    }
    else{
		resume:
        for(int i = 0;i<=num_coord;i++){
            if(device_coord[i].available ==1 ){
                temp[device_num].x = device_coord[i].x;
                temp[device_num].y = device_coord[i].y;
                device_coord[i].available = 0;
                int value = combination(device_num+1,num_devices,num_intersect,num_coord,best,connections,device_coord,temp,linha, best_points);
                device_coord[i].available = 1;
				if (value==0) return 0;

                if ((value!=-1) && (value < best)){
                    best = value;
					//Save best points until now
					for(int i=0; i<num_devices; i++){
						Point p = {.x = temp[i].x, .y = temp[i].y};
						best_points[i] = p;
					}
                    //printf(" Best temp %d ", value);
                }
            }
        }
        return best;
    }
    return -1;
}

int main(){

	char temp[MAX_TEMP];
	char *token = NULL;

	//Open file for validation
	output = fopen("data.out", "w");

	int test_case_number = 1;


	while(fgets(temp, MAX_TEMP, stdin) != NULL){ //Keep reading until EOF (end-of-file (null))
		int num_pos = atoi(temp); 	//Numero de posiçoes possiveis
		Point coord[num_pos]; 		//Coordenadas das posiçoes [p1,p2,p3,...,pN] p1 -> x1,y1

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
		int num_devices = atoi(token);
		token = strtok(NULL, " ");
		int num_intersect = atoi(token);

		int connections[num_intersect*2];
		Point temporario[num_devices];
		Linha linha[num_intersect];


		for(int i=0;i<num_devices;i++){
            temporario[i].available=1;
		}

		for(int i=0; i<num_intersect*2; i+=2){
			fgets(temp, MAX_TEMP, stdin);
			token = strtok(temp, " ");
			connections[i] = atoi(token);
			linha[i/2].inicio = atoi(token);
			token = strtok(NULL, " ");
			connections[i+1] = atoi(token);
			linha[i/2].fim = atoi(token);
		}

		//Best point array
		Point best_points[num_devices];

		/*for(int i=0; i<num_intersect; i++){
            printf("%d , %d \n",linha[i].inicio,linha[i].fim);
		}*/

		/*for(int i=0;i<num_devices;i++){
            printf("%d %d \n",device[i].id,device[i].available);
		}*/

		//Array que contem as coordenadas dos pontos de cada device
		//device_coord[device1_Point, device2_Point, device3_Point, ..., deviceN_Point]
		//Este array é mudado em cada nivel recursivo, de forma a atualizar as coordenadas

		//Test input
		//print_input(num_pos, coord, num_devices, num_intersect, connections);

		//printf("Best result: %d\n", _problemaA(0, num_devices, num_intersect, num_pos, coord, __INT_MAX__, connections, device_coord));
	
		printf("%d\n",combination(0,num_devices,num_intersect,num_pos,__INT_MAX__,connections,coord,temporario,linha, best_points));

		//Write best points to validation file
		fprintf(output, "Best results for test %d\n", test_case_number);
		for (int i=0; i<num_devices; i++){
			fprintf(output, "%d %d\n", best_points[i].x, best_points[i].y);
		}

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

		test_case_number+=1;
	}

	//Close output file
	fclose(output);

	return 0;
}
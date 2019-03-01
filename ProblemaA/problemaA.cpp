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
int line_segment(int coord_x1, int coord_y1, int coord_x2, int coord_y2, int coord_x3, int coord_y3){
    if(coord_x2 <= max(coord_x1, coord_x3) && coord_x2 >= min(coord_x1, coord_x3) && coord_y2 <= max(coord_y1, coord_y3) && coord_y2 >= min(coord_y1,coord_y3)){
        return 1;
    }
    return 0;
}

/*
	Source: http://www.dcs.gla.ac.uk/~pat/52233/slides/Geometry1x1.pdf
	Slope of segment (p1,p2): sigma = (y2-y1) / (x2-x1)
	Slope of segment (p2,p3): tau = (y3-y2) / (x3-x2)
	The orientation depends on whether the expression
		exp = (y2−y1)*(x3−x2) − (y3−y2)*(x2−x1)
		is positive, negative, or null.

	Returns:
		2, if counterclockwise -> exp < 0
	    1, if clockwise -> exp > 0
		0, if collinear -> exp = 0
*/
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
		0, if they don't intersect
*/
int check_interception(int coord_x1, int coord_y1, int coord_x2, int coord_y2, int coord_x3, int coord_y3, int coord_x4, int coord_y4){

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

    int o1 = orientacao(coord_x1, coord_y1, coord_x2, coord_y2, coord_x3, coord_y3);
    int o2 = orientacao(coord_x1, coord_y1, coord_x2, coord_y2, coord_x4, coord_y4);
    int o3 = orientacao(coord_x3, coord_y3, coord_x4, coord_y4, coord_x1, coord_y1);
    int o4 = orientacao(coord_x3, coord_y3, coord_x4, coord_y4, coord_x2, coord_y2);

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
    else if( device_num == num_devices || device_num>2 ){

        int values = 0;
        
		for(int i=0;i<num_intersect;i++){
        
                linha[i].p1.x = temp[linha[i].inicio-1].x;
                linha[i].p1.y = temp[linha[i].inicio-1].y;
                linha[i].p2.x = temp[linha[i].fim-1].x;
                linha[i].p2.y = temp[linha[i].fim-1].y;
		}
		
		for(int i=0;i<num_intersect;i++){
            for(int j=i+1;j<num_intersect;j++){
                
                int rc = check_interception(linha[i].p1.x,linha[i].p1.y,linha[i].p2.x,linha[i].p2.y,linha[j].p1.x,linha[j].p1.y,linha[j].p2.x,linha[j].p2.y);
                values+=rc;

                if(values >= best) return best;
            }
		}

		//if (device_num==num_devices && values < best) printf("-6Interseçoes: %d\n", values);
		
		if (device_num==num_devices) return values;
		//if (device_num==num_devices && values == best) return best;
		/*We haven't reached best yet, continue with the recursion. To do that
		  goto to the for loop where the coordinates are given to devices
		  (because we havent yet given a new coordinate to every device
		  i.e., device_num is < than the max number of devices)*/
		else goto resume;
    }
    else{
		//Resume the recursive steps
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
						Point p;
                        p.x = temp[i].x;
                        p.y = temp[i].y;
						best_points[i] = p;
					}
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

		//Test input
		//print_input(num_pos, coord, num_devices, num_intersect, connections);

		printf("%d\n",combination(0,num_devices,num_intersect,num_pos,__INT_MAX__,connections,coord,temporario,linha, best_points));

		//Write best points to validation file
		fprintf(output, "Best results for test %d\n", test_case_number);
		for (int i=0; i<num_devices; i++){
			fprintf(output, "%d %d\n", best_points[i].x, best_points[i].y);
		}

		test_case_number+=1;
	}

	//Close output file
	fclose(output);

	return 0;
}

/*************************************************************************
* Header files used
*************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

//To keep a track of valid regions
static int valid[27];

//To send data as parameter to the thread runner functions
typedef struct
{
    int row;			//row number
    int col;			//column number
    int grid;			//subgrid number
    /****************************
    * Subgrid numbering
    * 1 2 3
    * 4 5 6
    * 7 8 9
    ****************************/
    int (*testPuzzle)[9];	//A pointer to the main grid to be passed to the worker threads
}parameters;

/*************************************************************************
* Function prototypes
*************************************************************************/
void * rowCheck(void * param);  //Checks validity of a row
void * colCheck(void * param);  //Checks validity of a column
void * gridCheck(void * param); //Checks validity of a subgrid


/*************************************************************************
* The main function
*************************************************************************/
int main()
{
    int i, j;
    //Hard-coded sudoku grid
    int testPuzzle[9][9] = {{6,5,3,1,2,8,7,9,4},
                            {1,7,4,3,5,9,6,8,2},
                            {9,2,8,4,6,7,5,3,1},
                            {2,8,6,5,1,4,3,7,9},
                            {3,9,1,7,8,2,4,5,6},
                            {5,4,7,6,9,3,2,1,8},
                            {8,6,5,2,3,1,9,4,7},
                            {4,1,2,9,7,5,8,6,3},
                            {7,3,9,8,4,6,1,2,5}
                           };
    //Initial display
    printf("Sudoku from MySudoku\n");
    for(i = 0; i < 9; i++)
    {
        for(j = 0; j < 9; j++)
            printf("%i ", testPuzzle[i][j]);
        printf("\n");
    }
    
    pthread_t workerThread[27]; //Array of 9*3 worker threads
    parameters *param[27];      //Array of parameters for worker threads

    //Creating 9*3 row, column & subgrid checking threads
    for(i = 0; i < 27; i++)
    {
        //Setting up parameter to be passed to the runner function
        param[i] = (parameters *) malloc(sizeof(parameters));

        param[i]->row = 0;
        param[i]->col = 0;
        param[i]->grid = 0;
        param[i]->testPuzzle = testPuzzle;

        if(i<9)			//9 row checking threads
        {
            param[i]->row = i;
	    pthread_create(&workerThread[i], NULL, rowCheck, (void *) param[i]);
        }
        else if(i<18)		//9 column checking threads
        {
            param[i]->col = i-9;
	    pthread_create(&workerThread[i], NULL, colCheck, (void *) param[i]);
        }
        else			//9 subgrid checking threads
        {
            param[i]->grid = i-18;
	    pthread_create(&workerThread[i], NULL, gridCheck, (void *) param[i]);
        }
    }

    //Joining of 9*3 threads
    for(i = 0; i < 27; i++)
    {
        pthread_join(workerThread[i], NULL);
	free(param[i]);
    }

    //Analysing final result
    int flag = 1;		//Validity flag 1=valid, 0=invalid
    for(i = 0; i < 27; i++)
    {
        if(valid[i] != 1)
        {
            flag = 0;
	    break;
            
        }
    }
    if(flag == 0)
	printf("Incorrect solution!\n");
    else
	printf("Correct solution!\n");

    return 0;
}

/*************************************************************************
* Function definitions
*************************************************************************/
void * rowCheck(void * param)
{
     parameters *data =(parameters *)param;
     int row = data->row;
     int i, j, count = 0;
     for (i = 1; i <= 9; i++) 
     {
	    for(j = 0;j<9;j++)
            {
	         if(data->testPuzzle[row][j] == i)
		 {
		      count++; 
		      break;
		 }
	    }
     }
     if (count == 9)
	valid[row] = 1;
}

void * colCheck(void * param)
{
     parameters *data =(parameters *)param;
     int col = data->col;
     int i, j, count = 0;
     for (i = 1; i <= 9; i++) 
     {
	    for(j = 0;j<9;j++)
            {
	         if(data->testPuzzle[j][col] == i)
		 {
		      count++; 
		      break;
		 }
	    }
     }
     
     if (count == 9)
	valid[col+9] = 1;
}

void * gridCheck(void * param)
{
     parameters *data =(parameters *)param;
     int grid = data->grid;
     int r, c;
     switch(grid)
     {
	case 0:r=0;c=0;break;
	case 1:r=0;c=3;break;
	case 2:r=0;c=6;break;
	case 3:r=3;c=0;break;
	case 4:r=3;c=3;break;
	case 5:r=3;c=6;break;
	case 6:r=6;c=0;break;
	case 7:r=6;c=3;break;
	case 8:r=6;c=6;break;
     }
     int i, j, k, count = 0;
     for (i = 1; i <= 9; i++) 
     {
	    for(j = r;j<(r+3);j++)
            {
		for(k = c; k<(c+3);k++)
		{
	         if(data->testPuzzle[j][k] == i)
		 {
		      count++; 
		      break;
		 }
		}
	    }
     }
     if (count == 9)
	valid[grid+18] = 1;
}

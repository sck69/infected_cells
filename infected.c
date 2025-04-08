
#include   <stdio.h>
#include <stdlib.h>
#include <time.h>
//#define DEBUG

float  inf_prob = 0.6;  // prob of making a neighbor infected  

typedef struct nodet {
    int occupied ;  
    int time_infected;
    int visited;
    float prob_of_infection;
} node ;


int num_rows, num_cols; // table size
int start_r, start_c;   // starting point, seed

#define SEEDcell(r,c) ((r)==start_r && (c) == start_c )

void debug_show_cells( int y, int x, int maxc, node nhood[][maxc]) {
#ifdef DEBUG
   int r, c;
   printf("given row=%i col=%i \n", y, x );
   for ( r=0; r < num_rows; r++, printf ("\n"))
      for (c =0; c < num_cols; c++)
printf ("[%3i][%3i]vis=%2i time=%6i ",r,c, nhood[r][c].visited,nhood[r][c].time_infected);   
#endif    
}

//make sure the given cell fits within the table. Ignore out of bound indexes
//Non occupied  cells ?
// A recursive function !!!
void   process_cell( int r, int c, int time, float prob, int cols, node  nhood[][cols]) {
    if(!(r >= num_rows || c >= num_cols) && !(r < 0 || c < 0)){
        if(nhood[r][c].occupied){
            if(nhood[r][c].visited == 0 || nhood[r][c].time_infected > time){
                nhood[r][c].visited += 1;
                nhood[r][c].time_infected = time;
                nhood[r][c].prob_of_infection = prob;
                debug_show_cells( r,c, cols, nhood );
            }
            else
                return;
        }
        else{
            return;
        }
      //infect  neighbors if they are not already so  
      process_cell(r + 1, c, time + 1, prob * inf_prob, cols, nhood);
      process_cell(r - 1, c, time + 1, prob * inf_prob, cols, nhood);
      process_cell(r, c + 1, time + 1, prob * inf_prob, cols, nhood);
      process_cell(r, c - 1, time + 1, prob * inf_prob, cols, nhood);
   }
}


int main ( void) {
   int x,y, k; int max_time; float current_least_prob;
   int empty_cells ;

   printf("Give number of rows, columns => ");
   scanf("%i %i", &num_rows, &num_cols);
   
   printf("Provide number of empty cells desired. less than 30 %% of cells => ");
   scanf(" %i", &empty_cells) ;
   if ( empty_cells >  0.3 * (float)num_rows* (float)num_cols ) {  
      empty_cells = (int) (0.3*(float)num_rows*(float)num_cols);
      printf("Empty cells limited to 30% = %i\n", empty_cells);
   }
   
   printf("Provide infection probability, less than 1.0 => ");
   scanf("%f", &inf_prob) ;
   node table[num_rows] [num_cols];  // create 2-D table
   
   printf("\nGive seed: row and col indices as 2 integers in one line.\n");
   scanf("%i %i", &start_r, &start_c );
   
   for (x =0; x < num_rows; x++)
      for (y =0; y < num_cols; y++) {
         table[x][y].occupied = 1;      // default  
         table[x][y].time_infected = -1;
         table[x][y].prob_of_infection = inf_prob;
         table[x][y].visited = 0;
      }
   for (k =0; k < empty_cells;  ) {  // mark empty cells randomly.
      x= rand() % num_rows;
      y = rand() % num_cols;
      if ( ! SEEDcell(x,y) )        // not a seed cell
         table[x][y].occupied = 0, k++;   // empty
   }

   process_cell( start_r, start_c, 0, inf_prob, num_cols, table );
   // Can add more seed cells -- How ???
   
   printf("[   ] on the table means that cell not occupied. \n");
   current_least_prob =  inf_prob; 
   max_time = -1;
   for (x =0; x < num_rows; x++, printf("\n"))
      for (y =0; y < num_cols; y++) {
         if (max_time < table[x][y].time_infected )   
            max_time = table[x][y].time_infected;
         if ( current_least_prob > table[x][y].prob_of_infection )
             current_least_prob = table[x][y].prob_of_infection ;
            if (SEEDcell(x,y))
               printf("<%2i>-%8.6f  ", table[x][y].time_infected,
                           table[x][y].prob_of_infection);
            else if (! table[x][y].occupied )
               printf("   [        ]  ");  
            else
               printf("%4i-%8.6f  ", table[x][y].time_infected,
                           table[x][y].prob_of_infection);
         } 
   
   printf ("Max time=%i  least_prob=%f \n", max_time, current_least_prob );
   return 0;
}

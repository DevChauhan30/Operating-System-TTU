//Author: Dev Chauhan
//Date: 09/15/2022
//CSC 4100 - Operating Systems
//Professor: Mike Rogers

void print_border(); // all function prototypes. 
void k_clearsrc();
void k_printstr(char *string, int row, int col);
void run_test();

//Assignment 2 Declerations
void initIDT();
void setupPIC();

int main()
{   
  int start_row = 0;
  int start_col = 0;
  int end_row = 24;
  int end_col = 79;

  k_clearsrc(); // clear screen Function Call
  print_border(start_row, start_col, end_row, end_col); // print border Function Call
  run_test(); // run test Function Call
  while(1){}

}    

void print_border(int start_row, int start_col, int end_row, int end_col)
{
    int i, j;
    for (i = start_row; i <= end_row; i++)
    {
        for (j = start_col; j <= end_col; j++)
        {
            if ((i== start_row && j == start_col ) || (i==start_row && j==end_col) || (i==end_row && j== start_col) || (i== end_row && j== end_col))
            {
                k_printstr("+", i, j);
            }
            else if (i == start_row || i == end_row)
            {
                k_printstr("-", i, j);
            }
            else if (j== start_col || j== end_col)
            {
                k_printstr("|" , i, j);
            }
            else{
                k_printstr(" ", i, j);
            }
                           
        }
    }
}
void k_clearsrc()
{   
    int i;

    for(i = 0; i < 80; i++ )
    {
        k_printstr(" ", i, 0);
    }

}
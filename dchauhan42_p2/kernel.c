//Author: Dev Chauhan
//Date: 10/20/2022
//CSC 4100 - Operating Systems
//Professor: Dr. Mike Roger

#include <stdlib.h>
#include <stdint.h>
#include "io.h"
#include "queue.h"
#include "buddy.h"
#include "convert.h"

//Function Prototypes:
uint32_t next_pid = 0;
idt_entry_t idt[256];
struct pcb_t* run;
struct pcbq_t* round_robin;
void print_border(int start_row, int start_col, int end_row, int end_col); 
void k_clearsrc();
void k_printstr(char *string, int row, int col);
void init_idt_entry(idt_entry_t *entry, uint32_t base, uint16_t selector, uint8_t access);
void init_idt();
void default_exception();
void p1();
void p2();
int create_process(uint32_t code_address);

int main(void)
{   
  int start_row = 0;
  int start_col = 0;
  int end_row = 24;
  int end_col = 79;

  buddy_init();
  k_clearsrc(); // clear screen Function Call
  print_border(start_row, start_col, end_row, end_col); // print border Function Call
  k_printstr("Running Processes",1,2);
  round_robin = create_rr(); 
  init_idt();

  int retval = create_process((uint32_t)p1); // set retval 
  if(retval < 0)                             // if statment that prints error if retval is less than 0
  {
    k_printstr("Error",3,1);
  }

  retval = create_process((uint32_t)p2);
  if(retval < 0)
  {
    k_printstr("Error",3,1);
  }

  create_process((uint32_t)p1); // running processes
  create_process((uint32_t)p2);
  go();                         //calling go

  return 0;
}    

//Functions:
void print_border(int start_row, int start_col, int end_row, int end_col) //print border function
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
void k_clearsrc() // clear screen function
{   
    int i;

    for(i = 0; i < 80; i++ )
    {
        k_printstr(" ", i, 0);
    }

}

int create_process(uint32_t code_address) // create process function
{
    uint32_t* stackptr = (uint32_t*)kmalloc(1024*sizeof(1024));

    uint32_t* st = stackptr+1024;
    
   
    *(--st) = 0x0000; // e-flags
    *(--st) = 0x0010; // code segment register 
    *(--st) = code_address; // starting intrustion of the process
    *(--st) = (uint32_t)dispatch_leave; 
    *(--st) = 0x0000; // ebp
    *(--st) = 0x0000; // esp
    *(--st) = 0x0000; // edi
    *(--st) = 0x0000; // esi
    *(--st) = 0x0000; // edx
    *(--st) = 0x0000; // ecx
    *(--st) = 0x0000; // ebx
    *(--st) = 0x0000; // eax

    struct pcb_t* process = new_pcb((uint32_t)st,next_pid);
    next_pid++;
    enqueue(round_robin,process);

    return 0;  //no errors
}

void p1() //function for process 1
{
    print_border(10,10,13,35);
    k_printstr("Process 1 running...", 11,11);
    k_printstr("Value: ",12,11);

    uint32_t num = 0;
    char string [4];
   
    while(1)
    {
        convert_num(num,string); //calling convernt num to convert num
        k_printstr(string, 12, 18);
        num++;
        if(num>=1000)
            num=0; 
        asm("int $32");       
    }
    return;
}

void p2() //function for process 2
{
    print_border(15,10,18,35);
    k_printstr("Process 2 running...", 16,11);
    k_printstr("Value: ",17,11);

    uint32_t num = 0;
    char string[4];
   
    while(1)
    {
        convert_num(num,string); //calling convernt num to convert num
        k_printstr(string, 17, 18);
        num++;
        if(num >=1000)
            num=0;  
        asm("int $32");      
    }
    return;
}

void init_idt() //function for intializing idt
{
    for (int i = 0; i < 32; i++) //for loop tp print error
    {
        init_idt_entry(idt+i,(uint32_t)default_exception,16,0x8e);
    }

    init_idt_entry(idt+32,(uint32_t)dispatch,16,0x8e);

    for (int i = 33; i < 256; i++)
    {
        init_idt_entry(idt+i,0,0,0);
    }

    struct idtr idtr;
    idtr.base  = (uint32_t)idt;
    idtr.limit = sizeof(idt)-1;
    lidtr((uint32_t)& idtr);                  
}

void default_exception()
{
    k_printstr("DEFAULT EXCEPTION",1,20);
    while(1);
}
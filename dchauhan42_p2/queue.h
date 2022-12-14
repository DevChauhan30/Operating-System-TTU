//Author: Dev Chauhan
//Date: 10/20/2022
//CSC 4100 - Operating Systems
//Professor: Dr. Mike Rogers

#ifndef STRUCT_H
#define STRUCT_H

#include <stdlib.h>
#include <stdint.h>
#include "buddy.h"

struct pcbq_t
{
	struct pcb_t* head;
	struct pcb_t* tail;
};

struct pcb_t
{
    uint32_t esp;
    uint32_t pid;
    struct pcb_t* next;
};

struct pcbq_t* create_rr() // initialize function for round robin queue
{
	struct pcbq_t* queue = (struct pcbq_t*)kmalloc(sizeof(struct pcbq_t));
	queue->head = NULL;
	queue->tail = NULL;

	return queue;
}

struct pcb_t* new_pcb(uint32_t esp, uint32_t pid)
{
	struct pcb_t* temp = (struct pcb_t*)kmalloc(sizeof(struct pcb_t));
    temp->esp  = esp;
    temp->pid  = pid;
    temp->next = NULL;
    
    return temp;
}

struct pcb_t* dequeue(struct pcbq_t* queue) //function for dequeue
{
	struct pcb_t *temp;
    struct pcb_t *x = queue->head;
    temp = queue->head;
    queue->head = queue->head->next;
    kfree(temp);
    return(x);
}
void enqueue(struct pcbq_t* queue, struct pcb_t* pcb) //function for enqueue
{
    struct pcb_t* temp;
    struct pcb_t* temp2;
    temp = (struct pcb_t*)kmalloc(sizeof(struct pcb_t));
    temp = pcb;
    temp->next = NULL;

    if(queue->head == NULL)
    {
        queue->head = temp;
        queue->tail = temp;
    }
    else
    {
        temp2 = queue->head;
        if(temp2->next != NULL)
        {
            temp2 = temp2->next;
        }
        temp2->next = temp;
        queue->tail = temp;
    }
}

struct idt_entry    //data structure for interrupt
{
    uint16_t  base_low16;
    uint16_t  selector;
    uint8_t   always0;
    uint8_t   access;
    uint16_t  base_hi16;   

} __attribute__((packed));
typedef struct idt_entry idt_entry_t;

struct idtr
{
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

void init_idt_entry(idt_entry_t *entry, uint32_t addr_of_handler, uint16_t code_selector, uint8_t access)
{
    entry->base_low16 = addr_of_handler & 0xffff;
    entry->base_hi16 = (addr_of_handler>>16) & 0xffff;
    entry->selector = code_selector;
    entry->access = access;
}

#endif
//Author: Dev Chauhan
//Date: 11/30/2022
//CSC 4100 - Operating Systems
//Professor: Dr. Mike Rogers

#ifndef STRUCT_H
#define STRUCT_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "buddy.h"

void default_exception();
struct pcb_t
{
    uint32_t esp;
    uint32_t pid;
    struct pcb_t* next;
    uint32_t priority;
};

struct pcbq_t* first;
struct pcbq_t* second;
struct pcbq_t* third;

struct pcbq_t
{
	struct pcb_t* head;
	struct pcb_t* tail;
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
	struct pcb_t *temp = queue->head;
    queue->head = queue->head->next;

    if(queue->head == NULL)
    {
        queue->tail = NULL;
    }

    return(temp);
}
void enqueue(struct pcbq_t* queue, struct pcb_t* pcb) //function for enqueue
{
    pcb->next = NULL;

    if(queue->tail == NULL)
    {
        queue->tail = queue->head = pcb;
    } else 
    {
        queue->tail->next = pcb;
        queue->tail = pcb;
    }
}

//Enqueue Priority

void enqueue_priority(struct pcb_t* pcb)
{
    if(pcb->priority == 3)
    {
        enqueue(first, pcb);
    }
    else if(pcb->priority == 2)
    {
        enqueue(second, pcb);
    }
    else if(pcb->priority == 1)
    {
        enqueue(third, pcb);
    }
}

struct pcb_t* dequeue_priority(struct pcbq_t* queue)
{
    if(first->head != NULL)
    {
        return dequeue(first);
    }
    else if (first->head == NULL && second->head != NULL)
    {
        return dequeue(second);
    }
    else if (first->head == NULL && second->head == NULL && third->head != NULL)
    {
        return dequeue(third);
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
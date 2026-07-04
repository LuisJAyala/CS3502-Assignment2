//Luis Ayala, CS 3502, Assignment 2


#ifndef BUFFER_H
#define BUFFER_H

#define BUFFER_SIZE 10
#define SHM_KEY 0x1234   


typedef struct {
    int value;           
    int producer_id;     
} item_t;


typedef struct {
    item_t buffer[BUFFER_SIZE];
    int head;            // write position 
    int tail;            // read position 
    int count;           // number of items in the buffer
} shared_buffer_t;

#endif

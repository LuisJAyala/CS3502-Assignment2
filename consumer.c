//Luis Ayala, CS 3502, Assignment_2

#include <stdio.h>

#include <stdlib.h>

#include <sys/shm.h>

#include <sys/ipc.h>

#include <semaphore.h>

#include <fcntl.h>

#include <unistd.h>

#include "buffer.h"

int main(int argc, char* argv[]) {

    if (argc != 3) {
        printf("Usage: %s <id> <num_items>\n", argv[0]);
        return 1;
          }
                                
int consumer_id = atoi(argv[1]);
int num_items = atoi(argv[2]);
                                        
                                      
int shm_id = shmget(SHM_KEY, sizeof(shared_buffer_t), 0666);

    if (shm_id < 0) {
      perror("shmget failed (Is the producer running?)\n");
      return 1;
           }
           

shared_buffer_t* shared_ptr = (shared_buffer_t*)shmat(shm_id, NULL, 0);
    if (shared_ptr == (void*)-1) {
        perror("shmat failed");
        return 1;
            }
                            
                               
sem_t* empty = sem_open("/sem_empty", 0);

sem_t* full = sem_open("/sem_full", 0);

sem_t* mutex = sem_open("/sem_mutex", 0);
                                            
  if (empty == SEM_FAILED || full == SEM_FAILED || mutex == SEM_FAILED) {
      perror("sem_open failed");
       return 1;
        }
        

for (int i = 0; i < num_items; i++) {
            
sem_wait(full);   
sem_wait(mutex);  
                            
item_t consumed_item = shared_ptr->buffer[shared_ptr->tail];
                                                    
printf("Consumer %d: Consumed value %d from Producer %d\n", consumer_id, consumed_item.value, consumed_item.producer_id);


shared_ptr->tail = (shared_ptr->tail + 1) % BUFFER_SIZE;
shared_ptr->count--;
                        
                        
sem_post(mutex);  
sem_post(empty);  
                                                
sleep(1);
    }
    

shmdt(shared_ptr);
sem_close(empty);
sem_close(full);
sem_close(mutex);
                
return 0;
  }

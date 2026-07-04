//Luis Ayala, CS3502, Assignment_2

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
                                

int producer_id = atoi(argv[1]);

int num_items = atoi(argv[2]);


int shm_id = shmget(SHM_KEY, sizeof(shared_buffer_t), IPC_CREAT | 0666);

if (shm_id < 0) {
perror("shmget failed");
return 1;
    }



shared_buffer_t* shared_ptr = (shared_buffer_t*)shmat(shm_id, NULL, 0);

  if (shared_ptr == (void*)-1) {
      perror("shmat failed");
      return 1;
          }
          

sem_t* empty = sem_open("/sem_empty", O_CREAT, 0644, BUFFER_SIZE);

sem_t* full = sem_open("/sem_full", O_CREAT, 0644, 0);

sem_t* mutex = sem_open("/sem_mutex", O_CREAT, 0644, 1);
                        
if (empty == SEM_FAILED || full == SEM_FAILED || mutex == SEM_FAILED) {
        perror("sem_open failed");
        return 1;
      }
      

for (int i = 0; i < num_items; i++) {
int calculated_value = producer_id * 1000 + i;

sem_wait(empty); 

sem_wait(mutex);

shared_ptr->buffer[shared_ptr->head].value = calculated_value;

shared_ptr->buffer[shared_ptr->head].producer_id = producer_id;

printf("Producer %d: Produced value %d\n", producer_id, calculated_value);

shared_ptr->head = (shared_ptr->head + 1) % BUFFER_SIZE;

shared_ptr->count++;

sem_post(mutex);  

sem_post(full);   

sleep(1);
    }
                                    

shmdt(shared_ptr);

sem_close(empty);

sem_close(full);

sem_close(mutex);

                                                        
  return 0;
}

/* Assignment 4: Multi-threaded Producer Consumer Pipeline
 * Started: 11/03/20
 * Finished: 11/16/20
 * Name: Susan Liu
 * Email: liusus@oregonstate.edu
 */
//input is max 1000 characters must output 80 characters
//code was inspired by example code given in assignment 4
//I have coppied get_buff and put_buff from the example code

//gcc --std=gnu99 -pthread -lm -o line_processor Assignment4.c

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h> // must link with -lm
#include <string.h>
#include <ctype.h>


// Size of the buffers
//final size
#define SIZE 80
#define BSIZE 49000
//max size
#define MAX_SIZE 1000
// Number of items that will be produced. This number is less than the size of the buffer. Hence, we can model the buffer as being unbounded.
#define NUM_ITEMS 49000

char str[MAX_SIZE];
char line[50];

// Buffer 1, shared resource between input thread and square-root thread
int buffer_1[BSIZE];
// Buffer 2, shared resource between square root thread and output thread
double buffer_2[BSIZE];
// Buffer 3, shared resource between square root thread and output thread
double buffer_3[BSIZE];
// Number of items in the buffer
int count_1 = 0;
// Number of items in the buffer
int count_2 = 0;
// Number of items in the buffer
int count_3 = 0;
// Number of items in the buffer
int count_4 = 0;
// Index where the input thread will put the next item
int con_idx_1 = 0;
// Index where the input thread will put the next item
int prod_idx_1 = 0;
// Index where the output thread will pick up the next item
int con_idx_2 = 0;
// Index where the input thread will put the next item
int prod_idx_2 = 0;
// Index where the output thread will pick up the next item
int con_idx_3 = 0;
// Index where the input thread will put the next item
int prod_idx_3 = 0;
// Index where the output thread will pick up the next item
int con_idx_4 = 0;
// Index where the input thread will put the next item
int prod_idx_4 = 0;
// Initialize the mutex for buffer 1
pthread_mutex_t mutex_1 = PTHREAD_MUTEX_INITIALIZER;
// Initialize the condition variable for buffer 1
pthread_cond_t full_1 = PTHREAD_COND_INITIALIZER;
// Initialize the mutex for buffer 2
pthread_mutex_t mutex_2 = PTHREAD_MUTEX_INITIALIZER;
// Initialize the condition variable for buffer 2
pthread_cond_t full_2 = PTHREAD_COND_INITIALIZER;
// Initialize the mutex for buffer 3
pthread_mutex_t mutex_3 = PTHREAD_MUTEX_INITIALIZER;
// Initialize the condition variable for buffer 3
pthread_cond_t full_3 = PTHREAD_COND_INITIALIZER;
// Initialize the mutex for buffer 3
pthread_mutex_t mutex_4 = PTHREAD_MUTEX_INITIALIZER;




char get_userinput(){
  char arr[MAX_SIZE];
  printf("\nPlease input in a string: \n\n");
  scanf("%[^\t]s\n",str);
  strcat(arr,str);
  
  printf("\n\n-------------------------------------");
  printf("\nInput is:");
  printf("\n-------------------------------------\n");  
  printf("%s", arr);
  strcpy(str,arr);
  return str;
}

void put_buff_1(char item){
  // Lock the mutex before putting the item in the buffer
  pthread_mutex_lock(&mutex_1);
  // Put the item in the buffer
  buffer_1[prod_idx_1] = item;
  // Increment the index where the next item will be put.
  prod_idx_1 = prod_idx_1 + 1;
  count_1++;
  // Signal to the consumer that the buffer is no longer empty
  pthread_cond_signal(&full_1);
  // Unlock the mutex
  pthread_mutex_unlock(&mutex_1);
}

void *get_input(void *args){
  char item = get_userinput();
  put_buff_1(item);
  return NULL;
}

//Get the next item from buffer 1
int get_buff_1(){
  // Lock the mutex before checking if the buffer has data
  pthread_mutex_lock(&mutex_1);
  while (count_1 == 0)
    // Buffer is empty. Wait for the producer to signal that the buffer has data
    pthread_cond_wait(&full_1, &mutex_1);
  char item = buffer_1[con_idx_1];
  // Increment the index from which the item will be picked up
  con_idx_1 = con_idx_1 + 1;
  count_1--;
  // Unlock the mutex
  pthread_mutex_unlock(&mutex_1);
  // Return the item
  return item;
}


void put_buff_2(char item){
  // Lock the mutex before putting the item in the buffer
  pthread_mutex_lock(&mutex_2);
  // Put the item in the buffer
  buffer_2[prod_idx_2] = item;
  // Increment the index where the next item will be put.
  prod_idx_2 = prod_idx_2 + 1;
  count_2++;
  // Signal to the consumer that the buffer is no longer empty
  pthread_cond_signal(&full_2);
  // Unlock the mutex
  pthread_mutex_unlock(&mutex_2);
}

char changeto_line(){
  int i=0;
  
  while(str[i]!='\0'){
    if(str[i]=='\n'){
      str[i]=' ';
    }
    i++;
  }
  
  printf("\n\n-------------------------------------");
  printf("\nSwitching enter with space:");
  printf("\n-------------------------------------\n");
  printf("%s\n",str);
  return str;
}

char *new_line(void *args){
  char item = get_buff_1();
  char change= changeto_line(); 
  put_buff_2(change);
  return NULL;
}




//Get the next item from buffer 2
char get_buff_2(){
  // Lock the mutex before checking if the buffer has data
  pthread_mutex_lock(&mutex_2);
  while (count_2 == 0)
    // Buffer is empty. Wait for the producer to signal that the buffer has data
    pthread_cond_wait(&full_2, &mutex_2);
  char item = buffer_2[con_idx_2];
  // Increment the index from which the item will be picked up
  con_idx_2 = con_idx_2 + 1;
  count_2--;
  // Unlock the mutex
  pthread_mutex_unlock(&mutex_2);
  // Return the item
  return item;
}
void put_buff_3(char item){
  // Lock the mutex before putting the item in the buffer
  pthread_mutex_lock(&mutex_3);
  // Put the item in the buffer
  buffer_3[prod_idx_3] = item;
  // Increment the index where the next item will be put.
  prod_idx_3 = prod_idx_3 + 1;
  count_3++;
  // Signal to the consumer that the buffer is no longer empty
  pthread_cond_signal(&full_3);
  // Unlock the mutex
  pthread_mutex_unlock(&mutex_3);
}

//change \n with a space
char switch_items(){
  int i = 0;
  int j = 0;
  
  while(str[i]!='\0'){
    if(str[i]=='+' && str[i+1]=='+'){ 
      str[i]='^';
      //Shift the array to the left
      for (j=i+1; j<MAX_SIZE-1; j++){
          str[j]=str[j+1];
      }
    }
    i++;
  }
  printf("\n\n-------------------------------------");
  printf("\nString After Replacing '++' by '^'");
  printf("\n-------------------------------------\n");
  return 0;
}

void *check_fortwo(void *args){
  char item = get_buff_2(item);
  char sitem=switch_items();
  put_buff_3(sitem);
  printf("%s\n", str);
  return NULL;
}



//Get the next item from buffer 3
char get_buff_3(){
  // Lock the mutex before checking if the buffer has data
  pthread_mutex_lock(&mutex_3);
  while (count_3 == 0)
    // Buffer is empty. Wait for the producer to signal that the buffer has data
    pthread_cond_wait(&full_3, &mutex_3);
  char item = buffer_3[con_idx_3];
  // Increment the index from which the item will be picked up
  con_idx_3 = con_idx_3 + 1;
  count_3--;
  // Unlock the mutex
  pthread_mutex_unlock(&mutex_3);
  // Return the item
  return item;
}
char create_output(){
  size_t len = strlen(str);
  
  printf("\n\n-------------------------------------");
  printf("\nSeperate by 80 char per line:");
  printf("\n-------------------------------------\n");
  
  for(int i=0; i<len; i++){
    if(str[i]==' ' && str[i+1]=='S' && str[i+2]=='T' &&
    str[i+3]=='O' && str[i+4]=='P' && str[i+5]==' '){
      return str;
    }
    
    printf("%c", str[i]);
    
  	if(i % SIZE == SIZE-1 && i!=0){
      printf("\n");
    }
  }
  return 0;
}

void get_output(void *args){
  char items;
  
  items = get_buff_3();
  create_output();
  
  printf("\n\n\n");
  return NULL;
}




int main(){
  srand(time(0));
  pthread_t input_t, lineSeparate_t, plusSign_t, output_t;
  
  //create threads
  pthread_create(&input_t, NULL, get_input,NULL);
  pthread_create(&lineSeparate_t, NULL, new_line,NULL);
  pthread_create(&plusSign_t, NULL, check_fortwo,NULL);
  pthread_create(&output_t, NULL, get_output,NULL);

  //wait for the threads to terminate
  pthread_join(input_t, NULL);
  pthread_join(lineSeparate_t, NULL);
  pthread_join(plusSign_t, NULL);
  pthread_join(output_t, NULL);  
  return EXIT_SUCCESS;
}

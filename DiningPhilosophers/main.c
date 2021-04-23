#include <pthread.h> 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N_PHILOSOPHERS 5

enum pstate {THINKING,HUNGRY,EATING};  //Philosophers situations 
enum pstate state[5];

/*Global Variables*/
int philosopher_id[N_PHILOSOPHERS];   //Philosophers id numbers   
pthread_cond_t condition[N_PHILOSOPHERS];//condition variables
pthread_mutex_t mutex_lock;              //MUtex Lock to avoid functions are accessed by different threads at the same time
pthread_t philosophers[N_PHILOSOPHERS]; // Representation of philosophers as different threads


void *philosopher(void *prm);//Actions of philosophers
void test(int i);           //Check for other philosophers
void pickup_forks(int philo_id); //Waiting in order to eat
void putdown_forks(int philo_id); //Release the eating order
extern inline int check_left(int philo_id); //Returns left neighbor's id 
extern inline int check_right(int philo_id);//Returns right neighbor's id


int main(void)
{
  int i;
  for(i=0; i<N_PHILOSOPHERS; i++){ //Suppose that all philosophers at THINKING state at the begining
     state[i]=THINKING;
     philosopher_id[i]=i;          //Philosopher id held as an array because that prevents mutual exclusion.
     pthread_cond_init(&condition[i],NULL);// Initializes condition variables.
  }
  
  pthread_mutex_init(&mutex_lock,NULL); //Initializes mutex locks
  
  for(i=0; i<N_PHILOSOPHERS; i++){
    pthread_create(&philosophers[i],0,philosopher,(void*)&philosopher_id[i]); //Creates and starts philosopher's threads 
  }
  
  for(i=0; i<N_PHILOSOPHERS; i++)
     pthread_join(philosophers[i],NULL); //Waiting for the thread's jobs done (Also it doesnt finish because we used infinite loop )
     
  return 0;
} 



inline int check_left(int philo_id) //Returns left philospher's id 
{
  return ((philo_id == 0) ? N_PHILOSOPHERS-1 : philo_id-1);
}

inline int check_right(int philo_id)//Returns right philosopher's id
{
  return ((philo_id == N_PHILOSOPHERS-1) ? 0 : philo_id+1);
}

void test(int i)
{
  if( (state[i]==HUNGRY)           //Checking first herself:HUNGRY? and Chechking for the left and right neighbors ,Are they eating or not?
    && (state[check_left(i) ] !=EATING)
    && (state[check_right(i)] !=EATING) ){
       
       state[i]=EATING;                   //If not, can eat.
       pthread_cond_signal(&condition[i]);
     }
}

void pickup_forks(int philo_id)
{
  pthread_mutex_lock(&mutex_lock);// Locks the fuction to prevent function is accessed by multiple threads at the same time.
  
  state[philo_id]=HUNGRY; //Set the philosopher state as HUNGRY after some THINKING.
  test(philo_id);
  
  while(state[philo_id] !=EATING) { //If the neighbors are eating
  
       pthread_cond_wait(&condition[philo_id],&mutex_lock); //Wait for the release lock
    }
    /*If the while loop is broken it points that philosopher state is changed to EATING.*/
    printf("Philosopher %d is eating\n",philo_id); 
      
    pthread_mutex_unlock(&mutex_lock); //Release the function lock.
}

void putdown_forks(int philo_id)
{
   pthread_mutex_lock(&mutex_lock);// Locks the fuction to prevent function is accessed by multiple threads at the same time.
   state[philo_id]=THINKING;  //THINKING state
   /*When the philosopher finish EATING it checks his neighbors if they are HUNGRY and he gives eating order to them*/
   test(check_left(philo_id));//If left neighbor is HUNGRY or Not
   test(check_right(philo_id));//IF right neighbor is HUNGRY or Not
   
   pthread_mutex_unlock(&mutex_lock); //Release the function lock.
}


void *philosopher(void *prm)
{
  int *philo_id = prm; //Taken thread's id as a prm(parameter) 
  int sleep_time;
  
  srandom((unsigned) time(NULL)); //Initialize the random fuction to generate the numbers more efficently.
  
  while(1){
  
      printf("Philosopher %d is thinking\n",*philo_id); 
      
      sleep_time=(int)((random() % 2)+1); //Sleep during a random time [1-3] sec.(when while loop is done its turn back because of sleep) 
      sleep(sleep_time);
      
      pickup_forks(*philo_id);  
      sleep_time=(int)((random() % 2)+1); //Again philosopher sleeps during a random time for EATING
      sleep(sleep_time);
      
      putdown_forks(*philo_id);
      
      /* While loop makes this processes infinitely.*/ 
      }
 }

     

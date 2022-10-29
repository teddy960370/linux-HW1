#include<pthread.h>
#include<stdio.h>
#include<unistd.h>

enum action { upstairs = 0 , downstairs = 1 };

static enum action cat = 0;

static pthread_mutex_t __mutex_shared_variable = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;


void * thread_task_1() {

	while (1) {
		pthread_mutex_lock(&__mutex_shared_variable);   // Start critical section
		{
			cat = upstairs;
		}
		pthread_mutex_unlock(&__mutex_shared_variable); // End critical section
		sleep(7);
	}
}

void * thread_task_2() {

	while (1) {
		pthread_mutex_lock(&__mutex_shared_variable);   // Start critical section
		{
			cat = downstairs;
		}
		pthread_mutex_unlock(&__mutex_shared_variable); // End critical section
		sleep(3);
	}
}

void * main_thread_task() {

	while (1) {
		pthread_mutex_lock(&__mutex_shared_variable);   // Start critical section
		{
			if(cat == upstairs)
				printf("The cat is upstairs\n");
			else
				printf("The cat is downstairs\n");
		}
		pthread_mutex_unlock(&__mutex_shared_variable); // End critical section
		sleep(1);
	}
}


int main(int argc, char **argv)
{
    // create the thread objs
    pthread_t main_thread,thread_1, thread_2;

    // start the threads
	pthread_create(&main_thread, NULL, *main_thread_task,NULL);
    pthread_create(&thread_1, NULL, *thread_task_1,NULL);
    pthread_create(&thread_2, NULL, *thread_task_2,NULL);
	
    // wait for threads to finish
	pthread_join(main_thread,NULL);
    pthread_join(thread_1,NULL);
    pthread_join(thread_2,NULL);
    return 0;
}
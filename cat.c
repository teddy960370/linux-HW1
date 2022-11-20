#include<pthread.h>
#include<stdio.h>
#include<unistd.h>

static int flag = 0;

static pthread_mutex_t __mutex_shared_variable = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;

struct data_segment
{
    unsigned long start_code;
    unsigned long end_code;
	unsigned long start_data;
	unsigned long end_data;
	unsigned long start_brk;
	unsigned long brk;
	unsigned long start_stack;	
};

void print_data_segment(struct data_segment my_data_segment)
{
	unsigned long pa = 0;
	syscall(351 , my_data_segment.start_code , (void*)&pa);
	printf("start_code \t virtual address : 0x%lx, physical address : 0x%lx\n",my_data_segment.start_code,pa);
	syscall(351 , my_data_segment.end_code , (void*)&pa);
	printf("end_code \t virtual address : 0x%lx, physical address : 0x%lx\n",my_data_segment.end_code,pa);
	syscall(351 , my_data_segment.start_data , (void*)&pa);
	printf("start_data \t virtual address : 0x%lx, physical address : 0x%lx\n",my_data_segment.start_data,pa);
	syscall(351 , my_data_segment.end_data , (void*)&pa);
	printf("end_data \t virtual address : 0x%lx, physical address : 0x%lx\n",my_data_segment.end_data,pa);
	syscall(351 , my_data_segment.start_brk , (void*)&pa);
	printf("start_brk \t virtual address : 0x%lx, physical address : 0x%lx\n",my_data_segment.start_brk,pa);
	syscall(351 , my_data_segment.brk , (void*)&pa);
	printf("brk \t virtual address : 0x%lx, physical address : 0x%lx\n",my_data_segment.brk,pa);
	syscall(351 , my_data_segment.start_stack , (void*)&pa);
	printf("start_stack \t virtual address : 0x%lx, physical address : 0x%lx\n",my_data_segment.start_stack,pa);
}

void * thread_task_1() {
	struct data_segment my_data_segment;
	syscall(351 , getpid() , (void*)&my_data_segment);
	
	while (1) {
		pthread_mutex_lock(&__mutex_shared_variable);   // Start critical section
		{
			if(flag == 0)
			{
				print_data_segment(my_data_segment);
				flag = 1;
			}
			else
				continue;
		}
		pthread_mutex_unlock(&__mutex_shared_variable); // End critical section
	}
}

void * thread_task_2() {
	struct data_segment my_data_segment;
	syscall(351 , getpid() , (void*)&my_data_segment);
	
	while (1) {
		pthread_mutex_lock(&__mutex_shared_variable);   // Start critical section
		{
			if(flag == 1)
			{
				print_data_segment(my_data_segment);
				flag = 2;
			}
			else
				continue;
		}
		pthread_mutex_unlock(&__mutex_shared_variable); // End critical section
	}
}

void * thread_task_3() {
	struct data_segment my_data_segment;
	syscall(351 , getpid() , (void*)&my_data_segment);
	
	while (1) {
		pthread_mutex_lock(&__mutex_shared_variable);   // Start critical section
		{
			if(flag == 2)
			{
				print_data_segment(my_data_segment);
				flag = 3;
			}
			else
				continue;
		}
		pthread_mutex_unlock(&__mutex_shared_variable); // End critical section
	}
}


int main(int argc, char **argv)
{
	struct data_segment my_data_segment;
    // create the thread objs
    pthread_t thread_1,thread_2, thread_3;
	unsigned long physical_address = 0;
	
    // start the threads
    pthread_create(&thread_1, NULL, *thread_task_1,NULL);
    pthread_create(&thread_2, NULL, *thread_task_2,NULL);
	pthread_create(&thread_3, NULL, *thread_task_3,NULL);
	
	syscall(351 , getpid() , (void*)&my_data_segment);
	print_data_segment(my_data_segment);
	
	
	syscall(351 , &thread_1 , (void*)&physical_address);
	printf("thread_1 \t virtual address : 0x%lx, physical address : 0x%lx\n",&thread_1,physical_address);
	syscall(351 , &thread_2 , (void*)&physical_address);
	printf("thread_2 \t virtual address : 0x%lx, physical address : 0x%lx\n",&thread_2,physical_address);
	syscall(351 , &thread_3 , (void*)&physical_address);
	printf("thread_3 \t virtual address : 0x%lx, physical address : 0x%lx\n",&thread_3,physical_address);
	
	
    // wait for threads to finish
    pthread_join(thread_1,NULL);
    pthread_join(thread_2,NULL);
	pthread_join(thread_3,NULL);
    return 0;
}
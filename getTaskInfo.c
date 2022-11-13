#include<stdio.h>
#include <syscall.h>
#include<linux/kernel.h>
#include <sys/types.h>
#include<unistd.h>

struct thread_segment
{
	pid_t pid;
	unsigned long stack_pos;
	unsigned long file_pos;
};

struct vma_segment
{
	unsigned long vm_start;
	unsigned long vm_end;
	char ret_ptr[64];
};

struct data_segment
{
	struct thread_segment thread_main;
	struct thread_segment thread_1;
	struct thread_segment thread_2;
	struct thread_segment thread_3;
	
	int user_number;
    unsigned long start_code;
    unsigned long end_code;
	unsigned long start_data;
	unsigned long end_data;
	unsigned long start_brk;
	unsigned long brk;
	unsigned long start_stack;
	
	struct vma_segment VMA[30];
	
};

int main(int argc, char **argv)
{
	if(argc <= 1)
	{
		printf("Parameter Error, please just input one pid\n");
		return -1;
	}
	
	int pid;
	sscanf(argv[1], "%d", &pid);

	struct data_segment my_data_segment;
	
    int a = syscall(351 , pid , (void*)&my_data_segment);
    printf("return code is : %d\n",a);
	
	printf("Task User Number : %d\n",my_data_segment.user_number);
	printf("Task start code address : %#lx\n",my_data_segment.start_code);
	printf("Task end code address : %#lx\n",my_data_segment.end_code);
	printf("Task start data address : %#lx\n",my_data_segment.start_data);
	printf("Task end data address : %#lx\n",my_data_segment.end_data);
	printf("Task start heap address : %#lx\n",my_data_segment.start_brk);
	printf("Task end heap address : %#lx\n",my_data_segment.brk);
	printf("Task start stack address : %#lx\n\n",my_data_segment.start_stack);
	
	printf( "main thread PID : %d \n",my_data_segment.thread_main.pid);
	printf( "main thread stack pos : %#lx \n",my_data_segment.thread_main.stack_pos );
	printf( "main thread file pos : %#lx \n\n",my_data_segment.thread_main.file_pos );
	
	printf( "thread1 PID : %d \n",my_data_segment.thread_1.pid);
	printf( "thread1 stack pos : %#lx \n",my_data_segment.thread_1.stack_pos );
	printf( "thread1 file pos : %#lx \n\n",my_data_segment.thread_1.file_pos );
	
	printf( "thread2 PID : %d \n",my_data_segment.thread_2.pid);
	printf( "thread2 stack pos : %#lx \n",my_data_segment.thread_2.stack_pos );
	printf( "thread2 file pos : %#lx \n\n",my_data_segment.thread_2.file_pos );
	
	printf( "thread3 PID : %d \n",my_data_segment.thread_3.pid);
	printf( "thread3 stack pos : %#lx \n",my_data_segment.thread_3.stack_pos );
	printf( "thread3 file pos : %#lx \n\n",my_data_segment.thread_3.file_pos );
	
	/*
	for(int i = 0 ; i < 30 ; i++)
	{
		printf("VMA %d: %#lx ~ %#lx \t %s",i,my_data_segment.VMA[i].vm_start,my_data_segment.VMA[i].vm_end,my_data_segment.VMA[i].ret_ptr);
	}*/
	
    return 0;
}

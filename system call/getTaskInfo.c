#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/pid.h>
#include <asm/io.h>
#include <asm/current.h>
#include <asm/pgtable.h>
#include <linux/highmem.h>
#include <linux/syscalls.h>
#include<linux/slab.h>

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

asmlinkage int sys_getTaskInfo(pid_t user_pid, void* __user user_address)
{
    struct data_segment result;
    struct task_struct *task;
	struct mm_struct *mm;
	struct vm_area_struct *vma;
	struct thread_struct mythread;
	struct path base_path;
	char * ret_ptr = NULL;
	char * tpath   = NULL ;
	struct task_struct *thread = NULL;
	
	int count ;
	
	for_each_process(task) {
		
		
		if ( task->pid == user_pid ){ 
		
			thread = task;
			count = 0;
			while_each_thread(task, thread)
			{
				printk(KERN_INFO "Task Name : %s \n",thread->comm);
				printk(KERN_INFO "Task PID : %d \n",thread->pid);
				
				mythread = thread -> thread;
				printk(KERN_INFO "thread sp : %#lx \n",mythread.sp );
				printk(KERN_INFO "thread fsbase : %#lx \n",mythread.ip );
				if(count == 0)
				{
					result.thread_1.pid = thread->pid;
					result.thread_1.stack_pos = thread->thread.sp;
					result.thread_1.file_pos = thread->thread.ip;
				}
				else if(count == 1)
				{
					result.thread_2.pid = thread->pid;
					result.thread_2.stack_pos = thread->thread.sp;
					result.thread_2.file_pos = thread->thread.ip;
				}
				else if(count == 2)
				{
					result.thread_3.pid = thread->pid;
					result.thread_3.stack_pos = thread->thread.sp;
					result.thread_3.file_pos = thread->thread.ip;
				}
				count ++;
			}
		
		   result.thread_main.pid = task->pid;
		   result.thread_main.stack_pos = task->thread.sp;
		   result.thread_main.file_pos = task->thread.ip;
		   
		   mm = task->mm;
		   result.user_number = mm -> mm_users.counter;
		   result.start_code = mm -> start_code;
		   result.end_code = mm -> end_code;
		   result.start_data = mm -> start_data;
		   result.end_data = mm -> end_data;
		   result.start_brk = mm -> start_brk;
		   result.brk = mm -> brk;
		   result.start_stack = mm -> start_stack;
		   
		   printk(KERN_INFO "Task Name : %s \n",task->comm);
		   printk(KERN_INFO "Task PID : %d \n",task->pid);
		   
		   
		   
		   //printk(KERN_INFO "mm->pgd : %#lx \n",mm->pgd );
		   printk(KERN_INFO "Task User Number : %d \n",mm -> mm_users );
		   printk(KERN_INFO "mm->start_code : %#lx \n",mm->start_code );
		   printk(KERN_INFO "mm->end_code : %#lx \n",mm->end_code );
		   printk(KERN_INFO "mm->start_data : %#lx \n",mm->start_data );
		   printk(KERN_INFO "mm->end_data : %#lx \n",mm->end_data );
		   printk(KERN_INFO "mm->start_brk : %#lx \n",mm->start_brk );
		   printk(KERN_INFO "mm->brk : %#lx \n",mm->brk );
		   printk(KERN_INFO "mm->start_stack : %#lx \n",mm->start_stack );
		   
		   
		   vma = mm -> mmap;
		   count = 0;
		   while(vma -> vm_next != NULL)
		   {
				//printk(KERN_INFO "VMA %d start : %#lx \n" , count , vma -> vm_start );
				//printk(KERN_INFO "VMA %d end : %#lx \n" , count , vma -> vm_end );
				
				tpath = (char*)kmalloc(64, 0);
				if(NULL == tpath || NULL == task)
				{
					return NULL;
				}
				memset(tpath,'\0',64);
				
				if (vma->vm_file)
				{    
					base_path = vma->vm_file->f_path;
					ret_ptr = d_path(&base_path, tpath, 64);
					//printk(KERN_INFO "VMA %d File Path : %s \n" , count , ret_ptr);
				}
				else
					ret_ptr = NULL;
				
				printk(KERN_INFO "VMA %d : %#lx ~ %#lx \t %s \n" , count ,vma -> vm_start , vma -> vm_end , ret_ptr);
				/*
				result.VMA[count].vm_start = vma -> vm_start;
				result.VMA[count].vm_end = vma -> vm_end;
				memcpy(result.VMA[count].ret_ptr, ret_ptr,sizeof(result.VMA[count].ret_ptr));
				*/
				vma = vma -> vm_next;
				count++;
		   }
		   
		   mythread = task -> thread;
		   printk(KERN_INFO "thread end : %#lx \n",mythread.sp );
		   
		   copy_to_user(user_address, &result , sizeof(struct data_segment));
		   
		}
	}
	 
    return 1;
}

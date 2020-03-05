#include <linux/module.h>  
#include <linux/module.h>  
#include <linux/kernel.h>  
#include <linux/init.h>  
#include <linux/fs.h>  
#include <linux/cdev.h>  
#include <linux/device.h>  
#include <linux/uaccess.h>   
#include <linux/pid.h>   
#include <linux/sched.h>   
#include <linux/sched/signal.h>   
  
#define HELLO_MAJOR 0 
#define HELLO_MINOR 0  
#define NUMBER_OF_DEVICES 1  
  
struct class *hello_class;  
static struct cdev cdev;  
dev_t devno;  
  
static ssize_t hello_write(struct file *file, const char __user *buf, size_t count,  
                loff_t *ppos)  
{  
	char str[64];  
  	int i=0,pid=0;
	struct task_struct* p,*q;
	if(count < 64){
		copy_from_user(str,buf,count);  
	}
	for(i=0; i < count; i++){
		if(str[i] <= '9' && str[i] >= '0'){
			pid = pid * 10 + str[i] - '0';
		}else
			break;
	}
	if(pid > 0){
		rcu_read_lock();
		q = p = pid_task(find_pid_ns(pid,&init_pid_ns), PIDTYPE_PID);
		if(p){

			printk(KERN_INFO "process %d have thread %d,\n",pid, p->pid);
			while_each_thread(p, q){
				printk(KERN_INFO "thread %d,\n", q->pid);
			}
		}
		rcu_read_lock();
	}
	return count;  
}  
  
static int hello_open(struct inode *inode,struct file *file)  
{  
    return 0;   
}  
  
static const struct file_operations hello_fops = {  
        .open = hello_open,  
        .write = hello_write,  
        .owner = THIS_MODULE,  
};  
  
static int __init hello_init(void)  
{  
    int ret;  
    devno = MKDEV(HELLO_MAJOR,HELLO_MINOR);  
  
    if(HELLO_MAJOR){  
        ret = register_chrdev_region(devno,NUMBER_OF_DEVICES,"chrdev");  
    }else{  
        ret = alloc_chrdev_region(&devno, 0, NUMBER_OF_DEVICES, "chrdev");  
    }  
    if(ret < 0){  
        printk("%s register chrdev error\n",__func__);  
        return ret;  
    }  
  
    hello_class = class_create(THIS_MODULE,"hello_char_calss");  
    if(IS_ERR(hello_class)){  
        printk("%s create class error\n",__func__);  
        return -1;  
    }  
  
    device_create(hello_class, NULL, devno, NULL, "chrdev");  
      
      
    cdev_init(&cdev, &hello_fops);  
    cdev.owner = THIS_MODULE;  
    cdev_add(&cdev, devno, NUMBER_OF_DEVICES);  
  
    return 0;  
}  
  
  
static void __exit hello_exit(void)  
{  
    printk("%s",__func__);  
    cdev_del(&cdev);  
    device_destroy(hello_class,devno);  
    class_destroy(hello_class);  
    unregister_chrdev_region(devno,NUMBER_OF_DEVICES);  
  
}  
  
module_init(hello_init);  
module_exit(hello_exit);  
MODULE_LICENSE("GPL");  
MODULE_AUTHOR("oracleloyal@gmail.com"); 


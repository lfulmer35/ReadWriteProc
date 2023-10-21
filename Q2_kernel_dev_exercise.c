#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

#define MAX_PROC_SIZE 100
static char proc_data[MAX_PROC_SIZE];
size_t len = sizeof(proc_data) / sizeof(char) - 1;


MODULE_LICENSE("FREEFREEFREE");
MODULE_AUTHOR("John Q. Public");
MODULE_DESCRIPTION("Creates a proc and other stuff too!!");
MODULE_VERSION("1.0");


static int hello_proc_show(struct seq_file *m, void *v) {
  seq_printf(m, "Hello proc!\n");
  return 0;
}

static int hello_proc_open(struct inode *inode, struct  file *file) {
  return single_open(file, hello_proc_show, NULL);
}

/*
Summary:
    This function will be called each time we attempt to read from our proc 
    (e.g. cat /proc/hello_proc). 
*/
static ssize_t read_proc(struct file *filp, char __user *buf, size_t size, loff_t *off)
{
    loff_t offset = *off;
    size_t remaining;

    pr_info("proc file read\n");
    
    if (offset < 0)
        return -EINVAL;

    if (offset >= len || size == 0)
        return 0;

    if (size > len - offset)
        size = len - offset;

    remaining = copy_to_user(buf, proc_data + offset, size);

    if (remaining == size) {
        pr_err("copy_to_user failed\n");
        return -EFAULT;
    }

    size -= remaining;
    *off = offset + size;
    return size;
}

/*
*/
static ssize_t write_proc(struct file *filp, const char *buf, size_t size, loff_t *off)
{
    loff_t offset = *off;
    size_t remaining;
    
    pr_info("proc file write\n");

    if (offset < 0)
        return -EINVAL;

    if (offset >= len || size == 0)
        return 0;

    if (size > len - offset)
        size = len - offset;

    remaining = copy_from_user(proc_data + offset, buf, size);
    if (remaining == size) {
        pr_err("copy_from_user failed\n");
        return -EFAULT;
    }

    size -= remaining;
    *off = offset + size;
    return size;
}

static int release_proc(struct inode *inode, struct file *file)
{
    pr_info("proc file released...\n");
    return 0;
}

static const struct proc_ops hello_proc_fops = {
    .proc_open = hello_proc_open,
    .proc_read = read_proc,
    .proc_write = write_proc,
    .proc_release = release_proc,
};

static int __init hello_proc_init(void) {
  proc_create("hello_proc", 0777, NULL, &hello_proc_fops);
  return 0;
}

static void __exit hello_proc_exit(void) {
  remove_proc_entry("hello_proc", NULL);
}

module_init(hello_proc_init);
module_exit(hello_proc_exit);

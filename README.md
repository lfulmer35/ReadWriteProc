# ReadWriteProc
Simple kernel module which creates a proc for read and write operations


# Usage
From top dir:
make
sudo insmod Q2_kernel_dev_exercise.ko

Read operation:
cat /proc/hello_proc (should display a message)

Write operation:
echo "Some message" > /proc/hello_proc

sudo rmmod Q2_kernel_dev_exercise

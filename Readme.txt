Objectives:
The objectives of this assignment are the following: 
Acquire familiarity with using POSIX thread management primitives. 
How to write POSIX thread programs and communicate through shared memory. 
Read the manual pages for the LINUX primitives for creating shared memory between two LINUX processes. 
These functions are -- shmget and shmat. 
For sample programs on how to use the POSIX thread library and its shared memory management, 
you can use Google to search for "POSIX Thread Programming Examples".

Problem Statement:
In this assignment you will write a POSIX program. 
Consider a system comprising of three producer threads, one consumer thread, and a shared buffer of size 2. 
Each producer thread randomly (over time) creates a specific colored item: 
producer_red creates red colored items, producer_black creates black colored items, and producer_white creates white-colored items. 
Associated with each item created by a specific producer is a local LINUX timestamp in microseconds (use the gettimeofday() function) 
which records the time that item was placed into the buffer. 
Each producer deposits its item into the buffer, and the consumer retrieves the items from the buffer. 
The items produced by the producers will be of any of the three strings below:
“RED timestamp”, “BLACK timestamp”, “WHITE timestamp”
Each producer threads, after successfully depositing an item into the buffer, 
will write that item (essentially the “COLOR timestamp” string) into its log file called Producer_COLOR.txt (e.g. Producer_RED.txt). 
The consumer thread will then retrieve the items from this shared buffer. 
When it retrieves an item, the consumer thread will write that item (essentially the “COLOR timestamp” string) into a file called Consumer.txt.

Requirements for the program:
1. Each producer thread will produce its respective colored item (represented as a string “COLOR timestamp”). 
If the producer thread finds the shared buffer has space, it will first create a timestamp for that item. 
The producer thread will then write the item into its log file (Producer_COLOR.txt) and also deposit the item into the buffer. 
If the buffer does not have space, the producer will have to wait. 
Each string (or item) written into the log file will be on a new line.
2. The consumer thread will take an item from the buffer, and write its contents to Consumer.txt file. 
Each string should be written in the order it was read from the buffer, and in a new line. 
If the buffer was initially full, the consumer thread should signal any one of the producer threades.
3. Each producer thread should produce 1000 items. 
Hence at the end of running the program, the consumer should have written 3000 items into its log file.
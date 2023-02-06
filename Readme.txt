Author: Jiaqian Huang

Objectives:
1. Acquire familiarity with using POSIX thread management primitives. 
2. How to write POSIX thread programs and communicate through shared memory. 

Problem Statement:
Write a POSIX program. 
3 producer threads, 1 consumer thread, and a shared buffer of size 2. 
Each producer deposits its item into the buffer, and the consumer retrieves the items from the buffer. 
Each producer thread randomly (over time) creates a specific colored item: 
producer_red creates red colored items, producer_black creates black colored items, and producer_white creates white-colored items. 
Associated with each item created by a specific producer is a local LINUX timestamp in microseconds which records the time that item was placed into the buffer. 
The items produced by the producers will be of any of the three strings below: “RED timestamp”, “BLACK timestamp”, “WHITE timestamp”
Each producer threads, after successfully depositing an item into the buffer, will also write that item into its log file called Producer_COLOR.txt (e.g. Producer_RED.txt). 
The consumer thread will write that item (essentially the “COLOR timestamp” string) into a file called Consumer.txt.

Requirements for the program:
1. Each producer thread will produce its respective colored item (represented as a string “COLOR timestamp”). 
If the producer thread finds the shared buffer has space, it will first create a timestamp for that item. 
The producer thread will then write the item into its log file (Producer_COLOR.txt) in a new line and also deposit the item into the buffer. 
If the buffer does not have space, the producer will have to wait. 
2. The consumer thread will take an item from the buffer, and write its contents to Consumer.txt file. 
Each string should be written in the order it was read from the buffer, and in a new line. 
If the buffer was initially full, the consumer thread should signal any one of the producer threades.
3. Each producer thread should produce 1000 items. Hence at the end of running the program, the consumer should have written 3000 items into its log file.

How to run the code:
1. prepare a linix environment
2. open the terminal and cd into the project folder
3. run `$ make`
4. run `./main`
5. then the programming is executed and printing items into 4 txt files.
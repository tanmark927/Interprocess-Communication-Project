/**
 * Mark Tan
 * CECS 326 Programming Project
 * Sender One
 * This sender generates a random number divisible by 251
 * and sends it to a receiver. To terminate this program,
 * use the kill function.
 */

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include "getinfo.h"
#include <cstdlib>
using namespace std;
int ref_num = 251;
int ref_num_a = 500;

int main() {
	struct buf {
		long mtype;
		int source;
		int event_num;
		char message[50];
	};

	buf msg;
	int event, rand_num;
	int size = sizeof(msg)-sizeof(long);
	int qid = msgget(ftok(".",'u'), 0);

	do{	
		//generates random number divisible by 251
		do{
			rand_num = rand();		
			event = rand_num;	
		}while(rand_num % ref_num != 0);
			
		//creates and sends message to queue
		msg.event_num = event;
		cout << getpid() << ": sends event" << endl;
		msg.mtype = ref_num_a;
		msg.source = ref_num;
		msgsnd(qid, (struct msgbuf *)&msg, size, 0);
		cout << endl;
		sleep(1);
	}while(event != 0);

	//sends termination message to receiver
	strcpy(msg.message, "terminate");
	get_info(qid, (struct msgbuf *)&msg, size, ref_num_a);
}

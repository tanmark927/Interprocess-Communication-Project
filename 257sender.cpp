/**
 * Mark Tan
 * CECS 326 Programming Project
 * Sender Three
 * This sender generates a random number divisible by 257
 * and sends it to a receiver.
 */

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
using namespace std;
int ref_num = 257;
int ref_num_b = 750;

int main() {
	struct buf {
		long mtype;
		int source;
		int event_num;
		char message[50];
	};

	buf msg;
	int event, rand_num;
	int qid = msgget(ftok(".",'u'), 0);
	int size = sizeof(msg)-sizeof(long);

	do{
		//generates random number divisible by 257
		do{
			rand_num = rand();		
			event = rand_num;	
		}while(rand_num % ref_num != 0);
		string xx = string(msg.message),yy=string("terminate");
		if(xx == yy) {
			break;
		}

		//creates and sends message to queue
		sleep(1);
		msg.event_num = event;
		cout << getpid() << ": sends event" << endl;
		msg.mtype = ref_num_b;
		msg.source = ref_num;
		msgsnd(qid, (struct msgbuf *)&msg, size, 0);
		
		//receive messsage from 997 & 257 receiver to terminate
		//and exit program
		msgrcv (qid, (struct msgbuf *)&msg, size, 300, 0);
		cout << msg.message << endl;
		cout << endl;
	}while(msg.message != "terminate");
	//delete message queue
	cout << getpid() << ": now exits" << endl;
	msgctl (qid, IPC_RMID, NULL);
	exit(0);
}

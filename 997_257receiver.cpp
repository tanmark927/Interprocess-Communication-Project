/**
 * Mark Tan
 * CECS 326 Programming Project
 * Receiver Two
 * This receiver gets a message from 997 and 257 senders
 * and displays the message and sender.
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

int main() {
	struct buf {
		long mtype;
		int source;
		int event_num;
		char message[50];
	};

	buf msg;
	int size = sizeof(msg)-sizeof(long);
	int qid = msgget(ftok(".",'u'), 0);
	int msg_count;

	do{
		//reads message
		msgrcv(qid, (struct msgbuf *)&msg, size, 750, 0); 
		cout << getpid() << ": gets event" << endl;
		cout << "source: " << msg.source << endl;
		cout << "event: " << msg.event_num << endl;
		if(msg.event_num != 0)
			++msg_count;
		cout << "Messages Received: " << msg_count << endl;
		
		//sends ack message to sender
		strcpy(msg.message, "Got it!");
		cout << getpid() << ": sends ack message to " << msg.source << endl;
		msg.mtype = 300;
		msgsnd(qid, (struct msgbuf *)&msg, size, 0);
		cout << endl;
	}while(msg_count < 150);

	strcpy(msg.message, "terminate");
	msg.mtype = 300;
	msgsnd(qid, (struct msgbuf *)&msg, size, 0);
	cout << getpid() << ": now exits" << endl;
	exit(0);
}

/**
 * Mark Tan
 * CECS 326 Programming Project
 * Receiver One
 * This receiver gets a message from 251 and 997 senders
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
	//char msg_sOne[50], msg_sTwo[50];
	int size = sizeof(msg)-sizeof(long);
	//creates message queue
	int qid = msgget(ftok(".",'u'), IPC_EXCL|IPC_CREAT|0600);

	 do{
		//read message
		msgrcv(qid, (struct msgbuf *)&msg, size, 500, 0);
		cout << getpid() << ": gets event" << endl;
		cout << "source: " << msg.source << endl;
		cout << "event: " << msg.event_num << endl;
		string xx = string(msg.message),yy=string("terminate");
		if(yy == xx) {
			break;
		}
		
		//sends ack message to sender
		strcpy(msg.message, "Got it!");
		cout << getpid() << ": sends ack message to " << msg.source << endl;
		msg.mtype = 320;
		msgsnd(qid, (struct msgbuf *)&msg, size, 0);
		cout << endl;
	}while(msg.message != "terminate");

	//if messages say to terminate, then exit program
	cout << getpid() << ": now exits" << endl;
	exit(0);
}

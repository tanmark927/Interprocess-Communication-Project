/**
 * Mark Tan
 * CECS 326 Programming Project
 * Sender Two
 * This sender generates a random number divisible by 997
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
int ref_num = 997;
int ref_num_a = 500;
int ref_num_b = 750;

int main() {
	struct buf {
		long mtype;
		int source;
		int event_num;
		char message[50];
	};

	buf msg;
	int rand_num;
	int event = 100;
	int loop = 0;	//used in case rand() does not generate an event less than 100
	char msg_rOne[50], msg_rTwo[50];
	int qid = msgget(ftok(".",'u'), 0);
	int size = sizeof(msg)-sizeof(long);
	
	do{
		if(loop >= 50)
			event = 10;
		else{
			//generates random number divisible by 997
			do{
				rand_num = rand();		
				event = rand_num;
			}while(rand_num % ref_num != 0);
		}
		if(event < 100) {
			cout << "Event smaller than 100" << endl;
			break;
		}
		sleep(1);
		//creates and sends message to queue for 251/997 receiver
		msg.event_num = event;
		cout << getpid() << ": sends event for 251/997 receiver" << endl;
		msg.mtype = ref_num_a;
		msg.source = ref_num;
		msgsnd(qid, (struct msgbuf *)&msg, size, 0);
		
		//creates and sends message to queue for 997/257 receiver
		msg.event_num = event;
		cout << getpid() << ": sends event for 997/257 receiver" << endl;
		msg.mtype = ref_num_b;
		msg.source = ref_num;
		msgsnd(qid, (struct msgbuf *)&msg, size, 0);

		//receives acknowledgement from 251 & 997 reciever
		msgrcv(qid, (struct msgbuf *)&msg, size, 320, 0);
		cout << getpid() << ": gets ack from 251/997 receiver" << endl;
		cout << "reply: " << msg.message << endl;
		strcpy(msg_rOne, msg.message);
		
		//receives acknowledgement from 997 and 257 reciever
		msgrcv(qid, (struct msgbuf *)&msg, size, 300, 0);
		cout << getpid() << ": gets ack from 997/257 receiver" << endl;
		cout << "reply: " << msg.message << endl;
		strcpy(msg_rTwo, msg.message);
		loop++;
		cout << endl;

		/*sends message back to receivers once it gets
		  ack messages from 251 & 997 receiver and 997 & 257 receiver*/
		if(msg_rOne != NULL && msg_rTwo != NULL) {
			continue;
		}
	}while(event != 0);	

	//terminates when event number is less than 100
	strcpy(msg.message, "terminate");
	msg.mtype = ref_num_a;
	msg.source = ref_num;
	msgsnd (qid, (struct msgbuf *)&msg, size, 0);	
	cout << getpid() << ": now exits" << endl;
	exit(0);
}

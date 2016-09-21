//
//  DataCenter.cpp
//  Operating Systems Projects
//
//  Created by Xavier Martinez on 9/18/16.
//  Copyright © 2016 Xavier Martinez. All rights reserved.
//

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <cstdlib>
#include <thread>
using namespace std;

// declare my message buffer
struct mBuf {
    long mtype; // required
    char message[102]; // message container
};

struct rBuf {
    long mtype; // required
    int choice; // message container
};

void clientHandler(int qid, int mtype){
    mBuf messages[20];
    mBuf temp;
    int messageSize = sizeof(temp)-sizeof(long);
    
    for (int i = 0; i < 20; i++) {
        msgrcv(qid, (struct msgbuf *)&temp, messageSize, mtype, 0);
        char tens = temp.message[0];
        char ones = temp.message[1];
        int index = (10 * (int)(tens - '0')) + (int)(ones - '0');
        messages[index] = temp;
    }
    
    rBuf request;
    int requestSize = sizeof(request)-sizeof(long);
    bool connection = true;
    while (connection) {
        msgrcv(qid, (struct msgbuf *)&request, requestSize, mtype, 0);
        if (request.choice != -1) {
            msgsnd(qid, (struct msgbuf *)&messages[request.choice], messageSize, 0);
        }
        else connection = false;
    }
}


int main(){
    // create my msgQ with key value from ftok()
    int qid = msgget(ftok(".",'u'), IPC_EXCL|IPC_CREAT|0600);
    
    thread client1(clientHandler, qid, 111);
//    thread client2(clientHandler, qid, 222);
//    thread client3(clientHandler, qid, 333);
    client1.join();
//    client2.join();
//    client3.join();
    
    msgctl (qid, IPC_RMID, NULL);
    
    return 0;
}

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
using namespace std;

struct mBuf {
    long mtype; // required
    char message[105]; // message container
};

struct client{
    char arr[20][101];
};

struct rBuf {
    long mtype; // required
    char message[101]; // message container
};


int main(){
    // create my msgQ with key value from ftok()
    int qid = msgget(ftok(".",'u'), IPC_EXCL|IPC_CREAT|0600);
    if (qid == -1){
        cout << "Message Queue already created. Please use \"ipcrm -q <qid>\" to remove conflicting queue (to find the qid use \"ipcs -q\")." << endl;
        cout << "Exiting..." << endl;
        exit(1);
    }
    
    client one;
//    client two;
//    client three;
    
    mBuf tmp;
    int messageSize = sizeof(tmp)-sizeof(long);
    
    rBuf snd;
    snd.mtype = 222;
    int sndSize = sizeof(snd)-sizeof(long);
    
    int counter = 0;
    while (counter < 1) {
        msgrcv(qid, (struct msgbuf *)&tmp, messageSize, 111, 0);
        char container[105];
        strcpy(container, tmp.message);
        cout << container << endl;
        if (container[0] == '0') {
            if (container[1] == '1') {
                char tens = container[2];
                char ones = container[3];
                int index = (10 * (int)(tens - '0')) + (int)(ones - '0');
                for (int i = 4; i < 104; i++) {
                    one.arr[index][i-4] = container[i];
                }
                one.arr[index][100] = '\0';
            }
        }
        else {
            if (container[1] == '1') {
                char tens = container[2];
                char ones = container[3];
                int index = (10 * (int)(tens - '0')) + (int)(ones - '0');
                if (index != 20) {
                    strcpy(snd.message, one.arr[index]);
                    msgsnd(qid, (struct msgbuf *)&snd, sndSize, 0);
                }
                else {
                    counter++;
                }
            }
        }
    }
    msgctl (qid, IPC_RMID, NULL);
    return 0;
}
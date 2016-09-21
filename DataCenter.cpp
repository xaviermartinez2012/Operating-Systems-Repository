// this is a test
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

struct mBuf {
    long mtype; // required
    char message[104]; // message container
};

struct client{
    char arr[20][100];
};

struct rBuf {
    long mtype; // required
    char message[100]; // message container
};


int main(){
    // create my msgQ with key value from ftok()
    int qid = msgget(ftok(".",'u'), IPC_EXCL|IPC_CREAT|0600);
    
    client one;
    client two;
    client three;
    
    mBuf tmp;
    tmp.mtype = 111;
    int messageSize = sizeof(tmp)-sizeof(long);
    
    rBuf snd;
    snd.mtype = 222;
    int sndSize = sizeof(snd)-sizeof(long);
    
    int counter = 0;
    while (counter < 3) {
        msgrcv(qid, (struct msgbuf *)&tmp, messageSize, 111, 0);
        if (tmp.message[0] == '0') {
            if (tmp.message[1] == '1') {
                char tens = tmp.message[2];
                char ones = tmp.message[3];
                int index = (10 * (int)(tens - '0')) + (int)(ones - '0');
                for (int i = 4; i < 104; i++) {
                    one.arr[index][i] = tmp.message[i];
                }
            }
        }
        else {
            if (tmp.message[1] == '1') {
                char tens = tmp.message[2];
                char ones = tmp.message[3];
                int index = (10 * (int)(tens - '0')) + (int)(ones - '0');
                if (index != 20) {
                    for (int i = 0; i < 100; i++) {
                        snd.message[i] = one.arr[index][i];
                    }
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

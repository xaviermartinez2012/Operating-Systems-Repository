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
    char arr[20][100];
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
    
    int counter = 0;
    while (counter < 1) {
        mBuf tmp;
        int messageSize = sizeof(tmp)-sizeof(long);
        msgrcv(qid, (struct msgbuf *)&tmp, messageSize, 111, 0);
        cout << "R: " << tmp.message[0] << " C: " << tmp.message[1] << " S.ID: " << tmp.message[2] << tmp.message[3] << endl;
        if (tmp.message[0] == '0') {
            if (tmp.message[1] == '1') {
                char tens = tmp.message[2];
                char ones = tmp.message[3];
                int index = (10 * ((int)(tens - '0'))) + ((int)(ones - '0'));
                for (int i = 4; i < 104; i++) {
                    one.arr[index][i-4] = tmp.message[i];
                }
            }
        }
        else if (tmp.message[0] == '1') {
            if (tmp.message[1] == '1') {
                char tens = tmp.message[2];
                char ones = tmp.message[3];
                int index = (10 * (int)(tens - '0')) + (int)(ones - '0');
                if (index != 20) {
                    rBuf snd;
                    snd.mtype = 222;
                    int sndSize = sizeof(snd)-sizeof(long);
                    strcpy(snd.message, one.arr[index]);
                    msgsnd(qid, (struct msgbuf *)&snd, sndSize, 0);
                }
                else {
                    counter++;
                }
            }
        }
        else {
            cout << "An error has occured. \"R\" field of message header is missing or incorrect" << endl;
            counter = 20;
        }
    }
    msgctl (qid, IPC_RMID, NULL);
    return 0;
}
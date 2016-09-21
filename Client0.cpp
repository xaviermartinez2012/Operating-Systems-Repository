//
//  Client0.cpp
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

void populate(char *arr, int index){
    int ones = index % 10;
    int tens = index / 10;
    for (int i = 0; i < 102; i++) {
        if (i == 0) {
            arr[i] = (char)('0' + tens);
            continue;
        }
        if (i == 1) {
            arr[i] = (char)('0' + ones);
            continue;
        }
        srand(time(NULL));
        arr[i] = rand()%(90-65 + 1) + 65;
    }
}

struct mBuf {
    long mtype; // required
    char message[102]; // message container
};


struct rBuf {
    long mtype; // required
    int choice; // message container
};

int main(){
    int qid = msgget(ftok(".",'u'), 0);
    mBuf message;
    message.mtype = 111;
    int messageSize = sizeof(message)-sizeof(long);
    
    for (int i = 0; i < 20; i ++) {
        populate(message.message, i);
        msgsnd(qid, (struct msgbuf *)&message, messageSize, 0);
    }
    
    rBuf request;
    request.mtype = 111;
    int requestSize = sizeof(request)-sizeof(long);
    
    int userChoice = -1;
    bool end = false;
    while (!end) {
        cout << "Enter index: ";
        cin >> userChoice;
        request.choice = userChoice;
        msgsnd(qid, (struct msgbuf *)&request, requestSize, 0);
        if (userChoice != -1) {
            msgrcv(qid, (struct msgbuf *)&message, messageSize, 111, 0);
            for (int i = 2; i < 102; i++) {
                cout << message.message[i];
            }
            cout << endl;
        }
        else {
            end = true;
        }
    }
    return 0;
}

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
#include <string>

using namespace std;

void populate(char *arr, char client, int index){
    int ones = index % 10;
    int tens = index / 10;
    srand(time(NULL));
    for (int i = 0; i < 104; i++) {
        if (i == 0) {
            arr[i] = '0';
            continue;
        }
        if (i == 1) {
            arr[i] = client;
            continue;
        }
        if (i == 2) {
            arr[i] = (char)('0' + tens);
            continue;
        }
        if (i == 3) {
            arr[i] = (char)('0' + ones);
            continue;
        }
        arr[i] = rand()%(90-65 + 1) + 65;
    }
}

struct mBuf {
    long mtype; // required
    char message[105]; // message container
};

struct rBuf {
    long mtype; // required
    char message[101]; // message container
};

int main(){
    int qid = msgget(ftok(".",'u'), 0);
    
    string requestHeader = "11";
    char client = '1';
    
    for (int i = 0; i < 20; i ++) {
        srand(time(NULL));
        mBuf message;
        message.mtype = 111;
        int messageSize = sizeof(message)-sizeof(long);
        string messageHeader = "01";
        int ones = i % 10;
        int tens = i / 10;
        messageHeader = messageHeader + (char)('0' + tens);
        messageHeader = messageHeader + (char)('0' + ones);
        for (int j = 4; j < 104; j++) {
            messageHeader = messageHeader + (char)(rand()%(90-65 + 1) + 65);
        }
        char msg[105];
        size_t length = messageHeader.copy(msg, 104);
        msg[length] = '\0';
        cout << "Message is: " << msg << endl;
        strcpy(message.message, msg);
        int code = msgsnd(qid, (struct msgbuf *)&message, messageSize, 0);
        cout << code << endl;
    }
    
    rBuf request;
    int requestSize = sizeof(request)-sizeof(long);
    
    int userChoice = 0;
    while (userChoice >= 0) {
        mBuf message;
        message.mtype = 111;
        int messageSize = sizeof(message)-sizeof(long);
        message.message[0] = '1';
        message.message[1] = client;
        cout << "Enter index: ";
        cin >> userChoice;
        if (userChoice != -1) {
            message.message[2] = '2';
            message.message[3] = '0';
            msgsnd(qid, (struct msgbuf *)&message, messageSize, 0);
        }
        else {
            int ones = userChoice % 10;
            int tens = userChoice / 10;
            message.message[2] = (char)('0' + tens);
            message.message[3] = (char)('0' + ones);
            msgsnd(qid, (struct msgbuf *)&message, messageSize, 0);
            msgrcv(qid, (struct msgbuf *)&request, requestSize, 222, 0);
            for (int i = 0; i < 100; i++){
                cout << request.message[i];
            }
            cout << endl;
        }
    }
    return 0;
}
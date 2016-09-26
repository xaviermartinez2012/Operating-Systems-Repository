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
#include <ctime>

using namespace std;

struct mBuf {
    long mtype; // required
    char message[105]; // message container
};

struct rBuf {
    long mtype; // required
    char message[101]; // message container
};

int main(){
    srand(time(0));
    int qid = msgget(ftok(".",'u'), 0);
    string requestHeader = "11";
    char client = '1';
    
    for (int i = 0; i < 20; i ++) {
        mBuf message;
        message.mtype = 111;
        int messageSize = sizeof(message)-sizeof(long);
        string messageHeader = "0";
        messageHeader += client;
        int ones = i % 10;
        int tens = i / 10;
        messageHeader += (char)('0' + tens);
        messageHeader += (char)('0' + ones);
        for (int j = 4; j < 104; j++) {
            messageHeader += (char)(rand()%(90-65 + 1) + 65);
        }
        char msg[104];
        messageHeader.copy(msg, 104);
        strcpy(message.message, msg);
        int code = msgsnd(qid, (struct msgbuf *)&message, messageSize, 0);
        cout << code << endl;
    }
    
    int userChoice = 0;
    while (userChoice >= 0) {
        mBuf rMessage;
        rMessage.mtype = 111;
        int messageSize = sizeof(rMessage)-sizeof(long);
        string messageHeader = "1";
        messageHeader += client;
        cout << "Enter index: ";
        cin >> userChoice;
        if (userChoice != -1) {
            int ones = userChoice % 10;
            int tens = userChoice / 10;
            messageHeader += (char)('0' + tens);
            messageHeader += (char)('0' + ones);
            cout << "Message header: " << messageHeader << endl;
            for (int j = 4; j < 104; j++) {
                messageHeader += (char)'X';
            }
            char msg[104];
            messageHeader.copy(msg, 104);
            strcpy(rMessage.message, msg);
            msgsnd(qid, (struct msgbuf *)&rMessage, messageSize, 0);
            rBuf request;
            int requestSize = sizeof(request)-sizeof(long);
            int code = msgrcv(qid, (struct msgbuf *)&request, requestSize, 222, 0);
            if (code == -1){
                cout << "There has been an error with the data server. Exiting..." << endl;
                break;
            }
            char req[101];
            strcpy(req, request.message);
            for (int i = 0; i < 101; i ++){
                cout << req[i];
            }
            cout << endl;
        }
        else {
            messageHeader += (char)'2';
            messageHeader += (char)'0';
            for (int j = 4; j < 104; j++) {
                messageHeader += (char)'X';
            }
            char msg[104];
            messageHeader.copy(msg, 104);
            msgsnd(qid, (struct msgbuf *)&rMessage, messageSize, 0);
        }
    }
    return 0;
}
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

int main(){
    // create my msgQ with key value from ftok()
    int qid = msgget(ftok(".",'u'), IPC_EXCL|IPC_CREAT|0666);
    
    // declare my message buffer
    struct buf {
        long mtype; // required
        char arr[20][101]; // message container
    };
    
    buf clients[3];
    bool clientsRecieved[3] = {false, false, false};
    int size = sizeof(buf)-sizeof(long);
    
    while (!clientsRecieved[0] or !clientsRecieved[1] or !clientsRecieved[2]){
        try {
            if (!clientsRecieved[0]) {
                int ret = msgrcv(qid, (struct msgbuf *)&clients[0], size, 1, IPC_NOWAIT);
                if (ret < 0) {
                    throw system_error(ENOMSG, system_category());
                }
                else clientsRecieved[0] = true;
            }
        } catch (system_error& err) {
            cout << ".. Waiting for Client1 ... " << endl;
        }
        try {
            if (!clientsRecieved[1]) {
                int ret = msgrcv(qid, (struct msgbuf *)&clients[1], size, 2, IPC_NOWAIT);
                if (ret < 0) {
                    throw system_error(ENOMSG, system_category());
                }
                else clientsRecieved[1] = true;
            }
        } catch (system_error& err) {
            cout << ".. Waiting for Client2 ... " << endl;
        }
        try {
            if (!clientsRecieved[2]) {
                int ret = msgrcv(qid, (struct msgbuf *)&clients[2], size, 3, IPC_NOWAIT);
                if (ret < 0) {
                    throw system_error(ENOMSG, system_category());
                }
                else clientsRecieved[2] = true;
            }
        } catch (system_error& err) {
            cout << ".. Waiting for Client3 ... " << endl;
        }
    }
    cout << "...finished." << endl;

    
    return 0;
}

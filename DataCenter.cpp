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
    int qid = msgget(ftok(".",'u'), IPC_EXCL|IPC_CREAT|0600);
    
    // declare my message buffer
    struct buf {
        long mtype; // required
        char arr[20][101]; // message container
    };
    
    buf clients[3] = {false, false, false};
    bool clientsRecieved[3];
    int size = sizeof(buf)-sizeof(long);
    
    bool allRecieved = false;
    while (!allRecieved){
        try {
            if (!clientsRecieved[0]) {
                msgrcv(qid, (struct msgbuf *)&clients[0], size, 1, IPC_NOWAIT);
            }
            if (!clientsRecieved[1]) {
                msgrcv(qid, (struct msgbuf *)&clients[1], size, 2, IPC_NOWAIT);
            }
            if (!clientsRecieved[2]) {
                msgrcv(qid, (struct msgbuf *)&clients[2], size, 3, IPC_NOWAIT);
            }
            allRecieved = true;
            cout << "...finished." << endl;
        } catch (system_error err) {
            cout << "..." << endl;
        }
    }
    
    
    return 0;
}

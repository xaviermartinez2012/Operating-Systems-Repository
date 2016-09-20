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

void populate(char arr[][101], int size){
    for (int i=0; i < size; i++) {
        for (int j=0; j < 100; j++) {
            srand(time(NULL));
            arr[i][j] = rand()%(90-65 + 1) + 65;
        }
    }
}

int main(){
    int qid = msgget(ftok(".",'u'), 0);
    
    // declare my message buffer
    struct buf {
        long mtype; // required
        char arr[20][101]; // message container
    };
    buf msg0;
    msg0.mtype = 1;
    populate(msg0.arr, 20);
    
    buf msg1;
    msg1.mtype = 2;
    populate(msg1.arr, 20);
    
    buf msg2;
    msg2.mtype = 3;
    populate(msg2.arr, 20);
    
    int size = sizeof(buf)-sizeof(long);
    
    msgsnd(qid, (struct msgbuf *)&msg0, size, 0);
    msgsnd(qid, (struct msgbuf *)&msg1, size, 0);
    msgsnd(qid, (struct msgbuf *)&msg2, size, 0);
    
    return 0;
}

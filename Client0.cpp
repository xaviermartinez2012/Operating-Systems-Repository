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

void randString(char arr[]){
    char cstring[101];
    for(int i = 0; i < 100; i++){
        srand(time(NULL));
        cstring[i] = rand()%(90-65 + 1) + 65;
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
    
    buf msg1;
    buf msg2;
    int size = sizeof(buf)-sizeof(long);
    
    return 0;
}
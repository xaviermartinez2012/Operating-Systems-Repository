//
//  rogue.cpp
//  Operating Systems Projects
//
//  Created by Xavier Martinez on 9/12/16.
//  Copyright © 2016 Xavier Martinez. All rights reserved.
//

#include <stdio.h>
#include <cstdlib>
#include <unistd.h>
#include <time.h>
int main () {
    struct timespec delay, left;
    delay.tv_sec = 0;
    delay.tv_nsec = 1000000;
    while (true) {
        nanosleep(&delay,&left); // students should avoid this function in this class
    }
    return 0;
}
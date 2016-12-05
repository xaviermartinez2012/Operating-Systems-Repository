//
//  semaphores.cpp
//  Operating Systems Projects
//
//  Created by Xavier Martinez on 11/15/16.
//  Copyright © 2016 Xavier Martinez. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "semaphore.h"
using namespace std;

enum {SAVINGS, CHECKING, MONEY_MARKET, IRA, CARD, AUTO, LOC, MORTGAGE};

void parent_cleanup(SEMAPHORE &);
void husband_process(SEMAPHORE &, double *, int);
void wife_process(SEMAPHORE &, double *);
void account_status(double *);

void parentCleanup(SEMAPHORE &sem, int shmid) {
    shmctl(shmid, IPC_RMID, NULL);
    sem.remove();
}

int main(){
    srand(time(NULL));
    cout.precision(2);
    
    int shm_id;
    shm_id = shmget(IPC_PRIVATE, 8 * sizeof(double), PERMS);
    
    double * accounts = (double *) shmat(shm_id, NULL, 0);
    accounts[0] = 500000;
    accounts[1] = 10000;
    accounts[2] = 5000;
    accounts[3] = 1000000;
    accounts[4] = 10000;
    accounts[5] = 30000;
    accounts[6] = 5000;
    accounts[7] = 300000;
    
    SEMAPHORE sem(8);
    sem.V(SAVINGS);
    sem.V(CHECKING);
    sem.V(MONEY_MARKET);
    sem.V(IRA);
    sem.V(CARD);
    sem.V(AUTO);
    sem.V(LOC);
    sem.V(MORTGAGE);
    
    int status;
    
    for (int i = 0; i < 2; i++){
        pid_t process = fork();
        if (process == 0) {
            husband_process(sem, accounts, i+1);
            exit(0);
        }
    }
    for (int i = 0; i < 2; i++){
        wait(&status);
    }
    parentCleanup(sem, shm_id);
    
    exit(0);
}


void husband_process(SEMAPHORE & sem, double * accounts, int process_identifier){
    int randnum = rand();
    int transactions = 0;
    while (transactions < 100) {
        if (randnum % 37003) {
            int randnum2 = (rand() % 3) + 1;
            switch (randnum2) {
                
                case 1:
                    sem.P(SAVINGS);
                    cout << "Process ID = " << process_identifier << ". Operation 1: Locked Savings." << endl;
                    *(accounts + SAVINGS) += (accounts[SAVINGS] / 100.00);
                    cout << fixed << "Savings: $" << accounts[SAVINGS] << endl;
                    cout << "Process ID = " << process_identifier << ". Unlocking Savings." << endl;
                    sem.V(SAVINGS);
                    break;
                case 2:
                    sem.P(IRA);
                    cout << "Process ID = " << process_identifier << ". Operation 2: Locked IRA." << endl;
                    *(accounts + IRA) += (accounts[IRA] * (3.00/100.00));
                    cout << fixed << "IRA: $" << accounts[IRA] << endl;
                    cout << "Process ID = " << process_identifier << ". Unlocking IRA." << endl;
                    sem.V(IRA);
                    break;
                case 3:
                    sem.P(CHECKING);
                    cout << "Process ID = " << process_identifier << ". Operation 3: Locked Checking." << endl;
                    *(accounts + CHECKING) += 1500.00;
                    cout << fixed << "Checking: $" << accounts[CHECKING] << endl;
                    cout << "Process ID = " << process_identifier << ". Unlocking Checking." << endl;
                    sem.V(CHECKING);
                    break;

                default:
                    break;
            }
            transactions++;
        }
        randnum = rand();
    }
}

void account_status(double * accounts){
    cout << fixed << "Savings: $" << accounts[SAVINGS] << endl;
    cout << fixed <<"Checking: $" << accounts[CHECKING] << endl;
    cout << fixed << "Money Market: $" << accounts[MONEY_MARKET] <<endl;
    cout << fixed << "IRA: $" << accounts[IRA] <<endl;
    cout << fixed << "Credit Card: $" << accounts[CARD] <<endl;
    cout << fixed << "Auto Loan: $" << accounts[AUTO] <<endl;
    cout << fixed << "Line of Credit: $" << accounts[LOC] << endl;
    cout << fixed << "Mortgage: $" << accounts[MORTGAGE] <<endl;
}

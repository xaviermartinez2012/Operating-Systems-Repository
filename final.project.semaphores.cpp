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

int main(){
    srand(time(NULL));
    
    double accounts[] = {500000, 10000, 5000, 1000000, 10000, 30000, 5000, 300000};
    
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
    parent_cleanup(sem);
    
    exit(0);
}

void parent_cleanup (SEMAPHORE &sem) {
    wait(0);	/* wait for child to exit */
    sem.remove();
}

void husband_process(SEMAPHORE & sem, double * accounts, int process_identifier){
    int randnum = rand();
    int transactions = 0;
    while (transactions < 10) {
        if (randnum % 37003 == 0) {
            int randnum2 = (rand() % 3) + 1;
            switch (randnum2) {
                
                case 1:
                    sem.P(SAVINGS);
                    cout << "Process ID = " << process_identifier << ". Operation 1: Locked accounts." << endl;
                    account_status(accounts);
                    accounts[SAVINGS] += (accounts[SAVINGS] / 100.00);
                    account_status(accounts);
                    cout << "Process ID = " << process_identifier << ". Unlocking accounts." << endl;
                    sem.V(SAVINGS);
                    break;
                case 2:
                    sem.P(IRA);
                    cout << "Process ID = " << process_identifier << ". Operation 2: Locked accounts." << endl;
                    account_status(accounts);
                    accounts[IRA] += (accounts[IRA] * (3.00/100.00));
                    account_status(accounts);
                    cout << "Process ID = " << process_identifier << ". Unlocking accounts." << endl;
                    sem.V(IRA);
                    break;
                case 3:
                    sem.P(CHECKING);
                    cout << "Process ID = " << process_identifier << ". Operation 3: Locked accounts." << endl;
                    account_status(accounts);
                    accounts[CHECKING] += 1500.00;
                    account_status(accounts);
                    cout << "Process ID = " << process_identifier << ". Unlocking accounts." << endl;
                    sem.V(CHECKING);
                    break;

                default:
                    break;
            }
            transactions++;
        }
    }
}

void account_status(double * accounts){
    cout << "Savings: $" << accounts[SAVINGS] << endl;
    cout << "Checking: $" << accounts[CHECKING] << endl;
    cout << "Money Market: $" << accounts[MONEY_MARKET] <<endl;
    cout << "IRA: $" << accounts[IRA] <<endl;
    cout << "Credit Card: $" << accounts[CARD] <<endl;
    cout << "Auto Loan: $" << accounts[AUTO] <<endl;
    cout << "Line of Credit: $" << accounts[LOC] << endl;
    cout << "Mortgage: $" << accounts[MORTGAGE] <<endl;
}
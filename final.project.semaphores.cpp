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

enum {SAVINGS, CHECKING, MONEY_MARKET, IRA, CARD, AUTO, LOC, MORTGAGE, PRINT};

void child_process(SEMAPHORE &, double *, int);
void account_status(double *);
void account_status(SEMAPHORE &, double *, int);
void parentCleanup(SEMAPHORE &, int);

int main(){
    srand(time(NULL));
    cout.precision(2);
    
    // Establish shared memory
    int shm_id;
    shm_id = shmget(IPC_PRIVATE, 8 * sizeof(double), PERMS);
    
    // Establish accounts
    double * accounts = (double *) shmat(shm_id, NULL, 0);
    accounts[0] = 500000;
    accounts[1] = 100000;
    accounts[2] = 5000;
    accounts[3] = 1000000;
    accounts[4] = 10000;
    accounts[5] = 30000;
    accounts[6] = 5000;
    accounts[7] = 300000;
    
    // Initialize semaphores
    SEMAPHORE sem(9);
    sem.V(SAVINGS);
    sem.V(CHECKING);
    sem.V(MONEY_MARKET);
    sem.V(IRA);
    sem.V(CARD);
    sem.V(AUTO);
    sem.V(LOC);
    sem.V(MORTGAGE);
    sem.V(PRINT);

    account_status(accounts);
    
    int status;
    
    // Create child processes by forking
    for (int i = 0; i < 5; i++){
        pid_t process = fork();
        if (process == 0) {
            child_process(sem, accounts, i+1);
            exit(0);
        }
    }
    
    // Wait for all child processes to finish
    for (int i = 0; i < 5; i ++) {
        wait(NULL);
    }
    
    parentCleanup(sem, shm_id);
    
    exit(0);
}

// Child process operations
void child_process(SEMAPHORE & sem, double * accounts, int process_identifier){
    int randnum = rand();
    bool printed = false;
    int transactions = 0;
    while (transactions < 500) {
        if (randnum % 37003 == 0) {
            int randnum2 = (rand() % 3) + 1;
            switch (randnum2) {
                // Operation 1 - deposit 1% of fund or interest in savings
                case 1:
                    sem.P(SAVINGS);
                    *(accounts + SAVINGS) += (accounts[SAVINGS] / 100.00);
                    sem.V(SAVINGS);
                    break;
                // Operation 2 - separately deposit 3% of fund or interest in IRA
                case 2:
                    sem.P(IRA);
                    *(accounts + IRA) += (accounts[IRA] * (3.00/100.00));
                    sem.V(IRA);
                    break;
                // Operation 3 - separately deposit (or wired) of $1500 into checking
                case 3:
                    sem.P(CHECKING);
                    *(accounts + CHECKING) += 1500.00;
                    sem.V(CHECKING);
                    break;
            }
            transactions++;
            printed = false;
        }
        // Ooperation 4 - transfer $500 from equity line of credit into checking
        else if (randnum % 100003 == 0){
            sem.P(LOC);
            if ((*(accounts + LOC) -= 500) < 0){
                *(accounts + LOC) = 0;
            };
            sem.V(LOC);
            sem.P(CHECKING);
            *(accounts + IRA) += 500;
            sem.V(CHECKING);
            transactions++;
            printed = false;
        }
        else if(randnum % 6301 == 0){
            int randnum2 = (rand() % 2) + 1;
            // Operation 5 - withdraw $100 from checking or clear $100 check
            switch (randnum2) {
                case 1:
                    sem.P(CHECKING);
                    if ((*(accounts + CHECKING) -= 100) < 0){
                        *(accounts + CHECKING) = 0;
                    }
                    sem.V(CHECKING);
                    break;
                case 2:
                    // Clear check
                    if (process_identifier > 2) {
                        cout << "Process ID = " << process_identifier << ". Clearing $100 check." << endl;
                    }
                    break;
            }
            transactions++;
            printed = false;
        }
        // Operation 6 - deposit a small amount to IRA from a random account
        else if(randnum % 126247697 == 0){
            int randnum2 = (rand() % 3) + 1;
            switch(randnum2){
                case 1:
                    sem.P(IRA);
                    *(accounts + IRA) += 50;
                    sem.V(IRA);
                    sem.P(SAVINGS);
                    if ((*(accounts + SAVINGS) -= 50) < 0){
                        *(accounts + SAVINGS) = 0;
                    }
                    sem.V(SAVINGS);
                    break;
                case 2:
                    sem.P(CHECKING);
                    if ((*(accounts + CHECKING) -= 50) < 0){
                        *(accounts + CHECKING) = 0;
                    }
                    sem.V(CHECKING);
                    sem.P(IRA);
                    *(accounts + IRA) += 50;
                    sem.V(IRA);
                    break;
                case 3:
                    sem.P(IRA);
//                    sem.P(MONEY_MARKET);
                    if ((*(accounts + MONEY_MARKET) -= 50) < 0){
                        *(accounts + MONEY_MARKET) = 0;
                    }
                    *(accounts + IRA) += 50;
                    sem.V(IRA);
//                    sem.V(MONEY_MARKET);
                    break;
            }
            transactions++;
            printed = false;
        }
        else if(randnum % 7001 == 0){
            int randnum2 = (rand() % 2) + 1;
            switch (randnum2){
                // Loan Operation 1 - transfer $200 from checking to auto loan
                case 1:
                    sem.P(CHECKING);
//                    sem.P(AUTO);
                    if ((*(accounts + CHECKING) -= 200) < 0){
                        *(accounts + CHECKING) = 0;
                    }
                    *(accounts + AUTO) += 200;
                    sem.V(CHECKING);
//                    sem.V(AUTO);
                    break;
                // Loan Operation 2 - transfer $150 from checking to equity line of credit
                case 2:
                    sem.P(CHECKING);
                    if ((*(accounts + CHECKING) -= 150) < 0){
                        *(accounts + CHECKING) = 0;
                    }
                    sem.V(CHECKING);
                    sem.P(LOC);
                    *(accounts + LOC) += 150;
                    sem.V(LOC);
                    break;
            }
            transactions++;
            printed = false;
        }
        // Loan Operation 3 - transfer $300 from checking to home mortgage
        else if (randnum % 3001 == 0){
            sem.P(CHECKING);
//            sem.P(MORTGAGE);
            if ((*(accounts + CHECKING) -= 300) < 0){
                *(accounts + CHECKING) = 0;
            }
            *(accounts + MORTGAGE) += 300;
            sem.V(CHECKING);
//            sem.V(MORTGAGE);
            transactions++;
            printed = false;
        }
        // Print the status of the accounts every 100 transactions
        if ((transactions % 100) == 0 and !printed){
            account_status(sem, accounts, process_identifier);
            printed = true;
        }
        randnum = rand();
    }
//    cout << "Process " << process_identifier << " finished!" << endl;
}

// Deallocate shared memory and semaphores
void parentCleanup(SEMAPHORE &sem, int shmid) {
    shmctl(shmid, IPC_RMID, NULL);
    sem.remove();
}

// Print the status of the accounts with locking capability
void account_status(SEMAPHORE & sem, double * accounts, int pid){
    sem.P(PRINT);
    cout << endl << "####################" << endl << "Account status printed by process:  " << pid << "." << endl;
    cout << fixed << "Savings: $" << accounts[SAVINGS] << endl;
    cout << fixed <<"Checking: $" << accounts[CHECKING] << endl;
    cout << fixed << "Money Market: $" << accounts[MONEY_MARKET] <<endl;
    cout << fixed << "IRA: $" << accounts[IRA] <<endl;
    cout << fixed << "Credit Card: $" << accounts[CARD] <<endl;
    cout << fixed << "Auto Loan: $" << accounts[AUTO] <<endl;
    cout << fixed << "Line of Credit: $" << accounts[LOC] << endl;
    cout << fixed << "Mortgage: $" << accounts[MORTGAGE] <<endl;
    cout << "####################" << endl;
    sem.V(PRINT);
}

// Print the status of the accounts
void account_status(double * accounts){
    cout << fixed << endl << "Savings: $" << accounts[SAVINGS] << endl;
    cout << fixed <<"Checking: $" << accounts[CHECKING] << endl;
    cout << fixed << "Money Market: $" << accounts[MONEY_MARKET] <<endl;
    cout << fixed << "IRA: $" << accounts[IRA] <<endl;
    cout << fixed << "Credit Card: $" << accounts[CARD] <<endl;
    cout << fixed << "Auto Loan: $" << accounts[AUTO] <<endl;
    cout << fixed << "Line of Credit: $" << accounts[LOC] << endl;
    cout << fixed << "Mortgage: $" << accounts[MORTGAGE] <<endl;
}

//
//  forking.cpp
//  Operating Systems Projects
//
//  Created by Xavier Martinez on 10/22/16.
//  Copyright © 2016 Xavier Martinez. All rights reserved.
//

#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
using namespace std;

// Function that allocates random chars to a char [] reference.
void allocate(char arr[], int size){
    for(int j = 0; j < size; j++){
        char randC = rand()%(90-65 + 1) + 65;
        arr[j] = randC;
    }
}

// Function to count the number of occurences of "userInput" within arr
int count(char arr[], char userInput, int size){
    int total = 0;
    for(int i = 0; i < size; i++){
        if (arr[i] == userInput) {
            total++;
        }
    }
    return total;
}

int main(){
    // Generate random seed
    srand(time(NULL));
    
    // Begin user interface
    bool exitProgram = false;
    while (!exitProgram) {
        // Prompt user for choice (start program or exit)
        int choice = 2;
        cout << "(1) Run program" << endl;
        cout << "(2) Exit" << endl;
        cin >> choice;
        
        // Exit if choice is 2
        if (choice == 2) {
            exitProgram = true;
        }
        
        // Begin program
        else if (choice == 1){
            // Prompt user for the size of the array allocation
            int size = 1;
            cout << "Enter the size of the new array: ";
            cin >> size;
            
            // Allocate the array and fill it with random uppercase letters
            char * arr = new char[size];
            allocate(arr, size);
            
            // Begin spawning a child process
            pid_t childPid;
            childPid = fork();
            if(childPid == 0)  // Fork succeeded
            {
                // Prompt user for an uppercase letter to search within the aray
                char userInput;
                cout << "Enter an uppercase character: ";
                cin >> userInput;
                
                // Count occurences of the letter in the array
                int occurences;
                occurences = count(arr, userInput, size);
                cout << userInput << " appears in the array " << occurences << " times." << endl;
                
                // Terminate the child successfully
                exit(EXIT_SUCCESS);
            }
            
            else if(childPid < 0)  // Fork failed
            {    
                cout << "ERROR: Child process not created." << endl;
            }
            // End spawing a child process
            
            // Make parent process wait for child termination
            else {
                int returnStatus;
                waitpid(childPid, &returnStatus, 0);  // Parent process waits here for child to terminate.
                
                // Deallocate the array
                delete []arr;
            }
        }
        // End program
    }
    // End user interface
    
    exit(EXIT_SUCCESS);
}

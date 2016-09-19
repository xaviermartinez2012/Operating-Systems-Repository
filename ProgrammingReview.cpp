//
//  helloworld.cpp
//  Operating Systems Projects
//
//  Created by Xavier Martinez on 8/22/16.
//  Copyright © 2016 Xavier Martinez. All rights reserved.

/*  Program creates a struct that will have an array of integers 
    and an array of char pointers, each of size 20. The integer 
    array will be initialized based off the recursive definition: 
    f(0)=2900 & f(n+1)=2*f(n), where n is an index of the integer 
    array for 0< n < 20. The pointers will point to the arrays of 
    chars of varying size based off the value of their index in 
    the integer array (Ex: struct.characterPointerArray[i] = new char[struct.integerArray[i]).
    Every array of chars will be populated with random uppercase letters. 
    The interactive aspect of the program will handle user’s 
    requests to view the contents of a particular pointer, 
    view the indexes deallocated pointers, deallocate all pointers, 
    and exit the program. When a user access a particular pointer, 
    the program must handle user’s requests to view the first then 
    chars in the char array, deallocate the pointer, 
    and exit back to the main menu.
 */

#include <iostream>
#include <cstdlib>
using namespace std;

// Single struct with an array of 20 pointers to 'char' and an integer array with 20 elements
struct single{
    int arrI[20];
    char **arrC;
};

// Function that prints the indices of a bool array that are True
void printDeallocated(bool deallocated[]){
    for (int i = 0; i < 20; i++) {
        if (deallocated[i]) {
            cout << i << endl;
        }
    }
}

// Function that prints the first 10 chars of a char array
void printChars(char arr[]){
    for (int i = 0; i < 10; i++) {
        cout << arr[i];
    }
    cout << endl;
}

// Function that allocates random chars to a char [] reference.
void allocate(char arr[], int size){
    srand(time(NULL));
    for(int j = 0; j < size; j++){
        char randC = rand()%(90-65 + 1) + 65;
        arr[j] = randC;
    }
}

int main() {
    // structure: A single object that will be used to store data for use by the program
    single structure;
    
    /* Initialization of the integer array (structure.arrI) based off
     the recursive definition: f(0)=2900 & f(n+1)=2*f(n),
     where n is an index of the integer array for 0< n < 20.
     */
    structure.arrI[0] = 2900;
    for (int i = 1; i < 20; i++) {
        structure.arrI[i] = 2 * structure.arrI[i - 1];
    }
    
    /* Allocating memory to all pointers to 'char' (structure.arrc)
       based on the size stored in the integer array, and initialize
       every char element to a random upper case letter.
     */
    structure.arrC = new char*[20];
    for (int i = 0; i < 20; i++) {
        structure.arrC[i] = new char[structure.arrI[i]];
        allocate(structure.arrC[i], structure.arrI[i]);
    }
    
    /* Initiallize an array of booleans (representing whether or not the
       pointer to char has been deallocated.
     */
    bool deallocated[20];
    for (int i = 0; i < 20; i++) {
        deallocated[i] = false;
    }
    
    /* Begin first level of menu items.
       Main menu:
       (1) Access a pointer, 
       (2) List deallocated memory (index),
       (3) Deallocate all memory, and (4) Exit program.
     */
    bool end = false;
    while (!end) {
        cout << "1. Access a pointer" << endl;
        cout << "2. List deallocated memory (index)" << endl;
        cout << "3. Deallocate all memory" << endl;
        cout << "4. Exit program" << endl;
        int choice;
        cin >> choice;
        // Begin case statements to see what the user has chosen (menu)
        
        // Exit program by deallocating all memory
        if (choice == 4) {
            for (int i = 0; i < 20; i++) {
                if (!deallocated[i]) {
                    delete [](structure.arrC[i]);
                }
            }
            delete []structure.arrC;
            end = true;
        }
        // Deallocate all char pointers if they have not been deallocated already
        else if (choice == 3) {
            for (int i = 0; i < 20; i++) {
                if (!deallocated[i]) {
                    delete [](structure.arrC[i]);
                    deallocated[i] = true;
                }
            }
        }
        /* Iterate through the array of booleans and print
           the index of every true
        */
        else if (choice == 2){
            cout << "Deallocated indeces:" << endl;
            printDeallocated(deallocated);
        }
        /* On access: prompt the user for the index of the array to manipulate.
           Then, present the user with these submenu items: 
           (1) Print the first 10 char's in the chosen array, 
           (2) Delete all the char's associated with this pointer, and 
           (3) Return to main menu. 
           When the user chooses to delete the char's, deallocate memory (physically).
         */
        else if (choice == 1){
            cout << "Select an index (0 - 19): ";
            int index;
            cin >> index;
            /* If the user tries to access a deallocated pointer,
               notify the user of the missing char's and 
               re-allocate memory & re-initialize the char's.
             */
            if (deallocated[index]) {
                cout << "Error: Missing chars" << endl;
                cout << "Reallocating memory and reinitializing chars..." << endl;
                structure.arrC[index] = new char[structure.arrI[index]];
                allocate(structure.arrC[index], structure.arrI[index]);
                deallocated[index] = false;
            }
            bool sub_end = false;
            while (!sub_end) {
                cout << "1. Print first 10 chars" << endl;
                cout << "2. Delete all chars" << endl;
                cout << "3. Return to main menu" << endl;
                int sub_choice;
                cin >> sub_choice;
                
                // Exit to main menu
                if (sub_choice == 3) {
                    sub_end = true;
                }
                // Delete all chars
                else if (sub_choice == 2){
                    if (!deallocated[index]) {
                        delete [](structure.arrC[index]);
                        deallocated[index] = true;
                    }
                }
                // Print first 10 chars
                else if (sub_choice == 1){
                    if (!deallocated[index]) {
                        printChars(structure.arrC[index]);
                    }
                    /* If the user tries to access a deallocated pointer 
                       (that has been deallocated within the sub menu),
                       notify the user of the missing char's and
                       re-allocate memory & re-initialize the char's.
                     */
                    else{
                        cout << "Error: Missing chars" << endl;
                        cout << "Reallocating memory and reinitializing chars..." << endl;
                        structure.arrC[index] = new char[structure.arrI[index]];
                        allocate(structure.arrC[index], structure.arrI[index]);
                        deallocated[index] = false;
                        // Print first 10 chars
                        printChars(structure.arrC[index]);
                    }
                }
            // End case statements (submenu)
            // End submenu
            }
        // End choice == 1 (user accesses pointer)
        }
    // End case statements (menu)
    // End menu
    }
    return 0;
// End main
}
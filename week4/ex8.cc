/*
 * Copyright 2025 Zifeng Wang
 * 2478280
 * zifenw@uw.edu
 * 01/27/25
 */
#include <iostream>   // For standard input and output
#include <cstdlib>    // For EXIT_SUCCESS and EXIT_FAILURE
#include <cinttypes>  // For fixed-width integer types like uint64_t

using namespace std;

int main(int argc, char** argv) {
    uint64_t input;
    cout << "Which positive integer would you like me to factorize? ";
    cin >> input;  // Read user input

    // Check if the input is valid (not a non-integer value)
    if (!cin) {
        cerr << "Input is not a positive integer" << endl;
        return EXIT_FAILURE;  // Exit with failure status
    }

    // Ensure input is not zero (factorization of 0 is undefined)
    if (input == 0) {
        cerr << "input cannot be 0" << endl;
        return EXIT_FAILURE;  // Exit with failure status
    }

    // Loop through all numbers from 1 to the input value
    for (uint64_t i = 1; i <= input; i++) {
        if (input % i == 0) {
            // Print the factor, adding a space
            // before the number (except for the first factor)
            cout << (i == 1 ? "" : " ") << i;
        }
    }
    cout << endl;  // Print a newline for better formatting
    return EXIT_SUCCESS;  // Exit with success status
}

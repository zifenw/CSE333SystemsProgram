/*
 * Copyright 2025 Zifeng Wang
 * 2478280
 * zifenw@uw.edu
 * 02/07/31
 */

#include <iostream>     // for std::{cout, endl, cin, err, istream}
#include <vector>       // for std::vector
#include <algorithm>    // for std::sort
#include <cstdlib>      // for EXIT_FAILURE

// using namespace std;
using std::cout;
using std::endl;
using std::sort;
using std::vector;
using std::istream;
using std::cin;
using std::cerr;

// Function template to read a value of type T from an input stream
template <class T>
T ReadValue(istream& in);

int main(int argc, char** argv) {
    vector<double> vec;  // Vector to store the input doubles
    const int kNumDoubles = 6;  // Constant for the number of doubles to read

    cout << "Enter " << kNumDoubles << " doubles:" << endl;

    // Read 6 doubles from the user
    for (int i = 0; i < kNumDoubles; i++) {
        // Read a double from the input stream
        double input = ReadValue<double>(cin);
        vec.push_back(input);  // Add the double to the vector
    }

    // Sort the vector in ascending order
    sort(vec.begin(), vec.end());

    // Print the sorted doubles
    cout << "Your sorted doubles are:" << endl;
    // Use range-based for loop for better readability
    for (const auto& value : vec) {
        cout << value << endl;
    }
}

template <class T>
T ReadValue(istream& in) {
    T input;
    // Attempt to read a value of type T from the input stream
    in >> input;
    // Check if the input operation failed
    if (!cin.good()) {
        cerr << "conversion errors and unexpected EOF conditions" << endl;
        exit(EXIT_FAILURE);  // Terminate the program with an error code
    }
    return input;  // Return the successfully read value
}

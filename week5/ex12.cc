/*
 * Copyright 2025 Zifeng Wang
 * 2478280
 * zifenw@uw.edu
 * 02/07/31
 */

#include <iostream>   // For standard I/O operations (cout, endl, cerr)
#include <fstream>    // For file I/O operations (ifstream)
#include <map>        // For std::map to store word counts
#include <string>     // For std::string to handle words
#include <cstdlib>    // For exit() and EXIT_* constants

// using namespace std;
using std::cout;
using std::endl;
using std::ifstream;
using std::map;
using std::cerr;
using std::string;

int main(int argc, char** argv) {
    // Check command-line arguments: ensure a filename is provided
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <filename>" << endl;
        exit(EXIT_FAILURE);
    }

    // Create an ifstream object to open the input file
    ifstream file(argv[1]);

    // Check if the file was opened successfully
    if (!file) {
        cerr << "Error: Could not open file " << argv[1] << endl;
        exit(EXIT_FAILURE);
    }

    // Map to store word counts.
    // Keys are words (strings), values are counts (ints)
    map<string, int> word_count;
    string word;  // Temporary variable to hold each read word

    // Read words from the file using >> operator:
    // - ifstream automatically splits input by whitespace
    // - The loop continues as long as extraction (file >> word) succeeds
    // - When EOF or error occurs, the stream evaluates to "false"
    while (file >> word) {
        ++word_count[word];  // Increment count for the extracted word
    }

    // Output results in sorted order (std::map is ordered by keys)
    // - pair.first is the word (key), pair.second is the count (value)
    for (const auto& pair : word_count) {
        cout << pair.first << " " << pair.second << endl;
    }
    exit(EXIT_SUCCESS);
}

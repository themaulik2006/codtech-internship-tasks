#include <iostream>
#include <fstream>
#include <thread>
#include <string>
using namespace std;

// Global strings to store compressed parts from two threads
string part1 = "", part2 = ""; 

// RLE Compression Function
// Compresses the input string using Run Length Encoding and appends the result to output string
void compress(string input, string &output) {
    int count;
    for (int i = 0; i < input.length(); i++) {
        count = 1;
        // Count consecutive repeating characters
        while (i < input.length() - 1 && input[i] == input[i + 1]) {
            count++;
            i++;
        }
        // Append count and character to output (e.g., 4A for 'AAAA')
        output += to_string(count) + input[i];
    }
}

// Decompression Function
// Reads the compressed file, decompresses the data, and writes to output file
void decompressFile(const string& inputFile, const string& outputFile) {
    ifstream inFile(inputFile);
    ofstream outFile(outputFile);
    char ch;
    string number = ""; // To accumulate the count digits

    // Check if files opened successfully
    if (!inFile || !outFile) {
        cout << "File error during decompression.\n";
        return;
    }

    // Read each character from compressed file
    while (inFile.get(ch)) {
        if (isdigit(ch)) {
            // If digit, accumulate in number string
            number += ch;
        } else {
            // When non-digit found, convert accumulated number to int
            int count = stoi(number);
            // Write the character 'count' times to output file
            for (int i = 0; i < count; i++) {
                outFile << ch;
            }
            number = ""; // Reset number string for next count
        }
    }

    inFile.close();
    outFile.close();
    cout << "Decompression complete!" << endl;
}

// Multithreaded Compression Function
// Reads the entire input file, splits it into two parts, compresses each in separate threads, then combines results
void compressFile(const string& inputFile, const string& outputFile) {
    ifstream inFile(inputFile);
    string data;

    if (!inFile) {
        cout << "Could not open " << inputFile << endl;
        return;
    }

    // Read entire file content into string 'data'
    char ch;
    while (inFile.get(ch)) {
        data += ch;
    }
    inFile.close();

    // Determine midpoint for splitting data safely
    int mid = data.length() / 2;
    // Adjust mid to avoid splitting in the middle of a sequence of same characters
    while (mid < data.length() && data[mid] == data[mid - 1]) {
        mid++;
    }

    // Split data into two halves
    string firstHalf = data.substr(0, mid);
    string secondHalf = data.substr(mid);

    // Start two threads to compress each half concurrently
    thread t1(compress, firstHalf, ref(part1));
    thread t2(compress, secondHalf, ref(part2));

    // Wait for both threads to finish
    t1.join();
    t2.join();

    // Open output file to write combined compressed data
    ofstream outFile(outputFile);
    if (!outFile) {
        cout << "Could not create " << outputFile << endl;
        return;
    }

    // Write compressed parts one after another
    outFile << part1 << part2;
    outFile.close();

    cout << "Compression complete using multithreading!" << endl;
}

int main() {
    int choice;
    string inputFile, outputFile;

    do {
        // Display menu options to user
        cout << "\n=== FILE COMPRESSION MENU ===\n";
        cout << "1. Compress File\n";
        cout << "2. Decompress File\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(); // Clear input buffer

        switch (choice) {
            case 1:
                // Get input and output filenames for compression
                cout << "Enter input filename (to compress): ";
                getline(cin, inputFile);
                cout << "Enter output filename (compressed file): ";
                getline(cin, outputFile);

                // Reset global compressed parts before compressing
                part1 = "";
                part2 = "";

                // Call compression function
                compressFile(inputFile, outputFile);
                break;

            case 2:
                // Get filenames for decompression
                cout << "Enter compressed filename: ";
                getline(cin, inputFile);
                cout << "Enter output filename (after decompression): ";
                getline(cin, outputFile);

                // Call decompression function
                decompressFile(inputFile, outputFile);
                break;

            case 3:
                // Exit message
                cout << "Exiting program. Goodbye!\n";
                break;

            default:
                // Invalid menu choice
                cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 3);

    return 0;
}

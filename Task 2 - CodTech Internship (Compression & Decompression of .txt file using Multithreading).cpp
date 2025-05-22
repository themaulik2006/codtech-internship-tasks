#include <iostream>
#include <fstream>
#include <thread>
#include <string>
using namespace std;

string part1 = "", part2 = ""; // For compressed halves

// RLE Compression Function
void compress(string input, string &output) {
    int count;
    for (int i = 0; i < input.length(); i++) {
        count = 1;
        while (i < input.length() - 1 && input[i] == input[i + 1]) {
            count++;
            i++;
        }
        output += to_string(count) + input[i];
    }
}

// Decompression Function
void decompressFile(const string& inputFile, const string& outputFile) {
    ifstream inFile(inputFile);
    ofstream outFile(outputFile);
    char ch;
    string number = "";

    if (!inFile || !outFile) {
        cout << "File error during decompression.\n";
        return;
    }

    while (inFile.get(ch)) {
        if (isdigit(ch)) {
            number += ch;
        } else {
            int count = stoi(number);
            for (int i = 0; i < count; i++) {
                outFile << ch;
            }
            number = "";
        }
    }

    inFile.close();
    outFile.close();
    cout << "Decompression complete!" << endl;
}

// Multithreaded Compression Function
void compressFile(const string& inputFile, const string& outputFile) {
    ifstream inFile(inputFile);
    string data;

    if (!inFile) {
        cout << "Could not open " << inputFile << endl;
        return;
    }

    // Read full file into a string
    char ch;
    while (inFile.get(ch)) {
        data += ch;
    }
    inFile.close();

    // Safe split
    int mid = data.length() / 2;
    while (mid < data.length() && data[mid] == data[mid - 1]) {
        mid++;
    }

    string firstHalf = data.substr(0, mid);
    string secondHalf = data.substr(mid);

    // Compress each part with separate thread
    thread t1(compress, firstHalf, ref(part1));
    thread t2(compress, secondHalf, ref(part2));

    t1.join();
    t2.join();

    // Combine and write
    ofstream outFile(outputFile);
    if (!outFile) {
        cout << "Could not create " << outputFile << endl;
        return;
    }

    outFile << part1 << part2;
    outFile.close();

    cout << "Compression complete using multithreading!" << endl;
}

int main() {
    int choice;
    string inputFile, outputFile;

    do {
        cout << "\n=== FILE COMPRESSION MENU ===\n";
        cout << "1. Compress File\n";
        cout << "2. Decompress File\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(); // clear buffer

        switch (choice) {
            case 1:
                cout << "Enter input filename (to compress): ";
                getline(cin, inputFile);
                cout << "Enter output filename (compressed file): ";
                getline(cin, outputFile);
                part1 = "";
                part2 = "";
                compressFile(inputFile, outputFile);
                break;
            case 2:
                cout << "Enter compressed filename: ";
                getline(cin, inputFile);
                cout << "Enter output filename (after decompression): ";
                getline(cin, outputFile);
                decompressFile(inputFile, outputFile);
                break;
            case 3:
                cout << "Exiting program. Goodbye!\n";
                break;
            default:
                cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 3);

    return 0;
}

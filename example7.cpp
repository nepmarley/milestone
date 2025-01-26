#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <iomanip>
#include <sstream>

using namespace std;

// Global variables
vector<string> table; // Stores table data in a flat vector
int numOfRows = 0, numOfCols = 0; // Tracks rows and columns for the table

// Function declarations
void tableIndex(const vector<string> &table, int Rows, int Cols);
void createTable();
void addRowsToTable();
void ReadFromFile();
void processSQLCommand(const string &command);

int main() {
    int choice;

    do {
        // Display menu options
        cout << "\nChoose an option from below:\n";
        cout << "1. Read from file\n";
        cout << "2. Create new table\n";
        cout << "3. Execute SQL-like command\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(); // Clear buffer

        switch (choice) {
            case 1:
                ReadFromFile();
                break;
            case 2:
                createTable();
                break;
            case 3: {
                string command;
                cout << "Enter an SQL-like command (e.g., CREATE DATABASE <filename>):\n";
                getline(cin, command);
                processSQLCommand(command);
                break;
            }
            default:
                cout << "Invalid choice. Exiting program.\n";
                break;
        }

        cout << "Do you want to continue? (1 - Yes, 2 - No): ";
        cin >> choice;
        cin.ignore(); // Clear buffer

    } while (choice == 1);

    return 0;
}

// Function to display the table
void tableIndex(const vector<string> &table, int Rows, int Cols) {
    if (table.empty()) {
        cout << "The table is empty.\n";
        return;
    }

    for (int i = 0; i < Rows; i++) {
        for (int j = 0; j < Cols; j++) {
            int index = i * Cols + j;
            if (index < table.size())
                cout << setw(15) << table[index] << " | ";
            else
                cout << setw(15) << ""; // Handle incomplete rows
        }
        cout << endl;
    }
}

// Function to create a new table
void createTable() {
    table.clear();
    numOfRows = 0;
    numOfCols = 0;

    cout << "Enter the number of columns: ";
    cin >> numOfCols;

    cout << "Enter column names (separated by commas): ";
    cin.ignore();
    string columnNames;
    getline(cin, columnNames);

    stringstream ss(columnNames);
    string columnName;

    // Add column names to the table
    while (getline(ss, columnName, ',')) {
        table.push_back(columnName);
    }

    if (table.size() != numOfCols) {
        cout << "Error: Number of column names does not match the number of columns.\n";
        table.clear();
        return;
    }

    numOfRows++;
    cout << "Table created successfully with " << numOfCols << " columns.\n";

    // Display the table structure
    cout << "\nTable structure:\n";
    tableIndex(table, 1, numOfCols);

    // Prompt user to add rows to the table
    addRowsToTable();

    // Save the table to a file
    string filename;
    cout << "Enter filename to save the table: ";
    cin >> filename;

    ofstream outputFile(filename);
    if (!outputFile.is_open()) {
        cout << "Error: Could not save the table to the file.\n";
        return;
    }

    // Write column names
    for (int i = 0; i < numOfCols; i++) {
        outputFile << table[i];
        if (i < numOfCols - 1) outputFile << ",";
    }
    outputFile << endl;

    // Write table rows
    for (int i = numOfCols; i < table.size(); i++) {
        outputFile << table[i];
        if ((i + 1) % numOfCols == 0)
            outputFile << "\n";
        else
            outputFile << ",";
    }

    outputFile.close();
    cout << "Table saved to " << filename << ".\n";
}

// Function to add rows to the table
void addRowsToTable() {
    cout << "Enter table rows (comma-separated values). Type 'done' to finish.\n";

    while (true) {
        cout << "Enter row " << numOfRows << ": ";
        string input;
        cin.ignore();
        getline(cin, input);

        if (input == "done") {
            break;
        }

        stringstream ss(input);
        string value;
        vector<string> rowData;
        int colCount = 0;

        // Parse the input into individual column values
        while (getline(ss, value, ',')) {
            rowData.push_back(value);
            colCount++;
        }

        // Validate that the row matches the column count
        if (colCount != numOfCols) {
            cout << "Error: Row must contain exactly " << numOfCols << " values. Try again.\n";
            continue;
        }

        // Add the row to the table
        for (const auto &item : rowData) {
            table.push_back(item);
        }

        numOfRows++;
    }

    // Display the updated table
    cout << "\nUpdated Table:\n";
    tableIndex(table, numOfRows, numOfCols);
}

// Function to read the table from a file
void ReadFromFile() {
    ifstream inputFile;
    string filename, line;
    vector<string> localTable;
    int localRows = 0, localCols = 0;

    cout << "Enter the filename: ";
    cin >> filename;

    inputFile.open(filename);

    if (inputFile) {
        while (getline(inputFile, line)) {
            stringstream ss(line);
            string value;
            while (getline(ss, value, ',')) {
                localTable.push_back(value);
            }

            if (localCols == 0) {
                localCols = localTable.size();
            }
            localRows++;
        }

        cout << "Table read successfully:\n";
        tableIndex(localTable, localRows, localCols);
    } else {
        cout << "Error opening file.\n";
    }
}

// Function to process SQL-like commands
void processSQLCommand(const string &command) {
    stringstream ss(command);
    string keyword, filename;

    ss >> keyword;

    if (keyword == "CREATE") {
        ss >> keyword;
        if (keyword == "DATABASE") {
            ss >> filename;
            ofstream outfile(filename);
            if (outfile) {
                cout << "Database '" << filename << "' created successfully.\n";
                outfile.close();
            } else {
                cout << "Error: Could not create database file '" << filename << "'.\n";
            }
        } else {
            cout << "Error: Invalid syntax. Did you mean 'CREATE DATABASE'?\n";
        }
    } else {
        cout << "Error: Unknown command.\n";
    }
}

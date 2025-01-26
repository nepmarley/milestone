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
void deleteRows(const string &whereColumn, const string &whereValue);
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
        cin.ignore(); // clear buffer

        switch (choice) {
            case 1:
                ReadFromFile();
                break;
            case 2:
                createTable();
                break;
            case 3: {
                string command;
                cout << "Select the SQL-like command (e.g., CREATE DATABASE <filename>):\n";
                getline(cin, command);
                processSQLCommand(command);
                break;
            }
            default:
                cout << "Invalid choice. Exiting program.\n";
                break;
        }

        cout << "Do you want to continue? (1 - Yes, 2 - No): "; // asks user if they want to proceed
        cin >> choice;
        cin.ignore(); // clear buffer as cin and getline are both used

    } while (choice == 1); // runs when choice = 1

    return 0;
}

// Function to display the table
void tableIndex(const vector<string> &table, int Rows, int Cols) {
    if (table.empty()) {
        cout << "The table is empty.\n"; // return this message if table is empty
        return;
    }

    for (int i = 0; i < Rows; i++) {
        for (int j = 0; j < Cols; j++) {
            int index = i * Cols + j;
            cout << setw(15) << table[index] << " | ";
        }
        cout << endl;
    }
}

// Function to create a new table
void createTable() {
    table.clear(); // clears data from table
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

    numOfRows++; // increment
    cout << "Table created successfully with " << numOfCols << " columns.\n"; // notifies user that table has been created

    // Display the empty table structure
    cout << "\nTable structure:\n";
    tableIndex(table, 1, numOfCols);

    // prompts user to add rows to the table
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

    // write table rows
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

// function to add rows to the table
void addRowsToTable() {
    cout << "Enter table rows (comma-separated values). Type 'done' to finish.\n";

    while (true) {
        cout << "Enter row " << numOfRows + 1 << ": ";
        string input;
        getline(cin, input); // Get the entire row input as a string

        if (input == "done") {  // Check for 'done' to stop row entry
            break;
        }

        stringstream ss(input);
        string value;
        vector<string> rowData;
        int colCount = 0;

        while (getline(ss, value, ',')) {
            rowData.push_back(value);
            colCount++;
        }

        // Ensure row matches the number of columns
        if (colCount != numOfCols) {
            cout << "Error: Row must contain exactly " << numOfCols << " values. Try again.\n";
            continue;
        }

        // push row to the table
        for (const auto &item : rowData) {
            table.push_back(item);
        }

        numOfRows++;  // row increment count
    }

    // display updated table
    cout << "\nUpdated Table:\n";
    tableIndex(table, numOfRows, numOfCols);
}

// function to read the table from a file
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
            localRows++; // increment
        }

        cout << "Table read successfully:\n";
        tableIndex(localTable, localRows, localCols);
    } else {
        cout << "Error opening file.\n";
    }
}

// function to process the SQL-like commands
void processSQLCommand(const string &command) {
    stringstream ss(command);
    string keyword, tableName, whereColumn, whereValue;

    ss >> keyword;

    if (keyword == "CREATE") {
        ss >> keyword;  // stores "DATABASE" into keyword
        if (keyword == "DATABASE") {
            ss >> tableName;
            ofstream outfile(tableName);
            if (outfile) {
                cout << "Database '" << tableName << "' created successfully.\n";
                outfile.close();
            } else {
                cout << "Error: Could not create database file '" << tableName << "'.\n";
            }
        } else {
            cout << "Error: Invalid syntax. Did you mean 'CREATE DATABASE'?\n";
        }
    }
    else if (keyword == "INSERT") {
        ss >> keyword;  // stores "INTO"
        if (keyword == "INTO") {
            ss >> tableName;  // get table name
        }
    }
    else if (keyword == "SELECT") {
        ss >> keyword;  // stores "FROM"
        if (keyword == "FROM") {
            ss >> tableName;  // Select data logic here
        }
    }
    else if (keyword == "DELETE") {  // add handling for DELETE command
        ss >> keyword;  // stores "FROM"
        if (keyword == "FROM") {
            ss >> tableName;  // Get table name
            ss >> keyword;  // stores "WHERE"
            if (keyword == "WHERE") {
                ss >> whereColumn >> whereValue;
                deleteRows(whereColumn, whereValue);  // call deleteRows function
            }
        }
    }
    else {
        cout << "Error: Unknown command.\n";
    }
}

void deleteRows(const string &whereColumn, const string &whereValue) {
    vector<string> updatedTable;
    int columnIndex = -1; // flag

    // find index of column where condition will be applied
    for (int i = 0; i < numOfCols; i++) {
        if (table[i] == whereColumn) {
            columnIndex = i;
            break;
        }
    }

    // If column not found, output error and return
    if (columnIndex == -1) {
        cout << "Error: Column '" << whereColumn << "' not found.\n";
        return;
    }

    for (int i = numOfCols; i < table.size(); i++) {
        int rowIndex = (i - numOfCols) / numOfCols;  // Count row index
        int colIndex = i % numOfCols;  // Count column index

        // if column index matches and value matches condition skip row
        if (colIndex == columnIndex && table[i] == whereValue) {
            continue;
        }

        // else keep item in updated table
        updatedTable.push_back(table[i]);
    }

    // table is updated with new data
    table = updatedTable;
    numOfRows = table.size() / numOfCols;  // recount rows

    cout << "Rows deleted successfully where " << whereColumn << " = " << whereValue << ".\n";
    tableIndex(table, numOfRows, numOfCols);  // display updated table
}

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
string tableName;

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
        cout << "4. Exit\n";
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
                cout << "Enter an SQL-like command (e.g., CREATE TABLE players):\n";
                getline(cin, command);
                processSQLCommand(command);
                break;
            }
            case 4:
                cout << "Exiting program.\n";
                break;
            default:
                cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 4);

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
            cout << setw(15) << table[index] << " | ";
        }
        cout << endl;
    }
}

// Function to create a new table
void createTable() {
    table.clear(); // Clears existing table data
    numOfRows = 0;
    numOfCols = 0;

    cout << "Enter the number of columns: ";
    cin >> numOfCols;
    cin.ignore(); // Clear buffer

    cout << "Enter column names (separated by commas): ";
    string columnNames;
    getline(cin, columnNames);

    stringstream ss(columnNames);
    string columnName;

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

    // Display the empty table structure
    cout << "\nTable structure:\n";
    tableIndex(table, 1, numOfCols);

    // Prompt the user to add rows
    addRowsToTable();
}

// Function to add rows to the table
void addRowsToTable() {
    cout << "Enter table rows (comma-separated values). Type 'done' to finish.\n";

    while (true) {
        cout << "Enter row " << numOfRows + 1 << ": ";
        string input;
        getline(cin, input);

        if (input == "done") {
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

        if (colCount != numOfCols) {
            cout << "Error: Row must contain exactly " << numOfCols << " values. Try again.\n";
            continue;
        }

        for (const auto &item : rowData) {
            table.push_back(item);
        }

        numOfRows++;
    }

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
    string keyword;

    ss >> keyword;

    if (keyword == "CREATE") {
        ss >> keyword;
        if (keyword == "TABLE") {
            ss >> tableName;
            string columns;
            getline(ss, columns, '('); // Skip to column definitions
            getline(ss, columns, ')'); // Extract column definitions
            stringstream colStream(columns);
            string col;
            while (getline(colStream, col, ',')) {
                table.push_back(col);
            }
            numOfCols = table.size(); // Number of columns
            numOfRows = 1; // First row for column names
            cout << "Table '" << tableName << "' created successfully with " << numOfCols << " columns.\n";
            tableIndex(table, numOfRows, numOfCols);
        }
    }
    else if (keyword == "INSERT") {
        ss >> keyword;
        if (keyword == "INTO") {
            ss >> tableName;
            string values;
            getline(ss, values, '('); // Skip to values
            getline(ss, values, ')'); // Extract values
            stringstream valueStream(values);
            string value;
            vector<string> rowData;

            while (getline(valueStream, value, ',')) {
                rowData.push_back(value);
            }

            if (rowData.size() != numOfCols) {
                cout << "Error: Number of values does not match the number of columns.\n";
            } else {
                for (const auto &val : rowData) {
                    table.push_back(val);
                }
                numOfRows++;
                cout << "Row inserted into table '" << tableName << "' successfully.\n";
            }
        }
    }
    else if (keyword == "SELECT") {
        ss >> keyword;
        if (keyword == "*") {
            ss >> keyword;
            if (keyword == "FROM") {
                ss >> tableName;
                cout << "Displaying all rows from '" << tableName << "':\n";
                tableIndex(table, numOfRows, numOfCols);
            }
        }
    }
    else if (keyword == "UPDATE") {
        ss >> tableName;
        string setCol, setValue, whereCol, whereValue;
        ss >> keyword; // Skip "SET"
        ss >> setCol;
        ss.ignore(); // Skip '='
        ss >> setValue; // Get value to set
        ss >> keyword; // Skip "WHERE"
        ss >> whereCol;
        ss.ignore(); // Skip '='
        ss >> whereValue; // Get condition value

        for (int i = numOfCols; i < table.size(); i++) {
            int colIndex = i % numOfCols;
            if (table[colIndex] == whereCol && table[i] == whereValue) {
                table[i] = setValue;
            }
        }
        cout << "Rows updated successfully.\n";
    }
    else if (keyword == "DELETE") {
        cout << "DELETE is not supported in this implementation.\n";
    }
    else {
        cout << "Unknown or unsupported command.\n";
    }
}

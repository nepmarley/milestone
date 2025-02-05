#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <fstream>

using namespace std;

// Global variables
vector<string> table; 
int numOfRows = 0, numOfCols = 0; // Tracks rows and columns for the table
string tableName;
string databaseName = "";    // To store the current database name
string outputFileName = "";  // To store the output file name

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
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(); 

        switch (choice) {
            case 1:
                ReadFromFile();
                break;
            case 2:
                createTable();
                break;
            case 3: {
                string command;
                cout << "\nSQL-like Commands Help:" << endl;
                cout << "1. CREATE DATABASE <database_name>;" << endl; 
                cout << "2. CREATE TABLE <table_name> (<col1>,<col2>,...)" << endl;
                cout << "3. INSERT INTO <table_name> VALUES (<val1>,<val2>,...)" << endl;
                cout << "4. SELECT * FROM <table_name>" << endl;
                cout << "5. UPDATE <table_name> SET <col>=<value> WHERE <condition>" << endl;
                cout << "6. DELETE FROM <table_name> WHERE <condition>" << endl;
                cout << "7. SELECT COUNT(*) FROM <table_name>\n" << endl;
                cout << "Type 'exit' to finish and view the final table.\n";

                // Start SQL 
                do {
                    cout << "\nEnter an SQL-like command: ";
                    getline(cin, command);

                    if (command == "exit") {
                        break;  // End session and display the final table
                    } else {
                        // Process the SQL-like command
                        processSQLCommand(command);
                    }
                } while (true);

                // Display the final table
                cout << "\nFinal Table:\n";
                tableIndex(table, numOfRows, numOfCols);
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
    table.clear();
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
        cout << "Error: Number of column names does not match the number of columns.\n"; //error handling 1
        table.clear();
        return;
    }

    numOfRows++;
    cout << "Table created successfully with " << numOfCols << " columns.\n";
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
            cout << "Error: Row must contain exactly " << numOfCols << " values. Try again.\n"; //error handling 2
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

    cout << "Enter the filename (with extension, e.g., table.csv): ";
    cin >> filename;

    inputFile.open(filename);
    if (!inputFile.is_open()) {
        cout << "Error: Could not open the file '" << filename << "'. Please check the file name and try again.\n"; //error handling 3
        return;
    }

    while (getline(inputFile, line)) {
        stringstream ss(line);
        string value;
        int currentCols = 0;

        while (getline(ss, value, ',')) {
            localTable.push_back(value);
            currentCols++;
        }

        if (localCols == 0) {
            localCols = currentCols;
        } else if (currentCols != localCols) {
            cout << "Error: Inconsistent number of columns in the file.\n"; //error handling 4
            inputFile.close();
            return;
        }

        localRows++;
    }

    inputFile.close();

    table = localTable;
    numOfRows = localRows;
    numOfCols = localCols;

    cout << "Table read successfully from '" << filename << "':\n";
    tableIndex(table, numOfRows, numOfCols);
}

// Function to process SQL-like commands
void processSQLCommand(const string &command) {
    stringstream ss(command);
    string keyword;
    ss >> keyword;

    if (keyword == "CREATE") {
        ss >> keyword;
        if (keyword == "DATABASE") {
            // Parse database name
            ss >> databaseName;
            databaseName = databaseName.substr(0, databaseName.find(";")); // Remove ';' if exists
            outputFileName = "C:\\mariadb\\" + databaseName + ".mdb"; // Output file path

            cout << "Output File Name: " << outputFileName << endl; // Debugging
            cout << "Database Name: " << databaseName << endl;     // Debugging

            ofstream outputFile(outputFileName);
            if (outputFile.is_open()) {
                cout << "Database successfully created\n";
                outputFile.close();
            } else {
                cout << "Error: Unable to create database file\n"; //error handling 5
            }
        } else if (keyword == "TABLE") {
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
    } else if (keyword == "USE") {
        ss >> databaseName;
        databaseName = databaseName.substr(0, databaseName.find(";")); // Remove ';' if exists
        cout << "Using database: " << databaseName << endl;
    } else if (keyword == "INSERT") {
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
    } else if (keyword == "SELECT") {
        ss >> keyword;
        if (keyword == "*") {
            ss >> keyword;
            if (keyword == "FROM") {
                ss >> tableName;
                cout << "Displaying all rows from '" << tableName << "':\n";
                tableIndex(table, numOfRows, numOfCols);
            }
        }
    } else if (keyword == "UPDATE") {
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
    } else if (keyword == "DELETE") {
        string fromKeyword, whereColumn, whereValue;
        ss >> fromKeyword >> tableName >> whereColumn;
        ss.ignore(); // Skip '='
        ss >> whereValue;

        if (fromKeyword == "FROM") {
            deleteRows(whereColumn, whereValue);
        } else {
            cout << "Invalid DELETE syntax.\n";
        }
    }
    else {
        cout << "Unknown or unsupported command.\n";
    }
}

}

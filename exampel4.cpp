#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <iomanip>
#include <sstream>

using namespace std;

// Function declarations
void tableIndex(const vector<string> &table, int Rows, int Cols);
void writeOutputFile(const string &csvIn, const string &txtOut);
void updateTableRow(vector<string> &table, int numOfRows, int numOfCols);
void deleteTableRow(vector<string> &table, int &numOfRows, int numOfCols);
void Tabletofile();
void ReadFromFile();
void processSQLCommand(const string &command);

int main() {
    // File names for CSV and text files
    string csvFile = "Players.csv";
    string outputFile = "output.txt";

    // Convert CSV to plain text file
    writeOutputFile(csvFile, outputFile);

    int choice;

    do {
        // Display menu options to the user
        cout << "Choose an option:\n";
        cout << "1. Read from a file\n";
        cout << "2. Create a new file\n";
        cout << "3. Execute SQL-like command\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(); // Ignore any leftover newline character

        switch (choice) {
            case 1:
                // Option to read data from a file
                ReadFromFile();
                break;
            case 2:
                // Option to create a new file
                Tabletofile();
                break;
            case 3:
                // Option to process SQL-like commands
                {
                    string command;
                    cout << "Enter SQL-like command (e.g., CREATE DATABASE <filename>):\n";
                    getline(cin, command);
                    processSQLCommand(command);
                }
                break;
            default:
                // Handle invalid input
                cout << "Invalid choice. Exiting program.\n";
                break;
        }

        // Ask the user if they want to continue
        cout << "Do you want to continue? (1 - Yes, 2 - No): ";
        cin >> choice;
        cin.ignore(); // Ignore any leftover newline character

    } while (choice == 1);

    return 0;
}

// Function definitions

// Function to display the table
void tableIndex(const vector<string> &table, int Rows, int Cols) {
    // Loop through rows and columns to display table data
    for (int i = 0; i < Rows; i++) {
        for (int j = 0; j < Cols; j++) {
            int index = i * Cols + j;
            if (index < table.size())
                cout << setw(15) << table[index] << " | "; // Display each cell
            else
                cout << setw(15) << ""; // Handle incomplete rows
        }
        cout << endl;
    }
}

// Function to write CSV data to a plain text file
void writeOutputFile(const string &csvIn, const string &txtOut) {
    ifstream inputFile(csvIn); // Open input CSV file
    ofstream outputFile(txtOut); // Open output text file

    if (!inputFile.is_open()) {
        cout << "Error opening input CSV file!" << endl;
        return;
    }
    if (!outputFile.is_open()) {
        cout << "Error opening output text file!" << endl;
        return;
    }

    string line;
    while (getline(inputFile, line)) {
        outputFile << line << endl; // Write each line from CSV to text file
    }

    inputFile.close();
    outputFile.close();

    cout << "CSV data is written to " << txtOut << endl;
}

// Function to process SQL-like commands
void processSQLCommand(const string &command) {
    stringstream ss(command);
    string keyword, filename, values;

    ss >> keyword;

    // Handle CREATE DATABASE command
    if (keyword == "CREATE") {
        ss >> keyword;
        if (keyword == "DATABASE") {
            ss >> filename;
            if (filename.empty()) {
                cerr << "Error: Missing filename in CREATE DATABASE command.\n";
                return;
            }

            ofstream outfile(filename);
            if (outfile) {
                cout << "Database '" << filename << "' created successfully.\n";
                outfile.close();
            } else {
                cerr << "Error: Could not create database file '" << filename << "'.\n";
            }
        } else {
            cerr << "Error: Invalid syntax. Did you mean 'CREATE DATABASE'?\n";
        }
    }

    // Handle INSERT INTO command
    else if (keyword == "INSERT") {
        ss >> keyword;
        if (keyword == "INTO") {
            ss >> filename;
            if (filename.empty()) {
                cerr << "Error: Missing filename in INSERT INTO command.\n";
                return;
            }

            size_t pos = command.find("VALUES");
            if (pos == string::npos) {
                cerr << "Error: Missing 'VALUES' in INSERT INTO command.\n";
                return;
            }

            values = command.substr(pos + 6); // Extract values after 'VALUES'
            ofstream outfile(filename, ios::app);
            if (outfile) {
                outfile << values << endl;
                cout << "Values '" << values << "' inserted into '" << filename << "' successfully.\n";
                outfile.close();
            } else {
                cerr << "Error: Could not open database file '" << filename << "'.\n";
            }
        } else {
            cerr << "Error: Invalid syntax. Did you mean 'INSERT INTO <filename>'?\n";
        }
    }

    // Handle SELECT * FROM command
    else if (keyword == "SELECT") {
        ss >> keyword;
        if (keyword == "*") {
            ss >> keyword;
            if (keyword == "FROM") {
                ss >> filename;
                if (filename.empty()) {
                    cerr << "Error: Missing filename in SELECT command.\n";
                    return;
                }

                ifstream infile(filename);
                if (infile) {
                    string line;
                    cout << "Contents of '" << filename << "':\n";
                    while (getline(infile, line)) {
                        cout << line << endl;
                    }
                    infile.close();
                } else {
                    cerr << "Error: Could not open database file '" << filename << "'.\n";
                }
            } else {
                cerr << "Error: Invalid syntax. Did you mean 'SELECT * FROM <filename>'?\n";
            }
        }
    }

    // Invalid command
    else {
        cerr << "Error: Unknown command '" << keyword << "'.\n";
    }
}

// Function to update a specific row and column in the table
void updateTableRow(vector<string> &table, int numOfRows, int numOfCols) {
    if (table.empty()) {
        cout << "No data to update as the table is empty :/\n";
        return;
    }

    int rowUpdate, colUpdate;
    string newValue;

    // Display the current table
    cout << "Current table data:\n";
    tableIndex(table, numOfRows, numOfCols);

    // Ask the user which row and column they want to update
    cout << "Enter the row number to update (1 to " << numOfRows << "): ";
    cin >> rowUpdate;
    if (rowUpdate < 1 || rowUpdate > numOfRows) {
        cout << "Invalid row number.\n";
        return;
    }
    rowUpdate--; // Convert to zero-indexed

    cout << "Enter the column number to update (1 to " << numOfCols << "): ";
    cin >> colUpdate;
    if (colUpdate < 1 || colUpdate > numOfCols) {
        cout << "Invalid column number.\n";
        return;
    }
    colUpdate--; // Convert to zero-indexed

    // Ask the user for the new value
    cout << "Enter the new value for row " << rowUpdate + 1 << ", column " << colUpdate + 1 << ": ";
    cin >> newValue;

    // Calculate the index in the 1D table vector
    int index = rowUpdate * numOfCols + colUpdate;
    table[index] = newValue; // Update the value

    cout << "Row updated successfully!\n";

    // Re-display the updated table
    cout << "Updated table data:\n";
    tableIndex(table, numOfRows, numOfCols);
}

// Function to delete a specific row from the table
void deleteTableRow(vector<string> &table, int &numOfRows, int numOfCols) {
    int rowDelete;

    // Display the current table
    cout << "Current table data:\n";
    tableIndex(table, numOfRows, numOfCols);

    // Ask the user which row they want to delete
    cout << "Enter the row number to delete (1 to " << numOfRows << "): ";
    cin >> rowDelete;
    if (rowDelete < 1 || rowDelete > numOfRows) {
        cout << "Invalid row number.\n";
        return;
    }
    rowDelete--; // Convert to zero-indexed

    // Calculate the index of the first element in the row to delete
    int startIdx = rowDelete * numOfCols;

    // Remove the elements from the table
    table.erase(table.begin() + startIdx, table.begin() + startIdx + numOfCols);

    // Decrease the row count
    numOfRows--;

    cout << "Row deleted successfully!\n";

    // Re-display the updated table
    cout << "Updated table data:\n";
    tableIndex(table, numOfRows, numOfCols);
}

// Function to create a new table and write it to a file
void Tabletofile() {
    ofstream outputFile;
    string filename, input;
    int cols, numOfRows = 0, numOfCols = 0;
    vector<string> table;

    // Ask user for filename to create
    cout << "Enter the filename to create: ";
    cin >> filename;

    outputFile.open(filename);

    if (outputFile) {
        // Ask user for number of columns in the table
        cout << "Enter the number of columns: ";
        cin >> cols;
        numOfCols = cols;

        cout << "Enter data row by row. Type 'done' to finish.\n";
        while (true) {
            cout << "Enter row " << numOfRows + 1 << " (comma-separated values): ";
            cin.ignore();
            getline(cin, input);

            if (input == "done")
                break; // Stop input if 'done' is entered

            stringstream ss(input);
            string word;
            int currentCols = 0;
            vector<string> rowData;

            while (getline(ss, word, ',')) {
                rowData.push_back(word);
                currentCols++;
            }

            if (currentCols != cols) {
                cout << "Incorrect number of columns. Expected " << cols << " columns.\n";
                continue; // Validate column count
            }

            // Write row data to file
            for (int i = 0; i < cols; ++i) {
                outputFile << rowData[i];
                if (i < cols - 1)
                    outputFile << ",";
            }
            outputFile << "\n";

            // Update in-memory table representation
            for (const auto &item : rowData)
                table.push_back(item);

            numOfRows++;
        }

        outputFile.close();
        cout << "File created successfully with " << numOfRows << " rows and " << numOfCols << " columns.\n";

        // Display the created table
        if (!table.empty()) {
            cout << "\nDisplaying the table:\n";
            tableIndex(table, numOfRows, numOfCols);
        }
    } else {
        cout << "Error creating the file.\n";
    }
}

// Function to read the table from a file
void ReadFromFile() {
    ifstream inputFile;
    string filename, line;
    vector<string> table;
    int numOfRows = 0, numOfCols = 0;

    // Ask user for filename to read from
    cout << "Enter the filename: ";
    cin >> filename;

    inputFile.open(filename);

    if (inputFile) {
        if (inputFile.peek() == ifstream::traits_type::eof()) {
            cout << "The file is empty.\n";
        } else {
            // Read the file line by line
            while (getline(inputFile, line)) {
                stringstream ss(line);
                string word;
                int currentCols = 0;

                while (getline(ss, word, ',')) {
                    table.push_back(word);
                    currentCols++;
                }

                if (numOfCols == 0) {
                    numOfCols = currentCols; // Set column count from first row
                } else if (currentCols != numOfCols) {
                    cout << "Inconsistent number of columns in row " << numOfRows + 1 << ".\n";
                    return; // Validate column consistency
                }

                numOfRows++;
            }

            // Display the table and row count
            tableIndex(table, numOfRows, numOfCols);
            cout << "Number of rows in the table: " << numOfRows << "\n";

            // Ask user if they want to update the table
            char updateChoice;
            cout << "Do you want to update any row? (y/n): ";
            cin >> updateChoice;
            if (tolower(updateChoice) == 'y') {
                updateTableRow(table, numOfRows, numOfCols);
            }

            // Ask user if they want to delete a row
            char deleteChoice;
            cout << "Do you want to delete a row? (y/n): ";
            cin >> deleteChoice;
            if (tolower(deleteChoice) == 'y') {
                deleteTableRow(table, numOfRows, numOfCols);
            }
        }

        inputFile.close();
    } else {
        cout << "Error opening the file.\n";
    }
}

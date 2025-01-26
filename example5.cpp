#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <iomanip>
#include <sstream>

using namespace std;

// Struct for table row representation
struct TableRow {
    vector<string> columns;
};

// Global variables
vector<TableRow> table;
int numCols = 0;
string tableName;
string outputFile;

// Function declarations
void displayMenu();
void processSQLCommand(const string &command);
void displayTable();
void saveTableToFile();

int main() {
    int continueChoice;

    do {
        displayMenu();
        string command;
        cout << "Enter SQL-like command: ";
        cin.ignore();
        getline(cin, command);
        processSQLCommand(command);

        // Save table to file after every command
        saveTableToFile();

        cout << "Do you want to continue? (1 - Yes, 2 - No): ";
        cin >> continueChoice;
    } while (continueChoice == 1);

    return 0;
}

// Function to display the menu
void displayMenu() {
    cout << "\nSupported Commands:\n";
    cout << "1. CREATE TABLE <table_name>(<column1> <type1>, ...)<br>";
    cout << "2. INSERT INTO <table_name> VALUES (<value1>, ...)<br>";
    cout << "3. SELECT * FROM <table_name><br>";
    cout << "4. UPDATE <table_name> SET <column>=<value> WHERE <column>=<value><br>";
    cout << "5. DELETE FROM <table_name> WHERE <column>=<value><br>";
    cout << "6. SELECT COUNT(*) FROM <table_name><br>\n";
}

// Function to process SQL-like commands
void processSQLCommand(const string &command) {
    stringstream ss(command);
    string keyword;
    ss >> keyword;

    if (keyword == "CREATE") {
        string type, name;
        ss >> type >> name;

        if (type == "TABLE") {
            tableName = name;
            string columnsDef;
            getline(ss, columnsDef, ')');
            columnsDef = columnsDef.substr(columnsDef.find('(') + 1);

            stringstream colStream(columnsDef);
            string column;
            table.clear();
            numCols = 0;

            while (getline(colStream, column, ',')) {
                column.erase(remove(column.begin(), column.end(), ' '), column.end());
                numCols++;
            }

            cout << "Table '" << tableName << "' created with " << numCols << " columns.\n";
        } else {
            cerr << "Invalid CREATE command.\n";
        }
    } else if (keyword == "INSERT") {
        string into, name, values;
        ss >> into >> name;

        if (into == "INTO" && name == tableName) {
            size_t pos = command.find("VALUES");
            if (pos == string::npos) {
                cerr << "Error: Missing 'VALUES' in INSERT command.\n";
                return;
            }

            values = command.substr(pos + 6);
            values.erase(remove(values.begin(), values.end(), '('), values.end());
            values.erase(remove(values.begin(), values.end(), ')'), values.end());

            stringstream valueStream(values);
            string value;
            TableRow row;

            while (getline(valueStream, value, ',')) {
                row.columns.push_back(value);
            }

            if (row.columns.size() != numCols) {
                cerr << "Error: Column count mismatch.\n";
                return;
            }

            table.push_back(row);
            cout << "Row inserted successfully into '" << tableName << "'.\n";
        } else {
            cerr << "Invalid INSERT command.\n";
        }
    } else if (keyword == "SELECT") {
        string all, from, name;
        ss >> all >> from >> name;

        if (all == "*" && from == "FROM" && name == tableName) {
            displayTable();
        } else if (all == "COUNT(*)" && from == "FROM" && name == tableName) {
            cout << "Row count: " << table.size() << "\n";
        } else {
            cerr << "Invalid SELECT command.\n";
        }
    } else if (keyword == "UPDATE") {
        string name, set, where;
        ss >> name >> set;

        if (name == tableName && set == "SET") {
            string setClause, whereClause;
            getline(ss, setClause, 'W');
            ss >> where >> whereClause;

            if (where != "WHERE") {
                cerr << "Invalid UPDATE syntax.\n";
                return;
            }

            size_t equalsPosSet = setClause.find('=');
            size_t equalsPosWhere = whereClause.find('=');

            string setColumn = setClause.substr(0, equalsPosSet);
            string setValue = setClause.substr(equalsPosSet + 1);
            string whereColumn = whereClause.substr(0, equalsPosWhere);
            string whereValue = whereClause.substr(equalsPosWhere + 1);

            for (auto &row : table) {
                if (row.columns[stoi(whereColumn)] == whereValue) {
                    row.columns[stoi(setColumn)] = setValue;
                }
            }

            cout << "Rows updated successfully.\n";
        } else {
            cerr << "Invalid UPDATE command.\n";
        }
    } else if (keyword == "DELETE") {
        string from, name, where;
        ss >> from >> name >> where;

        if (from == "FROM" && name == tableName && where == "WHERE") {
            string whereClause;
            getline(ss, whereClause);

            size_t equalsPos = whereClause.find('=');
            string whereColumn = whereClause.substr(0, equalsPos);
            string whereValue = whereClause.substr(equalsPos + 1);

            table.erase(
                remove_if(table.begin(), table.end(), [&](const TableRow &row) {
                    return row.columns[stoi(whereColumn)] == whereValue;
                }),
                table.end());

            cout << "Rows deleted successfully.\n";
        } else {
            cerr << "Invalid DELETE command.\n";
        }
    } else {
        cerr << "Unknown command: " << keyword << "\n";
    }
}

// Function to display the table in CSV format
void displayTable() {
    for (const auto &row : table) {
        for (size_t i = 0; i < row.columns.size(); ++i) {
            cout << row.columns[i];
            if (i < row.columns.size() - 1) cout << ",";
        }
        cout << endl;
    }
}

// Function to save the table to a file
void saveTableToFile() {
    if (outputFile.empty()) {
        outputFile = "output_" + tableName + ".csv";
    }

    ofstream file(outputFile);

    if (file) {
        for (const auto &row : table) {
            for (size_t i = 0; i < row.columns.size(); ++i) {
                file << row.columns[i];
                if (i < row.columns.size() - 1) file << ",";
            }
            file << endl;
        }

        file.close();
        cout << "Table saved to " << outputFile << "\n";
    } else {
        cerr << "Error saving table to file.\n";
    }
}

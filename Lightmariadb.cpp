#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <iomanip>
#include <sstream>
using namespace std;

// Function to display the table
void tableIndex(const vector<string> &table, int Rows, int Cols)
{
    for (int i = 0; i < Rows; i++)
    {
        for (int j = 0; j < Cols; j++)
        {
            int index = i * Cols + j;
            if (index < table.size())
                cout << setw(15) << table[index];
            else
                cout << setw(15) << ""; // Handle cases where the table might be incomplete
        }
        cout << endl;
    }
}

void updateTableRow(vector<string> &table, int numOfRows, int numOfCols)
{
    int rowToUpdate, colToUpdate;
    string newValue;

    // Display the current table
    cout << "Current table data:\n";
    tableIndex(table, numOfRows, numOfCols);

    // Ask the user which row and column they want to update
    cout << "Enter the row number to update (1 to " << numOfRows << "): ";
    cin >> rowToUpdate;
    if (rowToUpdate < 1 || rowToUpdate > numOfRows)
    {
        cout << "Invalid row number.\n";
        return;
    }
    rowToUpdate--; // Convert to zero-indexed

    cout << "Enter the column number to update (1 to " << numOfCols << "): ";
    cin >> colToUpdate;
    if (colToUpdate < 1 || colToUpdate > numOfCols)
    {
        cout << "Invalid column number.\n";
        return;
    }
    colToUpdate--; // Convert to zero-indexed

    // Calculate the index in the 1D table vector
    int index = rowToUpdate * numOfCols + colToUpdate;

    // Display the current value before updating
    cout << "The current value at row " << rowToUpdate + 1 << ", column " << colToUpdate + 1 << " is: " << table[index] << endl;

    // Ask the user for the new value
    cout << "Enter the new value for row " << rowToUpdate + 1 << ", column " << colToUpdate + 1 << ": ";
    cin >> newValue;

    table[index] = newValue;

    cout << "Row updated successfully!\n";

    // Re-display the updated table
    cout << "Updated table data:\n";
    tableIndex(table, numOfRows, numOfCols);
}

void deleteTableRow(vector<string> &table, int &numOfRows, int numOfCols)
{
    int rowToDelete;

    // Display the current table
    cout << "Current table data:\n";
    tableIndex(table, numOfRows, numOfCols);

    // Ask the user which row they want to delete
    cout << "Enter the row number to delete (1 to " << numOfRows << "): ";
    cin >> rowToDelete;
    if (rowToDelete < 1 || rowToDelete > numOfRows)
    {
        cout << "Invalid row number.\n";
        return;
    }
    rowToDelete--; // Convert to zero-indexed

    // Calculate the index of the first element in the row to delete
    int startIdx = rowToDelete * numOfCols;

    // Remove the elements from the table
    table.erase(table.begin() + startIdx, table.begin() + startIdx + numOfCols);

    // Decrease the row count
    numOfRows--;

    cout << "Row deleted successfully!\n";

    // Re-display the updated table
    cout << "Updated table data:\n";
    tableIndex(table, numOfRows, numOfCols);
}

void Tabletofile()
{
    ofstream outputFile;
    string filename;
    string input;
    int cols;
    int numOfRows = 0, numOfCols = 0;
    vector<string> table;

    // Ask the user for the filename
    cout << "=== Create a New File ===\n";
    cout << "Enter the filename to create (e.g., data.csv): ";
    cin >> filename;

    outputFile.open(filename);

    if (outputFile)
    {
        // Ask the user for the number of columns
        cout << "\nHow many columns will the table have? ";
        cin >> cols;
        numOfCols = cols;

        cout << "\nYou will now enter data row by row.\n";
        cout << "Note:\n";
        cout << "  - Enter values separated by commas (e.g., value1,value2,value3).\n";
        cout << "  - Ensure the number of values matches the number of columns (" << cols << ").\n";
        cout << "  - Type 'done' when you are finished entering data.\n\n";

        while (true)
        {
            cout << "Enter data for row " << numOfRows + 1 << ": ";
            getline(cin >> ws, input); // 'ws' consumes any leading whitespace

            // Check if the user wants to finish
            if (input == "done")
            {
                break;
            }

            stringstream ss(input);
            string word;
            int currentCols = 0;
            vector<string> rowData;

            // Split the input by commas
            while (getline(ss, word, ','))
            {
                rowData.push_back(word);
                currentCols++;
            }

            // Validate the number of columns in the input
            if (currentCols != cols)
            {
                cout << "Error: You entered " << currentCols
                     << " values, but " << cols << " are required. Please try again.\n";
                continue;
            }

            // Write the row to the file
            for (int i = 0; i < cols; ++i)
            {
                outputFile << rowData[i];
                if (i < cols - 1)
                {
                    outputFile << ",";
                }
            }
            outputFile << "\n";

            // Update the table data for later display
            for (const auto &item : rowData)
            {
                table.push_back(item);
            }

            numOfRows++;
        }

        // Close the file
        outputFile.close();
        cout << "\nFile created successfully with " << numOfRows << " rows and " << numOfCols << " columns.\n";

        // Display the table
        if (!table.empty())
        {
            cout << "\nHere is your table:\n";
            tableIndex(table, numOfRows, numOfCols);
        }
    }
    else
    {
        cout << "Error: Unable to create the file. Please check the filename or permissions.\n";
    }
}

void ReadFromFile()
{
    ifstream inputFile;
    string filename;
    string line;
    vector<string> table;
    int numOfRows = 0, numOfCols = 0;

    cout << "Enter the filename: ";
    cin >> filename;

    inputFile.open(filename);

    if (inputFile)
    {
        if (inputFile.peek() == ifstream::traits_type::eof())
        {
            cout << "The file is empty.\n";
        }
        else
        {
            // Read the file line by line
            while (getline(inputFile, line))
            {
                stringstream ss(line);
                string word;
                int currentCols = 0;

                while (getline(ss, word, ','))
                {
                    table.push_back(word);
                    currentCols++;
                }

                if (numOfCols == 0)
                {
                    numOfCols = currentCols;
                }
                else if (currentCols != numOfCols)
                {
                    cout << "Inconsistent number of columns in row " << numOfRows + 1 << ".\n";
                    return;
                }

                numOfRows++;
            }

            tableIndex(table, numOfRows, numOfCols);

            char updateChoice;
            cout << "Do you want to update any row? (y/n): ";
            cin >> updateChoice;
            if (tolower(updateChoice) == 'y')
            {
                updateTableRow(table, numOfRows, numOfCols);
            }

            char deleteChoice;
            cout << "Do you want to delete a row? (y/n): ";
            cin >> deleteChoice;
            if (tolower(deleteChoice) == 'y')
            {
                deleteTableRow(table, numOfRows, numOfCols);
            }
        }

        inputFile.close();
    }
    else
    {
        cout << "Error opening the file.\n";
    }
}

int main()
{
    int choice;
    int continueChoice;
    do {
        cout << "Choose an option:\n";
        cout << "1. Read from a file\n";
        cout << "2. Create a new file\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            ReadFromFile();
            break;
        case 2:
            Tabletofile();
            break;
        default:
            cout << "Invalid choice.\n";
            break;
        }

        cout << "Do you want to continue? (1 - Yes, 2 - No): ";
        cin >> continueChoice;

    } while (continueChoice == 1);

    return 0;
}

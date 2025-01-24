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
    for (int row = 0; row < Rows; row++)
    {
        for (int col = 0; col < Cols; col++)
        {
            int index = row * Cols + col;
            if (index < table.size())
                cout << setw(15) << table[index];
            else
                cout << setw(15) << ""; // Handle cases where the table might be incomplete
        }
        cout << endl;
    }
}

// Function to write table to the file
void Tabletofile()
{
    ofstream outputFile;
    string filename;
    string input;
    int cols;
    int numOfRows = 0, numOfCols = 0;
    vector<string> table;

    cout << "Enter the filename to create: ";
    cin >> filename;

    outputFile.open(filename);

    if (outputFile)
    {
        cout << "Enter the number of columns: ";
        cin >> cols;
        numOfCols = cols;

        cout << "Enter data row by row. Type 'done' to finish.\n";
        while (true)
        {
            cout << "Enter row " << numOfRows + 1 << " (comma-separated values): ";
            getline(cin >> ws, input); // 'ws' to consume any leading whitespace

            if (input == "done")
            {
                break;
            }

            stringstream ss(input);
            string word;
            int currentCols = 0;
            vector<string> rowData;

            // Read each word separated by comma
            while (getline(ss, word, ','))
            {
                rowData.push_back(word);
                currentCols++;
            }

            if (currentCols != cols)
            {
                cout << "Incorrect number of columns. Expected " << cols << " columns.\n";
                continue;
            }

            // Write to file
            for (int i = 0; i < cols; ++i)
            {
                outputFile << rowData[i];
                if (i < cols - 1)
                {
                    outputFile << ",";
                }
            }
            outputFile << "\n";

            // Update table data
            for (const auto &item : rowData)
            {
                table.push_back(item);
            }

            numOfRows++;
        }

        outputFile.close();
        cout << "File created successfully with " << numOfRows << " rows and " << numOfCols << " columns.\n";

        // Display the table
        if (!table.empty())
        {
            cout << "\nDisplaying the table:\n";
            tableIndex(table, numOfRows, numOfCols);
        }
    }
    else
    {
        cout << "Error creating the file.\n";
    }
}

// Function to read the table from the file
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
        cout << "Invalid choice. Exiting program.\n";
        break;
    }

    return 0;
}

// *********************************************************
// Program: Lightmriadb.cpp
// Course: CCP6114 Programming Fundamentals
// Lecture Class: TC3L
// Tutorial Class: TT5L
// Trimester: 2430
// Member_1: 243UC2462Z| KASHVIN A\L MUTHU MANI GEORGE |KASHVIN.MUTHU.MANI@student.mmu.edu.my | +60 11-1149 6783
// Member_2: 243UC2461M | MUHAMMAD HANIF ‘AMALI BIN SYAUQI | MUHAMMAD.HANIF.AMALI@student.mmu.edu.my  | +60 11-1039 1973
// Member_3: 243UC245DJ | MUHAMMAD ANIQ SAFWAN BIN MUHAMAD SAFRI | MUHAMMAD.ANIQ.SAFWAN@student.mmu.edu.my  | +60 11-25453402
// Member_4: 243UC24635| MUHAMMAD FAHMI AIMAN BIN MOHD FAUZI| MUHAMMAD.FAHMI.AIMAN@student.mmu.edu.my  | +60 11-3714 5916
// *********************************************************
// Task Distribution
// Member_1:25%
// Member_2:25%
// Member_3:25%
// Member_4:25%
// *********************************************************
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
            if(index < table.size())
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
                // Optionally, you can choose to retry the current row or handle the error as needed
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

int main()
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
                // Use a stringstream to parse each line into comma-separated values
                stringstream ss(line);
                string word;
                int currentCols = 0;

                // Read each word separated by comma
                while (getline(ss, word, ','))
                {
                    table.push_back(word);
                    currentCols++;
                }

                // Set the number of columns based on the first line
                if (numOfCols == 0)
                {
                    numOfCols = currentCols;
                }
                else
                {
                    // Check for consistent number of columns
                    if (currentCols != numOfCols)
                    {
                        cout << "Inconsistent number of columns in row " << numOfRows + 1 << ".\n";
                        return 1; // Exit the program due to inconsistency
                    }
                }

                numOfRows++;
            }

            // Display the table
            tableIndex(table, numOfRows, numOfCols);
        }

        inputFile.close(); // Single close call
    }
    else
    {
        cout << "Error opening the file.\n";
    }

    return 0;
}


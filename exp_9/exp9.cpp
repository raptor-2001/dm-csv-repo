#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <climits>
#include <cmath>

using namespace std;

// Function to read CSV data from a file and return it as a vector of vectors of strings
vector<int> readCSV(const string &filename)
{
    vector<int> data;
    fstream file(filename, ios::in);

    if (!file.is_open())
    {
        cerr << "Error in opening input file: " << filename << endl;
        exit(1);
    }

    string line, value;
    int lineCount = 0;

    while (getline(file, line))
    {
        if (lineCount > 0)
        {
            stringstream ss(line);
            getline(ss, value, ',');
            data.push_back(stoi(value));
        }
        lineCount++;
    }

    file.close();
    return data;
}

// Function to write CSV data to a file
void writeCSV(const string &filename, float cf)
{
    ofstream file(filename, ios::out);

    if (!file.is_open())
    {
        cerr << "Error in opening output file: " << filename << endl;
        exit(1);
    }

    file << "Correlation Coefficient -> "
         << cf << endl;
    file.close();
}

float getCorrelationCoefficient(const vector<int> &a, const vector<int> &b)
{
    int n = a.size();
    int a_plus = 0, b_plus = 0, ab_plus = 0;

    for (int i = 0; i < n; i++)
    {
        a_plus += a[i] == 1 ? 1 : 0;
        b_plus += b[i] == 1 ? 1 : 0;
        ab_plus += (a[i] == 1 && b[i] == 1) ? 1 : 0;
    }

    if (a_plus == 0 || b_plus == 0)
    {
        return 0.0; // To handle cases where division by zero may occur
    }

    return static_cast<float>(ab_plus) / (a_plus * b_plus);
}

int main()
{

    vector<int> itemSet_1 = readCSV("correlation_input.csv");
    vector<int> itemSet_2 = readCSV("correlation_input.csv");

    float cf = getCorrelationCoefficient(itemSet_1, itemSet_2);

    writeCSV("correlation_output.csv", cf);

    cout << "correlation calculated correctly\n";
    return 0;
}
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <climits>
#include <cmath>
#include <algorithm>

using namespace std;

// Function to read CSV data from a file and return it as a vector of vectors of strings
vector<vector<string>> readCSV(const string &filename)
{
    vector<vector<string>> data;
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Error: File " << filename << " not found." << endl;
        return data;
    }

    string line;
    while (getline(file, line))
    {
        vector<string> row;
        istringstream lineStream(line);
        string cell;
        while (getline(lineStream, cell, ','))
        {
            row.push_back(cell);
        }
        data.push_back(row);
    }

    file.close();
    return data;
}

// Function to write CSV data to a file
void writeCSV(const string &filename, const vector<vector<string>> &data)
{
    ofstream file(filename);
    if (!file.is_open())
    {
        cerr << "Error: Unable to create or open file " << filename << " for writing." << endl;
        return;
    }

    for (const auto &row : data)
    {
        for (size_t i = 0; i < row.size(); ++i)
        {
            file << row[i];
            if (i < row.size() - 1)
            {
                file << ",";
            }
        }
        file << endl;
    }

    file.close();
}
// Function to calculate the median of a vector
double calculateMedian(vector<int> a)
{
    int size = a.size();
    if (size % 2 == 1)
        return a[size / 2];
    else
        return (a[(size / 2) - 1] + a[size / 2]) / 2.0;
}

// Function to calculate the first quartile (Q1)
double calculateQuartile1(vector<int> v)
{
    int n = v.size();
    vector<int> first;

    for (int i = 0; i < n / 2; i++)
    {
        first.push_back(v[i]);
    }
    return calculateMedian(first);
}

// Function to calculate the third quartile (Q3)
double calculateQuartile3(vector<int> v)
{
    int n = v.size();
    vector<int> last;
    if (n % 2 == 0)
    {
        for (int i = n / 2; i < n; i++)
        {
            last.push_back(v[i]);
        }
    }
    else
    {
        for (int i = n / 2 + 1; i < n; i++)
        {
            last.push_back(v[i]);
        }
    }
    return calculateMedian(last);
}

int main()
{
    vector<vector<string>> marks = readCSV("five_number_input.csv");

    int i = 0;
    string line, mark;
    vector<int> buffer;

    for (auto ele : marks[0])
    {
        int x = stoi(ele);
        buffer.push_back(x);
    }

    int n = buffer.size();
    sort(buffer.begin(), buffer.end());

    vector<vector<string>> fiveNumbers;

    vector<string> minimumValue;
    minimumValue.push_back("min: " + to_string(buffer[0]));
    fiveNumbers.push_back(minimumValue);

    vector<string> q1Value;
    q1Value.push_back("Q1: " + to_string(calculateQuartile1(buffer)));
    fiveNumbers.push_back(q1Value);

    vector<string> medValue;
    medValue.push_back("med: " + to_string(calculateMedian(buffer)));
    fiveNumbers.push_back(medValue);

    vector<string> q3Value;
    q3Value.push_back("Q3: " + to_string(calculateQuartile3(buffer)));
    fiveNumbers.push_back(q3Value);

    vector<string> maximumValue;
    maximumValue.push_back("max: " + to_string(buffer[n - 1]));
    fiveNumbers.push_back(maximumValue);

    writeCSV("five_number_output.csv", fiveNumbers);
    cout << "Five numbers calculated successfully." << endl;

    return 0;
}

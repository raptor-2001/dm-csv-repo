#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <climits>
#include <cmath>

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

// Function for Min-Max normalization
void minMaxNormalization()
{
    // Find the minimum and maximum values in the data
    const vector<vector<string>> data = readCSV("exp2_input_MinMax.csv");
    double min_value;
    double max_value;

    double min_data = INT_MAX;
    double max_data = INT_MIN;

    cout << "Enter min and max values: ";
    cin >> min_value >> max_value;

    // Calculate the minimum and maximum values in the data
    for (const vector<string> &row : data)
    {
        for (const string &cell : row)
        {
            double value = stod(cell);
            if (value < min_data)
            {
                min_data = value;
            }
            if (value > max_data)
            {
                max_data = value;
            }
        }
    }

    // Perform Min-Max normalization
    vector<vector<string>> normalized_data = data;

    for (vector<string> &row : normalized_data)
    {
        for (string &cell : row)
        {
            double value = stod(cell);
            double normalized_value = (value - min_data) / (max_data - min_data) * (max_value - min_value) + min_value;
            cell = to_string(normalized_value);
        }
    }

    // Write the normalized data to a file
    writeCSV("exp2_output_MinMax.csv", normalized_data);

    cout << "\n\nNormalization complete successfully.\n\n"
         << endl;
}

// Function for Z-score normalization
void zScoreNormalization()
{
    // Read the input data from a file
    const vector<vector<string>> data = readCSV("exp2_input_Zscore.csv");

    int size = data[0].size();
    vector<double> mean(size, 0.0);
    vector<double> stdDev(size, 0.0);

    // Calculate the mean for each column
    for (const vector<string> &row : data)
    {
        for (size_t i = 0; i < row.size(); ++i)
        {
            double value = stod(row[i]);
            mean[i] += value;
        }
    }

    for (double &m : mean)
    {
        m /= data.size();
    }

    // Calculate the standard deviation for each column
    for (const vector<string> &row : data)
    {
        for (size_t i = 0; i < row.size(); ++i)
        {
            double value = stod(row[i]);
            stdDev[i] += (value - mean[i]) * (value - mean[i]);
        }
    }

    for (double &sd : stdDev)
    {
        sd = sqrt(sd / data.size());
    }

    // Perform Z-score normalization
    vector<vector<string>> normalized_data = data;

    for (vector<string> &row : normalized_data)
    {
        for (size_t i = 0; i < row.size(); ++i)
        {
            double value = stod(row[i]);
            double normalized_value = (value - mean[i]) / stdDev[i];
            row[i] = to_string(normalized_value);
        }
    }

    // Write the normalized data to a file
    writeCSV("exp2_output_Zscore.csv", normalized_data);

    cout << "\n\nZ-score normalization complete.\n\n"
         << endl;
}

int main()
{
    int n;

    while (true)
    {
        cout << "Enter 1 for Min-Max normalization" << endl;
        cout << "Enter 2 for Z-score normalization" << endl;
        cout << "Enter any other value to exit" << endl;
        cin >> n;

        if (n == 1)
        {
            minMaxNormalization();
        }
        else if (n == 2)
        {
            zScoreNormalization();
        }
        else
        {
            cout << "Invalid option. Exiting." << endl;
            break;
        }
    }

    return 0;
}

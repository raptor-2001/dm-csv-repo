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

void equiwidthBinning()
{
  const vector<vector<string>> data = readCSV("input.csv");
  vector<string> row = data[0]; // Extract the single row of data

  int num_bins;
  double min_val = INT_MAX;
  double max_val = INT_MIN;

  cout << "Enter the number of bins: ";
  cin >> num_bins;

  // Find the minimum and maximum values in the data
  for (const string &cell : row)
  {
    double value = stod(cell);
    if (value < min_val)
    {
      min_val = value;
    }
    if (value > max_val)
    {
      max_val = value;
    }
  }

  // Calculate the bin width
  double bin_width = (max_val - min_val) / num_bins;
  double min1 = min_val;

  vector<double> bin_edges;

  for (int i = 0; i < num_bins + 1; i++)
  {
    bin_edges.push_back(min1 + i * bin_width);
  }

  vector<vector<string>> new_binned_data;

  // Binning the data
  for (int i = 0; i < num_bins; i++)
  {
    vector<string> temp;
    for (const string &j : row)
    {
      double ele = stod(j);
      if (ele >= bin_edges[i] && ele <= bin_edges[i + 1])
      {
        temp.push_back(j);
      }
    }
    new_binned_data.push_back(temp);
  }

  // Write the binned data to a file
  writeCSV("output_equiwidth.csv", new_binned_data);

  cout << "Equiwidth binning complete." << endl;
}

void equiFrequencyBinning()
{
  const vector<vector<string>> data = readCSV("input.csv");
  vector<string> row = data[0]; // Extract the single row of data

  int num_bins;
  cout << "Enter the number of bins: ";
  cin >> num_bins;

  // Sort the data in ascending order
  vector<double> sorted_data;
  for (const string &cell : row)
  {
    sorted_data.push_back(stod(cell));
  }
  sort(sorted_data.begin(), sorted_data.end());

  int data_size = sorted_data.size();
  int items_per_bin = data_size / num_bins;
  int remaining_items = data_size % num_bins;

  vector<vector<string>> new_binned_data;

  // Binning the data
  int start_index = 0;
  for (int i = 0; i < num_bins; i++)
  {
    vector<string> temp;
    int items_in_bin = items_per_bin;

    // Distribute any remaining items to the first bins
    if (remaining_items > 0)
    {
      items_in_bin++;
      remaining_items--;
    }

    for (int j = 0; j < items_in_bin; j++)
    {
      temp.push_back(to_string(sorted_data[start_index + j]));
    }
    start_index += items_in_bin;

    new_binned_data.push_back(temp);
  }

  // Write the binned data to a file
  writeCSV("output_equifreq.csv", new_binned_data);

  cout << "Equi-frequency binning complete." << endl;
}

int main()
{
  int n;

  while (true)
  {
    cout << "Enter 1 for equi width" << endl;
    cout << "Enter 2 for equi frequency" << endl;
    cout << "Enter any other value to exit" << endl;
    cin >> n;

    if (n == 1)
    {
      equiwidthBinning();
    }
    else if (n == 2)
    {
      equiFrequencyBinning();
    }
    else
    {
      cout << "Invalid option. Exiting." << endl;
      break;
    }
  }
}
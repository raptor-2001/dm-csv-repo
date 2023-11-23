#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <climits>
#include <cmath>
#include <algorithm>
#include <map>

using namespace std;

int output_count = 1;

ofstream fwtr("linkage_output.csv", ios::out);

int readCSV(string input_streamname)
{

    fstream input_stream1(input_streamname, ios::in);

    string line;
    getline(input_stream1, line);

    int line_index = 0;
    stringstream st(line);

    int j = 0, len = 0;
    string point;

    // Determine the number of points in the distance_matrixset
    while (getline(st, point, ','))
    {
        if (j == 0)
        {
            j++;
            continue;
        }
        len++;
    }

    return len;
}

void writeCSV(string input_streamname, map<string, map<string, int>> distance_matrix, string up, string down)
{
    ofstream fw(input_streamname, ios::out);
    fw << ",";

    // fw << "\n";

    for (auto p : distance_matrix)
    {
        fw << p.first << ",";
    }
    fw << "\n";

    for (auto p : distance_matrix)
    {
        fw << p.first << ",";
        for (auto pp : p.second)
        {
            fw << pp.second << ",";
        }
        fw << "\n";
    }

    fw.close();
    fwtr << down << " & " << up << "\n";
}

// Function to perform agglomerative clustering and return the name of the resulting cluster
string agglomerativeClustering(string input)
{
    map<string, map<string, int>> distance_matrix;

    fstream input_stream(input, ios::in);

    string line;
    getline(input_stream, line);

    int line_index = 0;
    stringstream st(line);

    int i = 0;
    string point;
    vector<string> points;

    // Read the point names from the first line of the input file
    while (getline(st, point, ','))
    {
        if (i == 0)
        {
            i++;
            continue;
        }
        points.push_back(point);
    }

    // Populate the distance matrix from the input input_stream
    while (getline(input_stream, line))
    {
        stringstream str(line);
        getline(str, point, ',');

        string dist;
        int idx = 0;

        while (getline(str, dist, ','))
        {
            if (dist.length() != 0)
                distance_matrix[point][points[idx]] = stoi(dist);

            idx++;
        }
    }

    string line_index1, line_index2;
    int min_dist = INT_MAX;

    // Find the two points with the minimum distance
    for (auto p : distance_matrix)
    {
        for (auto pp : p.second)
        {
            string p1 = p.first, p2 = pp.first;
            int dist = pp.second;

            if (p1 != p2 && dist < min_dist)
            {
                line_index1 = p1;
                line_index2 = p2;
                min_dist = dist;
            }
        }
    }

    string up, down;

    // Determine the order of the two points based on their names
    if (line_index1[0] > line_index2[0])
    {
        up = line_index2;
        down = line_index1;
    }
    else
    {
        up = line_index1;
        down = line_index2;
    }

    string newline_index = down + up;

    // Update distances and remove old points from the matrix
    for (auto p : distance_matrix)
    {
        point = p.first;

        if (point[0] > newline_index[0])
        {
            distance_matrix[point][newline_index] = min(distance_matrix[point][up], distance_matrix[point][down]);
        }
    }

    for (auto p : distance_matrix[down])
    {
        point = p.first;
        int d1 = p.second;

        if (point[0] < up[0])
            d1 = min(d1, distance_matrix[up][point]);
        else
            d1 = min(d1, distance_matrix[point][up]);

        distance_matrix[newline_index][point] = d1;
    }

    for (auto p : distance_matrix)
    {
        point = p.first;
        auto mtemp = p.second;

        if (point[0] >= up[0])
        {
            int d1 = distance_matrix[point][up];

            if (down[0] > point[0])
                d1 = min(d1, distance_matrix[down][point]);
            else
                d1 = min(d1, distance_matrix[point][down]);

            distance_matrix[point][newline_index] = d1;
            distance_matrix[point].erase(up);

            if (point[0] >= down[0])
                distance_matrix[point].erase(down);
        }
    }

    distance_matrix.erase(up);
    distance_matrix.erase(down);

    // Create an output input_stream with updated cluster distance_matrix
    string output = "cluster_iteration_" + to_string(output_count++) + ".csv";

    // write in CSV

    writeCSV(output, distance_matrix, up, down);

    return output;
}

int main()
{
    string input = "linkage_input.csv";

    // read CSV
    int len = readCSV(input);

    // Repeatedly perform agglomerative clustering to create clusters
    for (int i = 1; i <= len - 2; i++)
    {
        string output = agglomerativeClustering(input);
        input = output;
    }

    cout << "\nAgglomerative clustering completed successfully";
    return 0;
}

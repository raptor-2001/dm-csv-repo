#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <string>
#include <climits>
using namespace std;

struct clusterSet
{
    string line;
    int mid_point;
    string point, x, y;
    int i = 0;
    int val1;
    int val2;
    vector<pair<int, int>> v;
    pair<int, int> p;
    int nearestPoint = 0;
    float x_new;
    float y_new;
    float nearestDistance = INT_MAX;
} cluster;

// Function to calculate the Euclidean distance between two points (x1, y1) and (x2, y2)
float distance(float x1, float y1, int x2, int y2)
{
    return sqrt(((float)x2 - x1) * ((float)x2 - x1) + ((float)y2 - y1) * ((float)y2 - y1));
}

void readCSV(string filename)
{
    // Open the input file for reading
    fstream in(filename, ios::in);

    // Check if the file was opened successfully
    if (!in.is_open())
    {
        // cout << "Error: Unable to open the input file 'cluster_input.csv'." << endl;
        return;
    }

    // Read data from the CSV file line by line
    while (getline(in, cluster.line))
    {
        stringstream str(cluster.line);

        if (cluster.i == 0)
        {
            cluster.i++;
            continue; // Skip the first line (possibly a header).
        }

        // Parse the CSV line into variables point, x, and y
        getline(str, cluster.point, ',');
        getline(str, cluster.x, ',');
        getline(str, cluster.y, ',');

        cluster.val1 = stoi(cluster.x); // Convert x to an integer
        cluster.val2 = stoi(cluster.y); // Convert y to an integer

        cluster.v.push_back({cluster.val1, cluster.val2}); // Add the pair (val1, val2) to the vector v
    }
}

// Open the output file for writing
ofstream out("cluster_output.csv");

vector<float> generateDistanceMatrix()
{
    int n = cluster.v.size();
    int x_sum = 0, y_sum = 0;

    // Calculate the sum of x and y coordinates
    for (int i = 0; i < cluster.v.size(); i++)
    {
        int first = cluster.v[i].first;
        int second = cluster.v[i].second;

        x_sum += first;
        y_sum += second;
    }

    // Calculate the coordinates of the midpoint (average)
    float mid_x = (float)x_sum / n;
    float mid_y = (float)y_sum / n;
    // cout << "Midpoint of the data: (" << mid_x << "," << mid_y << ")" << endl;

    // Write the header for the output CSV file
    out << " , x1, x2, x3, x4, I";
    out << "\n";

    // Calculate the distances between all pairs of points
    for (int i = 0; i < cluster.v.size(); i++)
    {
        if (i < cluster.v.size())
            out << "x" << i + 1 << ",";

        for (int j = 0; j <= i; j++)
        {
            int f_x1 = cluster.v[i].first;
            int s_y1 = cluster.v[i].second;
            int f_x2 = cluster.v[j].first;
            int s_y2 = cluster.v[j].second;

            if (f_x1 == f_x2 && s_y1 == s_y2)
            {
                out << "0"
                    << ",";
                break;
            }
            float dis = distance(f_x1, s_y1, f_x2, s_y2);

            out << dis << ",";
        }
        out << "\n";
    }

    out << "I"
        << ",";

    vector<float> midvalue = {mid_x, mid_y};
    return midvalue;
}

void calculateImaginaryCenter(vector<float> midvalue)
{
    // Calculate the distances of each point from the calculated midpoint
    for (int i = 0; i < cluster.v.size(); i++)
    {
        int first = cluster.v[i].first;
        int second = cluster.v[i].second;
        float mid_x = midvalue[0];
        float mid_y = midvalue[1];

        float d = distance(mid_x, mid_y, first, second);

        // cout << "Distance of p" << i + 1 << " from the center: " << d << " units." << endl;

        if (cluster.nearestDistance > d)
        {
            cluster.nearestDistance = d;
            cluster.nearestPoint = i + 1;
            cluster.x_new = first;
            cluster.y_new = second;
        }

        out << d << ",";

        if (i == cluster.v.size() - 1)
            out << "0"
                << ",";
    }

    // cout << "The nearest distance from the center is: " << cluster.nearestDistance << " units." << endl;
    // cout << "The nearest point from the center is: "
    // << "p" << cluster.nearestPoint << endl;
    out << ",";
    out << "\n";
}

void updateDistanceMatrix()
{
    // New Center Calculation
    out << " , x1, x2, x2, x3";
    out << "\n";

    for (int i = 0; i < cluster.v.size(); i++)
    {
        if (i < cluster.v.size())
            out << "x" << i + 1 << ",";

        for (int j = 0; j <= i; j++)
        {
            int f_x1 = cluster.v[i].first;
            int s_y1 = cluster.v[i].second;
            int f_x2 = cluster.v[j].first;
            int s_y2 = cluster.v[j].second;

            if (f_x1 == f_x2 && s_y1 == s_y2)
            {
                out << "0"
                    << ",";
                break;
            }
            float dis = distance(f_x1, s_y1, f_x2, s_y2);

            out << dis << ",";
        }
        out << "\n";
    }

    out << "x" << cluster.nearestPoint << " (New Center)"
        << ",";

    // Calculate the distances of each point from the new center
    for (int i = 0; i < cluster.v.size(); i++)
    {
        int first = cluster.v[i].first;
        int second = cluster.v[i].second;

        float d = distance(cluster.x_new, cluster.y_new, first, second);

        // cout << "Distance of p" << i + 1 << " from the new center (p" << cluster.nearestPoint << "): " << d << " units." << endl;

        out << d << ",";

        if (i == cluster.v.size() - 1)
            out << "0"
                << ",";
    }
}

int main()
{

    // Read the data from CSV
    readCSV("cluster_input.csv");

    // Generate the distance matrix and mid values
    vector<float> midvalue = generateDistanceMatrix();

    // Calculate the distance matrix
    calculateImaginaryCenter(midvalue);

    // update distance matrix
    updateDistanceMatrix();

    cout << "Clustering done successfully\n";
}
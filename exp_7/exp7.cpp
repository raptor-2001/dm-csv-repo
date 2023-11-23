#include <bits/stdc++.h>
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

struct itemSet
{
    ifstream fin;
    double minfre;
    vector<set<string>> datatable;
    set<string> products;
    map<string, int> freq;
} item;

vector<string> wordsof(string str)
{
    vector<string> tmpset;
    string tmp = "";
    int i = 0;
    while (str[i])
    {
        if (isalnum(str[i]))
            tmp += str[i];
        else
        {
            if (tmp.size() > 0)
                tmpset.push_back(tmp);
            tmp = "";
        }
        i++;
    }
    if (tmp.size() > 0)
        tmpset.push_back(tmp);
    return tmpset;
}

string combine(vector<string> &arr, int miss)
{
    string str;
    for (int i = 0; i < arr.size(); i++)
        if (i != miss)
            str += arr[i] + " ";
    str = str.substr(0, str.size() - 1);
    return str;
}

set<string> cloneit(set<string> &arr)
{
    set<string> dup;
    for (set<string>::iterator it = arr.begin(); it != arr.end(); it++)
        dup.insert(*it);
    return dup;
}

set<string> apriori_gen(set<string> &sets, int k)
{
    set<string> set2;
    for (set<string>::iterator it1 = sets.begin(); it1 != sets.end(); it1++)
    {
        set<string>::iterator it2 = it1;
        it2++;
        for (; it2 != sets.end(); it2++)
        {
            vector<string> v1 = wordsof(*it1);
            vector<string> v2 = wordsof(*it2);

            bool alleq = true;
            for (int i = 0; i < k - 1 && alleq; i++)
                if (v1[i] != v2[i])
                    alleq = false;
            if (!alleq)
                continue;

            v1.push_back(v2[k - 1]);
            if (v1[v1.size() - 1] < v1[v1.size() - 2])
                swap(v1[v1.size() - 1], v1[v1.size() - 2]);

            for (int i = 0; i < v1.size() && alleq; i++)
            {
                string tmp = combine(v1, i);
                if (sets.find(tmp) == sets.end())
                    alleq = false;
            }

            if (alleq)
                set2.insert(combine(v1, -1));
        }
    }
    return set2;
}

void readCSV()
{
    item.fin.open("item_set_input.csv");
    cout << "Frequency % :";
    cin >> item.minfre;
    string str;
    while (!item.fin.eof())
    {
        getline(item.fin, str);
        vector<string> arr = wordsof(str); // taking data from file ,
        set<string> tmpset;
        for (int i = 0; i < arr.size(); i++)
            tmpset.insert(arr[i]);
        item.datatable.push_back(tmpset);
        for (set<string>::iterator it = tmpset.begin(); it != tmpset.end(); it++)
        {
            item.products.insert(*it);
            item.freq[*it]++;
        }
    }
    item.fin.close();
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

queue<set<string>::iterator> generateItemset()
{
    // read the CSV data;
    readCSV();

    cout << "No of transactions: " << item.datatable.size() << endl;
    item.minfre = item.minfre * item.datatable.size() / 100;
    cout << "Min frequency:" << item.minfre << endl;
    queue<set<string>::iterator> q;

    for (set<string>::iterator it = item.products.begin(); it != item.products.end(); it++)
        if (item.freq[*it] < item.minfre)
            q.push(it);
    while (q.size() > 0)
    {
        item.products.erase(*q.front());
        q.pop();
    }

    for (set<string>::iterator it = item.products.begin(); it != item.products.end(); it++)
    {
        cout << *it << " " << item.freq[*it] << endl;
    }
    return q;
}

void calculateItemset(queue<set<string>::iterator> &q)
{

    vector<vector<string>> output;
    vector<string> temp;

    int i = 2;
    set<string> prev = cloneit(item.products);
    while (i)
    {
        set<string> cur = apriori_gen(prev, i - 1);
        if (cur.size() < 1)
            break;
        for (set<string>::iterator it = cur.begin(); it != cur.end(); it++)
        {

            vector<string> arr = wordsof(*it);
            int tot = 0;

            for (int j = 0; j < item.datatable.size(); j++)
            {
                bool pres = true;
                for (int k = 0; k < arr.size() && pres; k++)
                    if (item.datatable[j].find(arr[k]) == item.datatable[j].end())
                        pres = false;
                if (pres)
                    tot++;
            }

            if (tot >= item.minfre)
                item.freq[*it] += tot;
            else
                q.push(it);
        }
        while (q.size() > 0)
        {
            cur.erase(*q.front());
            q.pop();
        }
        string word;
        for (set<string>::iterator it = cur.begin(); it != cur.end(); it++)
        {
            cout << *it << " " << item.freq[*it] << endl;
            word = *it + " " + to_string(item.freq[*it]);
        }
        temp.push_back(word);

        prev = cloneit(cur);
        i++;
    }
    output.push_back(temp);

    writeCSV("item_set_output.csv", output);
}

int main()
{
    queue<set<string>::iterator> q = generateItemset();

    calculateItemset(q);
}
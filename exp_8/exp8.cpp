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
    int pass;
    ifstream fin;
    double minfre;
    vector<set<string>> datatable;
    set<string> products;
    map<string, int> freq;
    double confidence;
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

// Count occurrences of a set of items in the dataset
int countOccurences(vector<string> v)
{
    int count = 0;

    for (auto s : item.datatable)
    {
        bool present = true;

        for (auto x : v)
        {
            if (s.find(x) == s.end())
            {
                present = false;
                break;
            }
        }

        if (present)
            count++;
    }

    return count;
}

// Generate subsets of items for association rule generation
ofstream fw1("association_output.csv", ios::out);
void subsets(vector<string> items, vector<string> v1, vector<string> v2, int idx)
{
    if (idx == items.size())
    {
        if (v1.size() == 0 || v2.size() == 0)
            return;

        int count1 = countOccurences(items); // Total support
        int count2 = countOccurences(v1);

        double conf = (((double)count1) / count2) * 100;

        if (conf >= item.confidence)
        {
            fw1 << "Association Rule: { ";
            for (auto s : v1)
            {
                fw1 << s << " ";
            }
            fw1 << "} -> {";
            for (auto s : v2)
            {
                fw1 << s << " ";
            }
            fw1 << "} , Confidence: " << conf << "%" << endl;
        }

        return;
    }

    v1.push_back(items[idx]);
    subsets(items, v1, v2, idx + 1);

    v1.pop_back();
    v2.push_back(items[idx]);
    subsets(items, v1, v2, idx + 1);
    v2.pop_back();
}

// Generate association rules from frequent itemsets
void generateAssociationRules(set<string> freqItems)
{
    for (auto it = freqItems.begin(); it != freqItems.end(); it++)
    {
        vector<string> items = wordsof(*it);

        subsets(items, {}, {}, 0);
    }
}

void readCSV()
{
    item.fin.open("association_input.csv", ios::in);

    if (!item.fin.is_open())
    {
        cerr << "Error in opening file." << endl;
        return;
    }

    cout << "Enter Minimum Support (%): ";
    cin >> item.minfre;

    cout << "Enter Minimum Confidence (%): ";
    cin >> item.confidence;

    string str;
    while (!item.fin.eof())
    {
        getline(item.fin, str);
        vector<string> arr = wordsof(str);
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

    cout << "Number of transactions: " << item.datatable.size() << endl;
    item.minfre = item.minfre * item.datatable.size() / 100;
    cout << "Minimum Frequency Threshold: " << item.minfre << endl;

    queue<set<string>::iterator> q;
    for (set<string>::iterator it = item.products.begin(); it != item.products.end(); it++)
        if (item.freq[*it] < item.minfre)
            q.push(it);

    while (q.size() > 0)
    {
        item.products.erase(*q.front());
        q.pop();
    }

    cout << "Frequent " << item.pass++ << "-item set: " << endl;
    for (set<string>::iterator it = item.products.begin(); it != item.products.end(); it++)
        cout << "{" << *it << "} - Support: " << item.freq[*it] << endl;

    return q;
}

void calculateItemset(queue<set<string>::iterator> &q)
{

    // vector<vector<string>> output;
    // vector<string> temp;

    int i = 2;
    set<string> prev = cloneit(item.products);

    while (i)
    {
        set<string> cur = apriori_gen(prev, i - 1);

        if (cur.size() < 1)
        {
            break;
        }

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

        bool flag = true;

        for (set<string>::iterator it = cur.begin(); it != cur.end(); it++)
        {
            vector<string> arr = wordsof(*it);

            if (item.freq[*it] < item.minfre)
                flag = false;
        }

        if (cur.size() == 0)
            break;
        // string dummy = "";
        cout << "\nFrequent " << item.pass++ << "-item set: " << endl;

        // dummy = "\nFrequent " + to_string(item.pass) + " -item set: ";
        // temp.push_back(dummy);

        for (set<string>::iterator it = cur.begin(); it != cur.end(); it++)
        {
            // dummy = "{" + *it + "} - Support: " + to_string(item.freq[*it]);
            cout << "{" << *it << "} - Support: " << item.freq[*it] << endl;
            // temp.push_back(dummy);
        }

        prev = cloneit(cur);
        i++;
    }
    // output.push_back(temp);

    generateAssociationRules(prev);

    // writeCSV("association_output.csv", output);
}

int main()
{
    item.pass = 1;
    queue<set<string>::iterator> q = generateItemset();

    calculateItemset(q);

    cout << "Association rules generated successfully." << endl;
}
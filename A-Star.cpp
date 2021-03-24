#include <iostream>
#include <vector>    //Vectors Structure
#include <algorithm> //Heap Structure
#include <string>    //String Structure
#include "Heuristic_1.h"
#include "Heuristic_2.h"
#include "Heuristic_3.h"
#include "Heuristic_4.h"
#include "Heuristic_5.h"
#define NIL -1
#define inf 2147483647

using namespace std;

struct vertex
{
    int name;
    vector<int> state;
    int parent;
    int g_cost;
    int h_cost;
    int f_cost;
};

struct A_Star
{
    vector<int> S;             // Entry
    vector<vertex> A;          // Open set
    vector<vertex> F;          // Closed set
    int currentStateIndex = 0; // Name index
    vector<int> T = {1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15, 4, 8, 12, 0};
};

int findBlank(vector<int> state)
{
    for (int i = 0; i < 16; i++)
    {
        if (state.at(i) == 0)
            return i;
    }
    return NIL;
}

vertex blankSwap(vector<int> state, int i, int j)
{
    vertex ret;
    int aux;
    aux = state.at(i);
    state.at(i) = state.at(j);
    state.at(j) = aux;
    ret.state = state;
    return ret;
}

vector<vertex> generateSuccessors(A_Star &tree, vertex parent)
{
    int blankPosition = findBlank(parent.state) + 1;
    vector<vertex> children;

    if (blankPosition == 0)
    {
        children.push_back(blankSwap(parent.state, blankPosition, blankPosition + 4));
        children.push_back(blankSwap(parent.state, blankPosition, blankPosition + 1));
    }

    else if (blankPosition == 3)
    {
        children.push_back(blankSwap(parent.state, blankPosition, blankPosition + 4));
        children.push_back(blankSwap(parent.state, blankPosition, blankPosition - 1));
    }

    else if (blankPosition == 12)
    {
        children.push_back(blankSwap(parent.state, blankPosition, blankPosition - 4));
        children.push_back(blankSwap(parent.state, blankPosition, blankPosition + 1));
    }

    else if (blankPosition == 15)
    {
        children.push_back(blankSwap(parent.state, blankPosition, blankPosition - 4));
        children.push_back(blankSwap(parent.state, blankPosition, blankPosition - 1));
    }

    else if (blankPosition == 1 || blankPosition == 2)
    {
        children.push_back(blankSwap(parent.state, blankPosition, blankPosition + 1));
        children.push_back(blankSwap(parent.state, blankPosition, blankPosition - 1));
        children.push_back(blankSwap(parent.state, blankPosition, blankPosition + 4));
    }

    else if (blankPosition == 4 || blankPosition == 8)
    {
        children.push_back(blankSwap(parent.state, blankPosition, blankPosition + 1));
        children.push_back(blankSwap(parent.state, blankPosition, blankPosition - 4));
        children.push_back(blankSwap(parent.state, blankPosition, blankPosition + 4));
    }

    else if (blankPosition == 13 || blankPosition == 14)
    {
        children.push_back(blankSwap(parent.state, blankPosition, blankPosition + 1));
        children.push_back(blankSwap(parent.state, blankPosition, blankPosition - 1));
        children.push_back(blankSwap(parent.state, blankPosition, blankPosition - 4));
    }

    else if (blankPosition == 7 || blankPosition == 11)
    {
        children.push_back(blankSwap(parent.state, blankPosition, blankPosition - 1));
        children.push_back(blankSwap(parent.state, blankPosition, blankPosition - 4));
        children.push_back(blankSwap(parent.state, blankPosition, blankPosition + 4));
    }

    else
    {
        children.push_back(blankSwap(parent.state, blankPosition, blankPosition - 1));
        children.push_back(blankSwap(parent.state, blankPosition, blankPosition + 1));
        children.push_back(blankSwap(parent.state, blankPosition, blankPosition - 4));
        children.push_back(blankSwap(parent.state, blankPosition, blankPosition + 4));
    }

    for (vertex v : children)
    {
        v.name = tree.currentStateIndex++;
        v.parent = parent.name;
    }
    return children;
}

int A_Star_Algorithm(A_Star *tree, vector<int> entry)
{
    vertex root;
    root.g_cost = 0;
    root.name = tree->currentStateIndex++;
    root.parent = NIL;
    root.state = entry;
    tree->S = entry;
    tree->A.push_back(root);
}

vector<int> split(string entry, char separator)
{
    size_t start = 0, end = 0;
    vector<int> result;

    while ((end = entry.find(separator, start)) != string::npos)
    {
        result.push_back((stoi(entry.substr(start, end - start), nullptr)));
        start = end + 1;
    }
    result.push_back((stoi(entry.substr(start), nullptr)));
    return result;
}

int main()
{
    A_Star tree;
    string in;
    getline(cin, in);
    while (in.at(0) == ' ')
        in.erase(0, 1);
    vector<int> entry = split(in, ' ');
    A_Star_Algorithm(&tree,entry);
}
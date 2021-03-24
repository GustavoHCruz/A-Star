#include <iostream>
#include <vector>    //Vectors Structure
#include <algorithm> //Heap Structure
#include <string>    //String Structure
#define NIL -1
#define inf 2147483647

using namespace std;

#include "Heuristics.h"

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
        if (state.at(i) == 0)
            return i;

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

void generateSuccessors(A_Star *tree,vertex parent)
{
    int blankPosition = findBlank(parent.state);
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
        v.name = tree->currentStateIndex++;
        v.parent = parent.name;
        tree->A.push_back(v);
    }
}

bool comp(vertex x, vertex y)
{
    return x.f_cost < y.f_cost;
}

void initializeTree(A_Star *tree, vector<int> entry)
{
    vertex root;
    root.g_cost = 0;
    root.h_cost = heuristic_1(entry, tree->T);
    root.f_cost = root.g_cost + root.h_cost;
    root.name = tree->currentStateIndex++;
    root.parent = NIL;
    root.state = entry;
    tree->S = entry;
    tree->A.push_back(root);
    make_heap(tree->A.begin(), tree->A.end(), comp);
}

int A_Star_Algorithm(A_Star *tree, vector<int> entry)
{
    bool end = false;

    initializeTree(&(*tree), entry);

    while (!end)
    {
        sort_heap(tree->A.begin(), tree->A.end(), comp);
        vertex parent = tree->A.front();
        generateSuccessors(&(*tree),parent);
    }
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
    A_Star_Algorithm(&tree, entry);
}
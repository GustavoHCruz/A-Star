#include <iostream>
#include <random>
#include <vector>
#include <queue>
#include <stack>
#include <assert.h>
#include <algorithm>
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
    vector<vertex> StatesTree; // 1x1 index map here?
    vector<int> A;             // Heap here?
    vector<int> F;             // Heap here?
    int currentStateIndex = 0; // index to map
    vector<int> answer = {1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15, 4, 8, 12, 0};
};

int findBlank(vector<int> state)
{
    for (int i = 0; i < 16; i++)
    {
        if (state.at(i) == 0)
            return i;
    }
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

void generateSuccessors(A_Star &tree, vertex parent)
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

    else if (blankPosition == 5 || blankPosition == 9 || blankPosition == 6 || blankPosition == 10)
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
}

int A_Star_Algorithm()
{
    cout << "Maybe Tomorrow";
}

int main()
{
    A_Star tree;
    cout << tree.answer[0];
    A_Star_Algorithm();
}
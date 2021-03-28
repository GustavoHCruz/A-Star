#include <iostream>
#include <vector>    //Vectors Structure
#include <algorithm> //Heap Structure
#include <string>    //String Structure
#define NIL -1
#define heuristic_used 1

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

int heuristic_1(vector<int> state, vector<int> answer)
{
    int counter = 0;
    int i;
    for (i = 0; i < 16; i++)
        if (state.at(i) != answer.at(i)){
            counter++;
        }

    return counter;
}

int heuristics(int h, vector<int> v, vector<int> answer)
{
    if (h == 1)
        return heuristic_1(v, answer);
    return NIL;
}

int findBlank(vector<int> state)
{
    for (size_t i = 0; i < 16; i++)
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

vector<vertex> generateSuccessors(A_Star *tree, vertex parent)
{
    size_t blankPosition = findBlank(parent.state);
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

    return children;
}

bool comp(vertex x, vertex y)
{
    return x.f_cost < y.f_cost;
}

void initializeTree(A_Star *tree, vector<int> entry)
{
    vertex root;
    root.g_cost = 0;
    root.h_cost = heuristics(heuristic_used, entry, tree->T);
    root.f_cost = root.g_cost + root.h_cost;
    root.name = tree->currentStateIndex++;
    root.parent = NIL;
    root.state = entry;
    tree->S = entry;
    tree->A.push_back(root);
    make_heap(tree->A.begin(), tree->A.end(), comp);
    make_heap(tree->F.begin(), tree->F.end(), comp);
}

int find_m(A_Star *tree, vertex m)
{
    int i = 0;
    for (vertex v : tree->A)
    {
        if (m.state == v.state)
            return i;
        i++;
    }
    return NIL;
}

void find_duplicated(A_Star *tree, vertex m)
{
    int i = 0;
    for (vertex v : tree->A)
    {
        if (m.state == v.state)
            if (m.g_cost < v.g_cost)
                tree->A.erase(tree->A.begin() + i);

        i++;
    }
}

int A_Star_Algorithm(A_Star *tree, vector<int> entry)
{
    int answer = 0;
    vector<vertex> children;
    bool end = false;

    initializeTree(&(*tree), entry);

    while (!end)
    {
        sort_heap(tree->A.begin(), tree->A.end(), comp);
        sort_heap(tree->F.begin(), tree->F.end(), comp);

        vertex parent = tree->A.front();
        tree->A.erase(tree->A.begin());
        tree->F.push_back(parent);

        children = generateSuccessors(&(*tree), parent);

        for (vertex m : children)
        {
            m.g_cost = parent.g_cost + 1;

            find_duplicated(&(*tree), m);

            if (find_m(&(*tree), m) == NIL)
            {
                m.parent = parent.name;
                m.h_cost = heuristics(heuristic_used, m.state, tree->T);
                m.f_cost = m.g_cost + m.h_cost;
                tree->A.push_back(m);
            }
        }

        for (vertex v : tree->A)
        {
            if(v.state == tree->T){
                end = true;
                answer = v.g_cost;
            }
        }
    }

    return answer;
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
    cout << A_Star_Algorithm(&tree, entry);
}
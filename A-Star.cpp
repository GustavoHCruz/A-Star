#include <iostream>  // I/O Library
#include <vector>    // Vectors Structure
#include <algorithm> // Heap Structure
#include <string>    // String Structure
#include <assert.h>  // Assertion Library
#include <map>       // Map Library

#define NIL -1
#define heuristic_used 3 // Determines the heuristic to be used

using namespace std;

struct vertex
{
    int name;
    vector<short> state;
    int parent;
    int g_cost;
    int h_cost;
    int f_cost;
};

struct A_Star
{
    vector<short> S;                                                          // Entry
    vector<vertex> A;                                                         // Open set
    vector<vertex> F;                                                         // Closed set
    int currentStateIndex = 0;                                                // Name index
    vector<short> T = {1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15, 4, 8, 12, 0}; // Final state set
};

// Number of out-of-order pieces
int heuristic_1(vector<short> state)
{
    int counter = 0;
    for (int i = 0; i < 15; i++)
        if ((state.at(i) - 1) != (i * 4) % 15)
            counter++;

    return counter;
}

// Number of pieces out of numerical order
int heuristic_2(vector<short> state)
{
    int counter = 0, comp;
    int number = state.at(0), i;
    for (i = 1; i < 15; i++)
    {
        comp = state.at((i * 4) % 15);
        if ((number + 1) != comp)
            counter++;
        number = state.at((i * 4) % 15);
    }
    comp = state.at((i * 4) % 15);
    if ((number + 1) % 15 != comp)
        counter++;

    return counter;
}

// Manhattan distance for each piece out of place
int heuristic_3(vector<short> state)
{
    int counter = 0, manhattamDistance, j, correctPosition;
    for (size_t i = 0; i < 15; i++)
    {
        manhattamDistance = 0;
        if (state.at(i) != 0)
        {
            correctPosition = ((state.at(i) - 1) * 4) % 15;
            j = i;
            while (correctPosition != j)
            {
                if (j < correctPosition)
                {
                    if ((j + 4) > correctPosition)
                        j++;
                    else
                        j = j + 4;
                }
                else
                {
                    if ((j - 4) < correctPosition)
                        j--;
                    else
                        j = j - 4;
                }
                manhattamDistance++;
            }
        }
        counter += manhattamDistance;
    }
    return counter;
}

// Weighted value of heuristics 1, 2 and 3
int heuristic_4(vector<short> v)
{
    float w1 = 0.15, w2 = 0.05, w3 = 0.80;
    return (heuristic_1(v) * w1 + heuristic_2(v) * w2 + heuristic_3(v) * w3);
}

// Maximum value between heuristics 1, 2 and 3
int heuristic_5(vector<short> v)
{
    return max({heuristic_1(v), heuristic_2(v), heuristic_3(v)});
}

// Determines which heuristic will be used
int heuristics(int h, vector<short> v)
{
    if (h == 1)
        return heuristic_1(v);
    else if (h == 2)
        return heuristic_2(v);
    else if (h == 3)
        return heuristic_3(v);
    else if (h == 4)
        return heuristic_4(v);
    else if (h == 5)
        return heuristic_5(v);
    return NIL;
}

int findBlank(vector<short> state)
{
    for (int i = 0; i < 16; i++)
        if (state.at(i) == 0)
            return i;

    return NIL;
}

vertex blankSwap(vector<short> state, int i, int j)
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
    vector<vertex> children;
    int blankPosition = findBlank(parent.state), i = blankPosition % 4, j = blankPosition / 4;

    if (i < 3)
        children.push_back(blankSwap(parent.state, blankPosition, blankPosition + 1));

    if (i > 0)
        children.push_back(blankSwap(parent.state, blankPosition, blankPosition - 1));

    if (j < 3)
        children.push_back(blankSwap(parent.state, blankPosition, blankPosition + 4));

    if (j > 0)
        children.push_back(blankSwap(parent.state, blankPosition, blankPosition - 4));

    return children;
}

bool comp(vertex x, vertex y)
{
    return x.f_cost > y.f_cost;
}

void initializeTree(A_Star *tree, vector<short> entry, map<vector<short>, int> &treeMap)
{
    vertex root;
    root.g_cost = 0;
    root.h_cost = heuristics(heuristic_used, entry);
    root.f_cost = root.g_cost + root.h_cost;
    root.name = tree->currentStateIndex++;
    root.parent = NIL;
    root.state = entry;
    tree->S = entry;
    tree->A.push_back(root);
    make_heap(tree->A.begin(), tree->A.end(), comp);

    treeMap[root.state] = root.g_cost;
}

bool find_m(A_Star *tree, vertex m, map<vector<short>, int> &treeMap)
{
    if (treeMap.find(m.state) != treeMap.end())
        return true;
    return false;
}

void find_duplicated(A_Star *tree, vertex m, map<vector<short>, int> &treeMap)
{
    if (treeMap.find(m.state) != treeMap.end())
    {
        if (m.g_cost < treeMap[m.state])
            treeMap.erase(m.state);
    }
}

int A_Star_Algorithm(vector<short> entry)
{
    A_Star tree;
    map<vector<short>, int> treeMap; // Hash
    vertex parent;
    vector<vertex> children;

    initializeTree(&tree, entry, treeMap);

    while (true)
    {
        parent = tree.A.front();
        pop_heap(tree.A.begin(), tree.A.end(), comp);
        tree.A.pop_back();

        tree.F.push_back(parent);

        if (parent.state == tree.T)
            return parent.f_cost;

        children = generateSuccessors(&tree, parent);

        for (vertex m : children)
        {
            m.g_cost = parent.g_cost + 1;

            find_duplicated(&tree, m, treeMap);

            if (find_m(&tree, m, treeMap) == false)
            {
                m.parent = parent.name;
                m.name = tree.currentStateIndex++;
                m.h_cost = heuristics(heuristic_used, m.state);
                m.f_cost = m.g_cost + m.h_cost;
                tree.A.push_back(m);
                treeMap[m.state] = m.g_cost;
                push_heap(tree.A.begin(), tree.A.end(), comp);
            }
        }
    }
    return NIL;
}

vector<short> split(string entry, char separator)
{
    size_t start = 0, end = 0;
    vector<short> result;

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
    // Normal Execution
    /*
    string in;
    getline(cin, in);
    while (in.at(0) == ' ')
        in.erase(0, 1);
    vector<short> entry = split(in, ' ');
    cout << A_Star_Algorithm(entry);
    */

    // Tests
    /*
    assert(A_Star_Algorithm({1, 5, 9, 13, 2, 10, 7, 14, 3, 6, 11, 15, 4, 8, 12, 0}) == 8);  // (Run.Codes 1)
    assert(A_Star_Algorithm({1, 5, 9, 13, 3, 2, 10, 14, 6, 7, 11, 15, 4, 8, 12, 0}) == 10); // (Run.Codes 2)
    assert(A_Star_Algorithm({2, 1, 10, 9, 3, 5, 11, 13, 4, 6, 12, 14, 0, 7, 8, 15}) == 15); // (Run.Codes 3)
    assert(A_Star_Algorithm({0, 2, 1, 9, 3, 5, 6, 13, 4, 7, 10, 14, 8, 12, 15, 11}) == 18); // (Run.Codes 4)
    assert(A_Star_Algorithm({3, 2, 1, 5, 4, 7, 6, 10, 8, 11, 0, 9, 12, 15, 14, 13}) == 30); // (Classroom 1)
    assert(A_Star_Algorithm({0, 5, 9, 13, 2, 10, 15, 14, 1, 4, 3, 6, 8, 11, 12, 7}) == 30); // (Classroom 2)
    assert(A_Star_Algorithm({9, 0, 13, 10, 5, 2, 6, 14, 1, 11, 15, 12, 7, 3, 4, 8}) == 25); // (Classroom 3)
    assert(A_Star_Algorithm({15, 9, 0, 7, 13, 1, 11, 14, 5, 4, 10, 2, 3, 8, 6, 12}) == 46); // (Classroom 4)
    assert(A_Star_Algorithm({5, 13, 6, 10, 1, 7, 2, 9, 4, 3, 15, 14, 8, 0, 11, 12}) == 20); // (Extra 1)
    assert(A_Star_Algorithm({2, 10, 11, 9, 3, 1, 0, 13, 4, 6, 7, 14, 5, 8, 12, 15}) == 27); // (Extra 2)
    assert(A_Star_Algorithm({0, 2, 9, 13, 3, 1, 5, 14, 4, 7, 6, 10, 8, 11, 12, 15}) == 18); // (Final 1)
    assert(A_Star_Algorithm({3, 2, 1, 9, 0, 5, 6, 13, 4, 7, 10, 14, 8, 12, 15, 11}) == 19); // (Final 2)
    assert(A_Star_Algorithm({2, 1, 9, 13, 3, 5, 10, 14, 4, 6, 11, 15, 7, 8, 12, 0}) == 12); // (Final 3)
    assert(A_Star_Algorithm({9, 13, 10, 0, 5, 2, 6, 14, 1, 7, 11, 15, 3, 4, 8, 12}) == 21); // (Final 4)
    assert(A_Star_Algorithm({4, 3, 2, 1, 8, 10, 11, 5, 12, 6, 0, 9, 15, 7, 14, 13}) == 38); // (Final 5)
    assert(A_Star_Algorithm({9, 13, 14, 15, 5, 6, 10, 8, 0, 1, 11, 12, 7, 2, 3, 4}) == 32); // (Final 6)
    assert(A_Star_Algorithm({10, 6, 2, 1, 7, 13, 9, 5, 0, 15, 14, 12, 11, 3, 4, 8}) == 38); // (Final 7)
    assert(A_Star_Algorithm({6, 2, 1, 5, 4, 10, 13, 9, 0, 8, 3, 7, 12, 15, 11, 14}) == 32); // (Final 8)
    assert(A_Star_Algorithm({10, 13, 15, 0, 5, 9, 14, 11, 1, 2, 6, 7, 3, 4, 8, 12}) == 27); // (Final 9)
    assert(A_Star_Algorithm({5, 9, 13, 14, 1, 6, 7, 10, 11, 15, 12, 0, 8, 2, 3, 4}) == 29); // (Final 10)
    */
}
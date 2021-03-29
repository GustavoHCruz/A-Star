#include <iostream>  // I/O Library
#include <vector>    // Vectors Structure
#include <algorithm> // Heap Structure
#include <string>    // String Structure
#include <assert.h>  // Assertion Library

#define NIL -1
#define heuristic_used 1 // Determines the heuristic to be used

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
    vector<int> S;                                                          // Entry
    vector<vertex> A;                                                       // Open set
    vector<vertex> F;                                                       // Closed set
    int currentStateIndex = 0;                                              // Name index
    vector<int> T = {1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15, 4, 8, 12, 0}; // Final state set
};

// Number of out-of-order pieces
int heuristic_1(vector<int> state, vector<int> answer)
{
    int counter = 0;
    for (int i = 0; i < 16; i++)
    {
        if (state.at(i) != 0)
        {
            if (state.at(i) != answer.at(i))
            {
                counter++;
            }
        }
    }

    return counter;
}

// Number of pieces out of numerical order
int heuristic_2(vector<int> state)
{
    int counter = 0, comp;
    int number = state.at(0), i;
    for (i = 1; i < 15; i++)
    {
        comp = state.at((i * 4) % 15);
        if ((number + 1) != comp && number != 0)
            counter++;
        number = state.at((i * 4) % 15);
    }
    comp = state.at((i * 4) % 15);
    if ((number + 1) % 15 != comp)
        counter++;

    return counter;
}

// Manhattan distance for each piece out of place
int heuristic_3(vector<int> state)
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
int heuristic_4(vector<int> v, vector<int> answer)
{
    int w1 = 0, w2 = 0, w3 = 1;

    return (heuristic_1(v, answer) * w1 + heuristic_2(v) * w2 + heuristic_3(v) * w3);
}

// Maximum value between heuristics 1, 2 and 3
int heuristic_5(vector<int> v, vector<int> answer)
{
    return max({heuristic_1(v, answer), heuristic_2(v), heuristic_3(v)});
}

// Determines which heuristic will be used
int heuristics(int h, vector<int> v, vector<int> answer)
{
    if (h == 1)
        return heuristic_1(v, answer);
    else if (h == 2)
        return heuristic_2(v);
    else if (h == 3)
        return heuristic_3(v);
    else if (h == 4)
        return heuristic_4(v, answer);
    else if (h == 5)
        return heuristic_5(v, answer);
    return NIL;
}

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
}

int find_m(A_Star *tree, vertex m)
{
    for (vertex v : tree->A)
    {
        if (m.state == v.state)
            return true;
    }
    for (vertex v : tree->F)
    {
        if (m.state == v.state)
            return true;
    }
    return false;
}

void find_duplicated(A_Star *tree, vertex m)
{
    for (int i = 0; i < tree->A.size(); i++)
    {
        if (m.state == tree->A.at(i).state)
            if (m.g_cost < tree->A.at(i).g_cost)
            {
                tree->A.erase(tree->A.begin() + i);
                make_heap(tree->A.begin(), tree->A.end(), comp); // If removing a non-max element, heapfy must be done again
                break;
            }
    }
}

int A_Star_Algorithm(A_Star *tree, vector<int> entry)
{
    vertex parent;
    vector<vertex> children;

    initializeTree(&(*tree), entry);

    while (true)
    {
        parent = tree->A.front();
        pop_heap(tree->A.begin(), tree->A.end(), comp);
        tree->A.pop_back();

        tree->F.push_back(parent);

        if (parent.state == tree->T)
            return parent.f_cost;

        children = generateSuccessors(&(*tree), parent);

        for (vertex m : children)
        {
            m.g_cost = parent.g_cost + 1;

            find_duplicated(&(*tree), m); // If removing a non-max element, heapfy must be done again

            if (find_m(&(*tree), m) == false)
            {
                m.parent = parent.name;
                m.name = tree->currentStateIndex++;
                m.h_cost = heuristics(heuristic_used, m.state, tree->T);
                m.f_cost = m.g_cost + m.h_cost;
                tree->A.push_back(m);
                push_heap(tree->A.begin(), tree->A.end(), comp);
            }
        }
    }
    return NIL;
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
    // Normal Execution
    /*
    A_Star tree;
    string in;
    getline(cin, in);
    while (in.at(0) == ' ')
        in.erase(0, 1);
    vector<int> entry = split(in, ' ');
    cout << A_Star_Algorithm(&tree, entry);
    */

    // Tests
    A_Star A, B, C, D, E, F, G, H, I, J;
    vector<int> a = {1, 5, 9, 13, 2, 10, 7, 14, 3, 6, 11, 15, 4, 8, 12, 0}; // = 8  (Run.Codes 1)
    vector<int> b = {1, 5, 9, 13, 3, 2, 10, 14, 6, 7, 11, 15, 4, 8, 12, 0}; // = 10 (Run.Codes 2)
    vector<int> c = {2, 1, 10, 9, 3, 5, 11, 13, 4, 6, 12, 14, 0, 7, 8, 15}; // = 15 (Run.Codes 3)
    vector<int> d = {0, 2, 1, 9, 3, 5, 6, 13, 4, 7, 10, 14, 8, 12, 15, 11}; // = 18 (Run.Codes 4)
    vector<int> e = {3, 2, 1, 5, 4, 7, 6, 10, 8, 11, 0, 9, 12, 15, 14, 13}; // = 30 (Classroom 1)
    vector<int> f = {0, 5, 9, 13, 2, 10, 15, 14, 1, 4, 3, 6, 8, 11, 12, 7}; // = 30 (Classroom 2)
    vector<int> g = {9, 0, 13, 10, 5, 2, 6, 14, 1, 11, 15, 12, 7, 3, 4, 8}; // = 25 (Classroom 3)
    vector<int> h = {15, 9, 0, 7, 13, 1, 11, 14, 5, 4, 10, 2, 3, 8, 6, 12}; // = 46 (Classroom 4)
    vector<int> i = {5, 13, 6, 10, 1, 7, 2, 9, 4, 3, 15, 14, 8, 0, 11, 12}; // = 20 (Extra 1)
    vector<int> j = {2, 10, 11, 9, 3, 1, 0, 13, 4, 6, 7, 14, 5, 8, 12, 15}; // = 27 (Extra 2)

    //assert(A_Star_Algorithm(&A, a) == 8);
    //assert(A_Star_Algorithm(&B, b) == 10);
    //assert(A_Star_Algorithm(&C, c) == 15);
    //assert(A_Star_Algorithm(&D, d) == 18);
    //assert(A_Star_Algorithm(&E, e) == 30);
    //assert(A_Star_Algorithm(&F, f) == 30);
    //assert(A_Star_Algorithm(&G, g) == 25);
    //assert(A_Star_Algorithm(&H, h) == 46);
    //assert(A_Star_Algorithm(&I, i) == 20);
    //assert(A_Star_Algorithm(&J, j) == 27);

    cout << A_Star_Algorithm(&A, a) << endl;
    cout << A_Star_Algorithm(&B, b) << endl;
    cout << A_Star_Algorithm(&C, c) << endl;
    cout << A_Star_Algorithm(&D, d) << endl;
    cout << A_Star_Algorithm(&E, e) << endl;
    cout << A_Star_Algorithm(&F, f) << endl;
    cout << A_Star_Algorithm(&G, g) << endl;
    cout << A_Star_Algorithm(&H, h) << endl;
    cout << A_Star_Algorithm(&I, i) << endl;
    cout << A_Star_Algorithm(&J, j) << endl;
}
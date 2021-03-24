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
/*
3cardpoker.cpp
Hunter Barndt
Started: 11/17
Purpose: 3 card poker probabilities and returns

Notes: Could've more efficiently encoded the data but it's clean and organized this way

- Possible suit and values values in a deck:
Suits:
    - D
    - H
    - C
    - S
Values:
    - Ace = 1
    - 2 = 2
    - 3 = 3
    - 4 = 4
    - 5 = 5
    - 6 = 6
    - 7 = 7
    - 8 = 8
    - 9 = 9
    - 10 = 10
    - Jack = 11
    - Queen = 12
    - King = 13
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>
#include <cstdlib>
using std::string;
using std::vector;
using std::cout;
using std::cin;
using std::endl;
using std::swap;
using std::div;

struct Card
{
    string suit;
    int val;
};

string computeRank(vector<Card> hand)
{
    bool flush_flag = false;
    bool straight_flag = false;

    // Checking for a flush
    if (hand[0].suit == hand[1].suit && hand[1].suit == hand[2].suit && hand[2].suit == hand[0].suit)
        flush_flag = true;
    // Checking for a straight
    if (hand[0].val == hand[1].val-1 && hand[1].val-1 == hand[2].val-2 && hand[0].val == hand[2].val-2 || hand[0].val == 1 && hand[1].val == 12 && hand[2].val == 13)
        straight_flag = true;
    // Checking for a straight flush
    if (flush_flag && straight_flag)
        return "SF";
    // Checking for Three of a Kind
    if (hand[0].val == hand[1].val &&
        hand[1].val == hand[2].val &&
        hand[0].val == hand[2].val)
        return "3K";
    if (straight_flag)
        return "ST";
    if (flush_flag)
        return "FL";
    // Checking for Pairs
    if (hand[0].val == hand[1].val || 
        hand[1].val == hand[2].val ||
        hand[2].val == hand[0].val)
        return "PR";
    return "HC";
};

vector<Card> generateAllCards(vector<char> suits, vector<int> values)
{
    vector<Card> all_cards;
    for (int i = 0; i < suits.size(); i++)
    {
        for (int j = 0; j < values.size(); j++)
        {
            Card tempcard;
            tempcard.suit = suits[i];
            tempcard.val = values[j];
            all_cards.push_back(tempcard);
        }
    }
    return all_cards;
}

vector<vector<Card>> generateAllHands(vector<Card> all_cards, bool perm_flag)
{
    vector<vector<Card>> all_hands;
    
    if (!perm_flag)
    {
        for (int a = 0; a < 50; ++a) 
        {
            for (int b = a + 1; b < 51; ++b) 
            {
                for (int c = b + 1; c < 52; ++c) 
                {
                    vector<Card> temphand;
                    temphand.push_back(all_cards[a]);
                    temphand.push_back(all_cards[b]);
                    temphand.push_back(all_cards[c]);
                    all_hands.push_back(temphand);
                }
            }
        }
    }
    else
    {
        for (int a = 0; a < 52; ++a) 
        {
            for (int b = 1; b < 52; ++b) 
            {
                for (int c = 2; c < 52; ++c) 
                {
                    vector<Card> temphand;
                    temphand.push_back(all_cards[a]);
                    temphand.push_back(all_cards[b]);
                    temphand.push_back(all_cards[c]);
                    all_hands.push_back(temphand);
                }
            }
        }       
    }
    cout << "Generated " << all_hands.size() << " hands!" << endl;
    return all_hands;
};

void sortAllHands(vector<vector<Card>> &all_hands)
{
    for (int i = 0; i < all_hands.size(); i++)
    {
        //Bubble Sort
        for (int j = 0; j < all_hands[i].size() - 1; j++)
            for (int k = 0; k < all_hands[i].size() - j - 1; k++)
                if (all_hands[i][k].val > all_hands[i][k + 1].val)
                    swap(all_hands[i][k], all_hands[i][k + 1]);
    }
    cout << "Sorted!" << endl;
}

int main()
{
    const int num_values = 13;
    const int num_suits = 4;
    const int num_cards = num_values * num_suits;

    vector<int> values(13);
    for (int i = 0; i < values.size(); i++)
        values[i] = i+1;
    vector<char> suits{'D', 'H', 'C', 'S'};
    vector<Card> all_cards = generateAllCards(suits, values);
    vector<vector<Card>> all_hand;
    
    char input;
    cout << "Would you like to generate permutations or combinations? [Options: 'P' or 'C']" << endl;
    cin >> input;

    if (input == 'C')
        all_hand = generateAllHands(all_cards, false);
    else
        all_hand = generateAllHands(all_cards, true);


    sortAllHands(all_hand);

    vector<string> ranks = {"SF", "3K", "ST", "FL", "PR", "HC"};
    vector<int> payout = {50, 40, 10, 5, 1, 0};
    vector<int> rank_count(6);
    vector<double> prob(6);
    vector<double> returns(6);
    cout.precision(10);

    for (int i = 0; i < all_hand.size(); i++)
    {
        string rank = computeRank(all_hand[i]);
        if (rank == ranks[0])
            rank_count[0] += 1;
        else if (rank == ranks[1])
            rank_count[1] += 1;
        else if (rank == ranks[2])
            rank_count[2] += 1;
        else if (rank == ranks[3])
            rank_count[3] += 1;
        else if (rank == ranks[4])
            rank_count[4] += 1;
        else
            rank_count[5] += 1;
    }
    
    double tot_return = 0;
    for (int i = 0; i < ranks.size(); i++)
    {
        double temp = (double)rank_count[i]/all_hand.size();
        prob[i] = temp * 100;
        returns[i] = (double)prob[i] * payout[i];
        tot_return += returns[i];
        cout << ranks[i] << " with " << rank_count[i] << " number of hands with " << prob[i] << " percent chance of getting them." << endl;
        cout << "Giving a return of " << returns[i] << endl;
    }
    cout << "Giving a total return of " << tot_return << endl;
}
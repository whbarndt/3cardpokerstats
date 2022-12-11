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

    // Checking for a royal flush
    if (hand[0].val == 1 && hand[1].val == 12 && hand[2].val == 13)
        return "RF";
    // Checking for three aces
    if (hand[0].val == 1 && hand[1].val == 1 && hand[2].val == 1)
        return "3A";
    // Checking for a flush
    if (hand[0].suit == hand[1].suit && hand[1].suit == hand[2].suit && hand[2].suit == hand[0].suit)
        flush_flag = true;
    // Checking for a straight
    if (hand[0].val == hand[1].val-1 && hand[1].val-1 == hand[2].val-2 && hand[0].val == hand[2].val-2)
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
        //Bubble Sort - only because it's simple and efficiency isn't necessary
        for (int j = 0; j < all_hands[i].size() - 1; j++)
            for (int k = 0; k < all_hands[i].size() - j - 1; k++)
                if (all_hands[i][k].val > all_hands[i][k + 1].val)
                    swap(all_hands[i][k], all_hands[i][k + 1]);
    }
    cout << "Sorted!" << endl;
}

void computeHandStats(string rank, vector<string> ranks, vector<int> &rank_count)
{
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
    else if (rank == ranks[5])
        rank_count[5] += 1;
    else if (rank == ranks[6])
        rank_count[6] += 1;
    else
        rank_count[7] += 1;
}

vector<vector<Card>> createSelectHands()
{
    vector<vector<Card>> select_hands;
    vector<string> select_suits = {"S", "H", "H", "D", "H", "S", "C", "C", "C", "C", "H", "D", "H", "C", "D", "S", "S", "C", "C", "D", "D", "S", "C", "C", "S", "S", "S", "C", "C", "C"};
    vector<int> select_values = {1, 1, 12, 6, 7, 8, 1, 2, 3, 3, 6, 2, 1, 1, 1, 4, 5, 6, 3, 7, 11, 3, 2, 10, 4, 4, 4, 1, 11, 12};
    for(int i = 0; i < 10; i++)
    {
        vector<Card> temp_hand(3);
        for(int j = 0; j < 3; j++)
        {
            Card temp_card = {select_suits[j], select_values[j]};
            temp_hand.push_back(temp_card);
        }
        select_hands.push_back(temp_hand);
    }
}

void drawCards(vector<Card> hand, vector<string> ranks, vector<vector<Card>> )
{
    cout << "Your hand is: \n" << hand[0].suit << hand[0].val << "\n" << hand[1].suit << hand[1].val << "\n" << hand[2].suit << hand[2].val << endl;
    int num_cards = 0;
    string rank = computeRank(hand);
    vector<int> rankcount;
    computeHandStats(rank, ranks, rankcount);
    //cout << "Your hand is: \n" << hand[0].suit << hand[0].val << "\n" << hand[1].suit << hand[1].val << "\n" << hand[2].suit << hand[2].val << endl;
    cout << "How many cards would you like to replace? [Options: 0, 1, 2, 3]" << endl;
    cin >> num_cards;
    if (num_cards == 0)
    {
        
    }
    else if(num_cards == 1)
    {

    }
    else if(num_cards == 2)
    {

    }
    else
    {
        
    }
    
}

void printChart(vector<string> ranks, vector<int> rank_count, vector<vector<Card>> all_hands, vector<double> prob, vector<int> payout, vector<double> returns)
{
    cout.precision(10);
    double tot_return = 0;
    for (int i = 0; i < ranks.size(); i++)
    {
        double temp = (double)rank_count[i]/all_hands.size();
        prob[i] = temp * 100;
        returns[i] = (double)prob[i] * payout[i];
        tot_return += returns[i];
        cout << ranks[i] << " with " << rank_count[i] << " number of hands with " << prob[i] << " percent chance of getting them." << endl;
        cout << "Giving a return of " << returns[i] << endl;
    }
    cout << "Giving a total return of " << tot_return << endl;
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
    vector<vector<Card>> all_hands;
    
    char input;
    cout << "Would you like to generate permutations or combinations? [Options: 'P' or 'C']" << endl;
    cin >> input;

    if (input == 'C')
        all_hands = generateAllHands(all_cards, false);
    else
        all_hands = generateAllHands(all_cards, true);

    sortAllHands(all_hands);

    vector<string> ranks_v2 = {"RF", "SF", "3A", "3K", "ST", "FL", "PR", "HC"};
    vector<int> payout_v2 = {250, 50, 100, 40, 10, 5, 1, 0};
    vector<int> rank_count_v2(8);
    vector<double> prob_v2(8);
    vector<double> returns_v2(8);

    // Homework 6 Statistics
    vector<vector<Card>> select_hands = createSelectHands();

    // All Hands Statistics
    for(int i = 0; i < all_hands.size(); i++)
    {
        string rank = computeRank(all_hands[i]);
        computeHandStats(rank, ranks_v2, rank_count_v2);
    }

    printChart(ranks_v2, rank_count_v2, all_hands, prob_v2, payout_v2, returns_v2);

    /*
    // Select Hands Statistics
    for(int i = 0; i < select_hands.size(); i++)
    {
        string rank = computeRank(select_hands[i]);
        computeHandStats(rank, ranks_v2, rank_count_v2);
    }
    */
}
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
#include <algorithm>
using std::string;
using std::vector;
using std::cout;
using std::cin;
using std::endl;
using std::swap;
using std::div;
using std::max_element;

struct Card
{
    string suit;
    int val;
};

vector<Card> generateAllCards(vector<char> suits, vector<int> values)
{
    vector<Card> global_cards;
    for (int i = 0; i < suits.size(); i++)
    {
        for (int j = 0; j < values.size(); j++)
        {
            Card temp_card;
            temp_card.suit = suits[i];
            temp_card.val = values[j];
            global_cards.push_back(temp_card);
        }
    }
    return global_cards;
}

vector<vector<Card>> generateAllHands(vector<Card> global_cards)
{
    vector<vector<Card>> global_hands;
    for (int i = 0; i < 50; ++i) 
    {
        for (int j = i + 1; j < 51; ++j) 
        {
            for (int k = j + 1; k < 52; ++k) 
            {
                vector<Card> temp_hand;
                temp_hand.push_back(global_cards[i]);
                temp_hand.push_back(global_cards[j]);
                temp_hand.push_back(global_cards[k]);
                global_hands.push_back(temp_hand);
            }
        }
    }
    cout << "Generated " << global_hands.size() << " hands!" << endl;
    return global_hands;
};

void sortCardsInHands(vector<vector<Card>> &hands)
{
    for (int i = 0; i < hands.size(); i++)
    {
        //Bubble Sort - only because it's simple and efficiency isn't necessary
        for (int j = 0; j < hands[i].size() - 1; j++)
            for (int k = 0; k < hands[i].size() - j - 1; k++)
                if (hands[i][k].val > hands[i][k + 1].val)
                    swap(hands[i][k], hands[i][k + 1]);
    }
    cout << "Sorted!" << endl;
}

string computeRank(vector<Card> hand)
{
    bool flush_flag = false;
    bool straight_flag = false;

    // Checking for a flush
    if (hand[0].suit == hand[1].suit && 
        hand[1].suit == hand[2].suit && 
        hand[2].suit == hand[0].suit)
        flush_flag = true;
    // Checking for a straight
    if (hand[0].val == hand[1].val-1 && 
        hand[1].val-1 == hand[2].val-2 && 
        hand[0].val == hand[2].val-2)
        straight_flag = true;
    // Checking for a royal flush
    if (hand[0].val == 1 && hand[1].val == 12 && hand[2].val == 13 && flush_flag)
        return "RF";
    // Checking for three aces
    if (hand[0].val == 1 && hand[1].val == 1 && hand[2].val == 1)
        return "3A";
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

void addRankToCount(string rank, vector<string> ranks, vector<int> &rank_count)
{
    for(int i = 0; i < ranks.size(); i++)
        if (rank == ranks[i])
            rank_count[i] += 1;
}

vector<vector<Card>> createSelectHands(const int size_of_hand)
{
    vector<vector<Card>> select_hands;
    vector<string> select_suits = {"S", "H", "H", "D", "H", "S", "C", "C", "C", "C", "H", "D", "H", "C", "D", "S", "S", "C", "C", "D", "D", "S", "C", "C", "S", "S", "S", "C", "C", "C"};
    vector<int> select_values = {1, 1, 12, 6, 7, 8, 1, 2, 3, 3, 6, 2, 1, 1, 1, 4, 5, 6, 3, 7, 11, 3, 2, 10, 4, 4, 4, 1, 12, 13};
    int num_select_hands = select_suits.size()/size_of_hand;
    for(int i = 0; i < num_select_hands; i++)
    {
        vector<Card> temp_hand;
        for(int j = 0; j < size_of_hand; j++)
        {
            Card temp_card;
            int j_index = j % size_of_hand;
            temp_card.suit = select_suits[j_index+i*size_of_hand];
            temp_card.val = select_values[j_index+i*size_of_hand];
            temp_hand.push_back(temp_card);
        }
        select_hands.push_back(temp_hand);
    }
    return select_hands;
}

bool CardsAreEqual(Card first, Card second)
{
    bool is_equal = false;
    if(first.suit == second.suit && first.val == second.val)
        is_equal = true;
    return is_equal;
}

int getPayoutOfRank(string rank, vector<string> ranks, vector<int> payout)
{
    for(int i = 0; i < ranks.size(); i++)
        if(rank == ranks[i])
            return payout[i];
}

vector<Card> getRemainingDeck(vector<Card> hand, vector<Card> global_cards)
{
    for(int i = 0; i < global_cards.size(); i++)
        for(int j = 0; j < hand.size(); j++)
            if(CardsAreEqual(hand[j], global_cards[i]))
                global_cards.erase(global_cards.begin()+i);
    return global_cards;
}

vector<vector<Card>> getPossibleHandsOneCardReplaced(int replaced_index, vector<Card> hand, vector<Card> remaining_cards, vector<Card> global_cards, vector<vector<Card>> global_hands)
{
    vector<vector<Card>> possible_hands;
    for(int i = 0; i < remaining_cards.size(); i++)
    {
        hand[replaced_index] = remaining_cards[i];
        possible_hands.push_back(hand); 
    }
    return possible_hands;
}

vector<vector<Card>> getPossibleHandsAllCardsReplaced(vector<Card> hand, vector<Card> remaining_cards, vector<Card> global_cards, vector<vector<Card>> global_hands)
{
    for(int i = 0; i < global_hands.size(); i++)
    {
        for(int j = 0; j < hand.size(); j++)
        {
            if((hand[j].val == global_hands[i][0].val && hand[j].suit == global_hands[i][0].suit) || 
               (hand[j].val == global_hands[i][1].val && hand[j].suit == global_hands[i][1].suit) ||
               (hand[j].val == global_hands[i][2].val && hand[j].suit == global_hands[i][2].suit))
                global_hands.erase(global_hands.begin()+i);
        }
    }
    return global_hands;
}

vector<double> computeExpectedValues(int num_cards_to_draw, vector<Card> hand, vector<string> ranks, vector<int> payout, vector<Card> global_cards, vector<vector<Card>> global_hands)
{
    vector<double> exp_vals;
    vector<Card> remaining_cards = getRemainingDeck(hand, global_cards);
    // Last part that's hardcoded, couldn't figure out a way to abstract for n size hands
    if (num_cards_to_draw == 1)
    {
        for(int i = 0; i < hand.size(); i++) // [i = 0 -> replace 0th index of hand with another card], goes with i reach hand size.
        {
            double temp_exp_val = 0;
            vector<vector<Card>> possible_hands = getPossibleHandsOneCardReplaced(i, hand, remaining_cards, global_cards, global_hands);
            sortCardsInHands(possible_hands);
            vector<int> possible_hand_payouts;
            for(int j = 0; j < possible_hands.size(); j++)
            {
                possible_hand_payouts.push_back(getPayoutOfRank(computeRank(possible_hands[j]), ranks, payout));
                temp_exp_val += (double)possible_hand_payouts[j] * num_cards_to_draw/remaining_cards.size();
            }
            exp_vals.push_back(temp_exp_val);
        }
        return exp_vals;
    }
    //if (num_cards_to_draw == 2) Not time to implement
    /*
    if (num_cards_to_draw == 3) Didn't get working in time
    {
        double temp_exp_val = 0;
        vector<vector<Card>> possible_hands = getPossibleHandsAllCardsReplaced(hand, remaining_cards, global_cards, global_hands);
        sortCardsInHands(possible_hands);
        vector<int> possible_hand_payouts;
        for(int j = 0; j < possible_hands.size(); j++)
        {
            possible_hand_payouts.push_back(getPayoutOfRank(computeRank(possible_hands[j]), ranks, payout));
            temp_exp_val += (double)possible_hand_payouts[j] * num_cards_to_draw/remaining_cards.size();
        }
        exp_vals.push_back(temp_exp_val);
    }
    return exp_vals;
    */
}

void printChart(vector<string> ranks, vector<int> rank_count, vector<vector<Card>> hands, vector<double> prob, vector<int> payout, vector<double> returns)
{
    double tot_return = 0;
    for (int i = 0; i < ranks.size(); i++)
    {
        double temp = (double)rank_count[i]/hands.size();
        prob[i] = temp * 100;
        returns[i] = (double)prob[i] * payout[i];
        tot_return += returns[i];
        cout << ranks[i] << " with " << rank_count[i] << " number of hands with " << prob[i] << " percent chance of getting them." << endl;
        cout << "Giving a return of " << returns[i] << endl;
    }
    cout << "Giving a total return of " << tot_return << endl;
}

void drawCards(vector<Card> hand, vector<string> ranks, vector<int> payouts, vector<Card> global_cards, vector<vector<Card>> global_hands)
{
    cout << "Calculating expectation values for possible draws..." << endl;
    vector<vector<double>> expected_values;
    // Generalized:
    // for(int i = 1; i <= hand.size(); i++)
    // {
    //     expected_values.push_back(computeExpectedValues(i, hand, ranks, payout, global_cards, global_hands));
    // }
    expected_values.push_back(computeExpectedValues(1, hand, ranks, payouts, global_cards, global_hands));
    //expected_values.push_back(computeExpectedValues(3, hand, ranks, payouts, global_cards, global_hands));
    // Replace one card
    cout << "Best expectation value for drawing and replacing " << 1 << " cards is: " << (double)*std::max_element(expected_values[0].begin(), expected_values[0].end()) << endl;
    for(int j = 0; j < hand.size(); j++)
    {
        cout << "If you replace " << hand[j].suit << hand[j].val << " you will get an expectation value of " << expected_values[0][j] << endl;
    }
    // Replace all three cards
    //cout << "Expectation value for drawing and replacing 3 cards is: " << expected_values[1][4] << endl;
    int max_expectation_value = 0;
    int i_index = 0;
    int j_index = 0;
    for(int i = 0; i < expected_values.size(); i++)
    {
        for(int j = 0; j < expected_values[i].size(); j++)
        {
            if(expected_values[i][j] > max_expectation_value)
            {
                max_expectation_value = expected_values[i][j];
                i_index = i;
                j_index = j;
            }
        }
    }
    string selected_hand_rank = computeRank(hand);
    int selected_hand_payout = getPayoutOfRank(selected_hand_rank, ranks, payouts);
    vector<int> rank_count_expectation_value_hand(ranks.size());
    vector<double> prob_expectation_value_hand(ranks.size());
    vector<double> returns_expectation_value_hand(ranks.size());
    if(selected_hand_payout > max_expectation_value)
    {
        cout << "The best drawing choice is to keep your cards!" << endl;
        cout << "The return table with the rest of the cards is" << endl;
        vector<Card> remaining_cards = getRemainingDeck(hand, global_cards);
        vector<vector<Card>> other_possible_hands = getPossibleHandsAllCardsReplaced(hand, remaining_cards, global_cards, global_hands);
        for(int i = 0; i < other_possible_hands.size(); i++)
        {
            string rank = computeRank(other_possible_hands[i]);
            addRankToCount(rank, ranks, rank_count_expectation_value_hand);
        }
        printChart(ranks, rank_count_expectation_value_hand, other_possible_hands, prob_expectation_value_hand, payouts, returns_expectation_value_hand);
    }
    else
    {
        cout << "The best drawing choice is to draw one card!" << endl;
        cout << "If you replace " << hand[j_index].suit << hand[j_index].val << " you will get the best expectation value of " << expected_values[i_index][j_index] << "!" << endl;
        cout << "The return table if you draw and replace this card would be.." << endl;
        vector<Card> remaining_cards = getRemainingDeck(hand, global_cards);
        vector<vector<Card>> possible_hands = getPossibleHandsOneCardReplaced(j_index, hand, remaining_cards, global_cards, global_hands);
        for(int i = 0; i < possible_hands.size(); i++)
        {
            string rank = computeRank(possible_hands[i]);
            addRankToCount(rank, ranks, rank_count_expectation_value_hand);
        }
        printChart(ranks, rank_count_expectation_value_hand, possible_hands, prob_expectation_value_hand, payouts, returns_expectation_value_hand);
    }
}

int main()
{
    const int num_values = 13;
    const int num_suits = 4;
    const int num_cards = num_values * num_suits;
    const int size_of_hands = 3;

    vector<int> values(13);
    for (int i = 0; i < values.size(); i++)
        values[i] = i+1;
    vector<char> suits{'D', 'H', 'C', 'S'};
    vector<Card> global_cards = generateAllCards(suits, values);
    vector<vector<Card>> global_hands = generateAllHands(global_cards);
    sortCardsInHands(global_hands);

    vector<string> ranks_v2 = {"RF", "SF", "3A", "3K", "ST", "FL", "PR", "HC"};
    vector<int> payouts_v2 = {250, 50, 100, 40, 10, 5, 1, 0};
    const int num_of_ranks_v2 = ranks_v2.size();
    vector<int> rank_count_global(num_of_ranks_v2);
    vector<double> prob_global(num_of_ranks_v2);
    vector<double> returns_global(num_of_ranks_v2);

    // Homework 6 Statistics
    vector<vector<Card>> select_hands = createSelectHands(size_of_hands);
    sortCardsInHands(select_hands);

    // All Hands Statistics
    for(int i = 0; i < global_hands.size(); i++)
    {
        string rank = computeRank(global_hands[i]);
        addRankToCount(rank, ranks_v2, rank_count_global);
    }

    cout.precision(10);
    printChart(ranks_v2, rank_count_global, global_hands, prob_global, payouts_v2, returns_global);

    int input = 0;
    for(int i = 0; i < select_hands.size(); i++)
    {
        cout << "Hand Options: \n" << select_hands[i][0].val << select_hands[i][0].suit << ", " << select_hands[i][1].val << select_hands[i][1].suit << ", " << select_hands[i][2].val << select_hands[i][2].suit << endl;
    }
    cout << "Pick a sample hand by choosing a number between 1 - 10: " << endl;
    //cin >> input;
    input = 4;
    vector<Card> selected_hand = select_hands[input-1];
    cout << "Your hand is: \n" << selected_hand[0].val << selected_hand[0].suit << "\n" << selected_hand[1].val << selected_hand[1].suit << "\n" << selected_hand[2].val << selected_hand[2].suit << endl;
    string selected_hand_rank = computeRank(selected_hand);
    int selected_hand_payout = getPayoutOfRank(selected_hand_rank, ranks_v2, payouts_v2);
    cout << "With a " << selected_hand_rank << " rank hand and a payout and return of " << selected_hand_payout << endl;
    drawCards(selected_hand, ranks_v2, payouts_v2, global_cards, global_hands);
}
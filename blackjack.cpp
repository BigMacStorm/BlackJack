#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <ctime>

#include "data.h"
#include "Timer.h"

using namespace std;

int MUTATION_RATE = 1; //must be between 0 and 10000, represents percentage
int deckCount = 6;
double wins = 0;
long long int RUNS = 10000;
vector<moves> choices;    
set info;

void initDeck(vector<int>& sent);
bool play(Dhand& dealer, Phand& player, vector<int>& sent, bool split);
bool learn(Dhand& dealer, Phand& player, vector<int>& sent, bool split);
void shuffle(vector<int>& sent);
void deal(Dhand& dealer, Phand& player, vector<int>& sent);
void resolve(Dhand& dealer, Phand& player, int& x, int& y);
void board(Dhand& dealer, Phand& player);

int main()
{
    //set up a timer and seed the random and create the decks and hands
    Timer clock;
    srand(time(NULL));
    vector<int> deck(52*deckCount);
    vector<int> copy(52*deckCount);
    Dhand dealer;
    Phand player;
    
    //initialize the decks by loading in 
    initDeck(deck);
    initDeck(copy);   
    
    char temp;
    
    while(true)
    {
        cout << "What would you like to do? ((s)how, (p)lay), (l)earn: ";
        cin >> temp;
        switch(temp)
        {
            //this section is for if a human player wishes to play a single game, but havent gotten a chance to implement the play function yet, so play currently does not do much.
            /*case 'p':
                initDeck(deck);
                shuffle(deck);
                    
                deal(dealer, player, deck);
                
                board(dealer, player);
                play(dealer, player, deck, false);
                
                cout <<  endl << "========================" << endl << "      NEW GAME" << endl << "========================" << endl;
                break; */
            case 'l':
                /*runs a timer to see how quickly Runs amount takes for debugging issues.
                  this section first copies the original deck to a temp copy and shuffles it
                  as reinitializing the deck every time was too slow. It then loops through run times
                  in order to simulate that many games played. */
                wins = 0;
                clock.start();
                for(long long int x=0; x<RUNS; x++)
                {
                    copy = deck;
                    shuffle(copy);
                    
                    deal(dealer, player, copy);
                    if(learn(dealer, player, copy, false))
                    {
                        wins++;
                    }
                }
                clock.stop();
                cout << clock.getElapsedTime() << endl;
                cout << "won: " << (wins/RUNS)*100 << "%" << endl;
                break;
            case 's':
                info.show();
                break;
        }
    }
    
    return 0;
}

/* This function is used at the beginning of the program in order to fill the original deck with cards
   originally this was meant to be ran several times, but I learned that it was more efficient to simply
   copy the deck to a temp holder and shuffle them.*/
void initDeck(vector<int>& sent)
{
    //resize the deck to 52
    sent.resize(52*deckCount);
    int count = 0;
    //add in the number cards
    for(int x=2; x<=9; x++)
    {
        for(int y=0; y<4*deckCount; y++)
        {
            sent[count] = x;
            count++;
        }
    }
    //add in the face cards
    for(int x=0; x<16*deckCount; x++)
    {
        sent[count] = 10;
        count++;
    }
    //add in the aces
    for(int x=0; x<4*deckCount; x++)
    {
        sent[count] = 11;
        count++;
    }
}

//standard Fisher-Yates algorithm for shuffling, very quick and efficient
void shuffle(vector<int>& sent)
{
    int y, temp;
    for(int x=52*deckCount-1; x>=(52*deckCount-1)-40; x--)
    {
        y = rand() % (52*deckCount);
        temp = sent[x];
        sent[x] = sent[y];
        sent[y] = temp;
    }    
}

//deal takes in the player and dealer structs and sets the first set of data to them
//by dealing out data and deciding wether the result is soft or not.
void deal(Dhand& dealer, Phand& player, vector<int>& sent)
{
    //set default stuff
    player.total = 0;
    dealer.total = 0;
    player.count = -1;
    dealer.count = -1;
    dealer.show = false;
    player.soft = false;
    dealer.soft = false;

    for(int x=0; x<2; x++)
    {
    player.count++; 
    player.total += sent.back();
    player.cards[player.count] = sent.back();
    //sets soft if player pulls an ace, same as dealer below.
    if(player.cards[player.count] == 11) player.soft = true;
    sent.pop_back();
    
    dealer.count++; 
    dealer.total += sent.back();
    dealer.cards[dealer.count] = sent.back();
    if(dealer.cards[dealer.count] == 11) dealer.soft = true;
    sent.pop_back();
    }    
}

//not yet implemented, does not affect the learning function.
bool play(Dhand& dealer, Phand& player, vector<int>& sent, bool split)
{
}

/*The learning function that carries out most of the work.
  the function runs through several basic steps as all good algorithms  do
  1. Check to see if the play has pulled a blackjack draw
  2. */
bool learn(Dhand& dealer, Phand& player, vector<int>& sent, bool split)
{    
    int temp;
    bool done;
    moves made; 
    bool split2 = true;
    player.update();
    dealer.update();
    if(player.total == 21)
    {
        done = true;
    }
    //store all choices in array, mod values based on cahrs in array
    while(player.total < 21 && temp != 2 && temp != 4 && split2)
    {
        //just to ensure that the split is handled need a throw for it.
        split2 = false;
        //resolve board to a number
        resolve(dealer, player, made.left, made.top);
        //temp is what move was decided after the roll
        temp = info.data[made.left][made.top].roll();
        //stores the move made into the moves container
        made.move = temp;
        
        //add to vector
        choices.push_back(made);
        
        //if hit
        if(temp == 1)
        {
            //deal out one card
            player.count++;
            player.total += sent.back();
            player.cards[player.count] = sent.back();
            if(player.cards[player.count] == 11) player.soft = true;
            sent.pop_back(); 
            player.update();      
        }
        //if you decide to double
        else if(temp == 4)
        {
            player.count++;
            player.total += sent.back();
            player.cards[player.count] = sent.back();
            if(player.cards[player.count] == 11) player.soft = true;
            sent.pop_back(); 
            player.update(); 
            //putting this line here ensures doubling is worth more.
            choices.push_back(made);      
            choices.push_back(made);      
        }
        /*if a split is chosen
          create a second hand, copy the cards to the start of both of those 
          hands, and then deal out two new cards, one each*/
        else if(temp == 3)
        {
            //cant split on a split
            
            if(!split && player.cards[0] == player.cards[1])
            {
                //make the two hands and split the cards
                Phand first;
                Phand second;
                first.count++;
                first.total += player.cards[0];
                first.cards[first.count] = player.cards[0];   
                if(first.cards[first.count] == 11) first.soft = true;  
                
                second.count++;
                second.total += player.cards[1];
                second.cards[second.count] = player.cards[1];   
                if(second.cards[second.count] == 11) second.soft = true;         
                
                //deal to the first hand
                first.count++;
                first.total += sent.back();
                first.cards[first.count] = sent.back();
                if(first.cards[first.count] == 11) first.soft = true;
                sent.pop_back(); 
                first.update(); 
                
                //deal to the second hand
                second.count++;
                second.total += sent.back();
                second.cards[second.count] = sent.back();
                if(second.cards[second.count] == 11) second.soft = true;
                sent.pop_back(); 
                second.update(); 
                //find the results of the games
                choices.push_back(made);
                if(learn(dealer, first, sent, true) || learn(dealer, first, sent, true))
                {
                    done = true;
                }
                else
                {
                    done = false;
                }
            }
            else
            {
                if(!split)
                {
                    split2 = true;
                }
            }
        }
       
    }    
    dealer.show = true;
    //this line is key, decides for dealer stand on 17
    //if you uncomment the other half, changes to hit on soft 17
    //changes the math quite a bit.
    while(dealer.total < 17 /*|| (dealer.total == 17 && dealer.soft)*/)
    {
        dealer.count++;
        dealer.total += sent.back();
        dealer.cards[dealer.count] = sent.back();
        if(dealer.cards[dealer.count] == 11) dealer.soft = true;
        sent.pop_back();  
        dealer.update();
    }
    
    //list of all of the ending conditions.
    //player hit a blackjack this draw        
    if(player.total == 21)
    {
        done = true;
    }
    //player bust this draw
    else if(player.total > 21)
    {
        done = false;
    } 
    //dealer bust
    else if(dealer.total > 21)
    {
        done = true;
    }
    //player and dealer both blackjack
    else if((dealer.total == 21 && player.total == 21) || (dealer.total == player.total))
    {  
        done = true;
    }
    //dealer wins after player stands
    else if(dealer.total > player.total)
    {  
        done = false;
    }
    //dealer loses.
    else if(dealer.total < player.total)
    {  
        done = true;
    }
    /*The split command check here is important, because if a split is called 
    then this function is called on both sides, but it does not want to learn
    from the other side, just from the original call, which makes it basically
    so that splits arent weighted super high, and because then it would be
    difficult for the program to discern wether it was in a split or not.*/
    if(!split)
    {
        //iterate through the choices
        while(choices.size()>0)
        {
            made = choices.back();
            //if you won
            if(done)
            {
                /*if you blackjack, this must be a great move, so add 4 to that
                  "slice" of the roulette wheel*/                
                if(player.total == 21 && dealer.total != 21)
                    info.data[made.left][made.top].data[made.move-1] += 4;
                /*if you push at blackjack, this means you got a blackjack but 
                  probably unlucky on the dealers playing, so this must be 
                  a good move, so add 3 to that "slice" of the roulette wheel*/   
                else if(player.total == 21 && dealer.total == 21)
                    info.data[made.left][made.top].data[made.move-1] += 3;
                /*if you push, this can either be bad luck but probably
                  means you didnt make a horrible call, so this must be 
                  a positive move, so add 2 to that "slice" of the roulette wheel*/  
                else if(player.total == dealer.total)
                    info.data[made.left][made.top].data[made.move-1] += 2;
                //you won without blackjack and beat the dealer, add one only.
                else
                    info.data[made.left][made.top].data[made.move-1] += 1;
                 
            }
            //if you lost
            else if(!done)
            {
                    //remove a point from that "slice" of the wheel
                    info.data[made.left][made.top].data[made.move-1] -= 1;
                    //you never want a negative value slice, and I kept 1
                    //to always allow a small element of mutation
                    if(info.data[made.left][made.top].data[made.move-1] <= 0)
                        info.data[made.left][made.top].data[made.move-1] = 1;
            }
            choices.pop_back();
        }
    }
    
    return done;
}

//resolves the current board state to a numerical value for left and top
void resolve(Dhand& dealer, Phand& player, int& x, int& y)
{
    char temp;
    y=dealer.cards[1]-2;
    //regular
    if(!(player.soft) && (player.cards[0] != player.cards[1]))
    {
        x=player.total-4;
    }
    //soft
    else if((player.soft) && (player.cards[0] != player.cards[1]))
    {
        x=player.total+4;
    }
    //double
    else if(!(player.soft) && (player.cards[0] == player.cards[1]))
    {
        x=player.cards[0]+23;
    }
    //double aces
    else if((player.soft) && (player.cards[0] == player.cards[1]))
    {
        x=34;
    }
}

//outputs the board, which just called the print command for the player
//and the dealer, used primarily for debugging.
void board(Dhand& dealer, Phand& player)
{
    int left, top;
    dealer.print();
    player.print();
    resolve(dealer, player, left, top);
    cout << "Board position: " << left << " " << top << endl;
}


















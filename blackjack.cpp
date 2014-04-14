#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <ctime>

#include "data.h"
#include "Timer.h"

using namespace std;

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
    Timer clock;
    srand(time(NULL));
    vector<int> deck(52*deckCount);
    vector<int> copy(52*deckCount);
    Dhand dealer;
    Phand player;
    
    initDeck(deck);
    initDeck(copy);   
    
    char temp;
    
    while(true)
    {
        cout << "What would you like to do? ((s)how, (p)lay), (l)earn: ";
        cin >> temp;
        switch(temp)
        {
            case 'p':
                initDeck(deck);
                shuffle(deck);
                    
                deal(dealer, player, deck);
                
                board(dealer, player);
                play(dealer, player, deck, false);
                
                cout << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << "========================" << endl << "      NEW GAME" << endl << "========================" << endl;
                break;
            case 'l':
                wins = 0;
                clock.start();
                for(long long int x=0; x<RUNS; x++)
                {
                    copy = deck;
                    //initDeck(deck);
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

void initDeck(vector<int>& sent)
{
    sent.resize(52*deckCount);
    int count = 0;
    for(int x=2; x<=9; x++)
    {
        for(int y=0; y<4*deckCount; y++)
        {
            sent[count] = x;
            count++;
        }
    }
    
    for(int x=0; x<16*deckCount; x++)
    {
        sent[count] = 10;
        count++;
    }
    
    for(int x=0; x<4*deckCount; x++)
    {
        sent[count] = 11;
        count++;
    }
}

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

void deal(Dhand& dealer, Phand& player, vector<int>& sent)
{
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
    if(player.cards[player.count] == 11) player.soft = true;
    sent.pop_back();
    
    dealer.count++; 
    dealer.total += sent.back();
    dealer.cards[dealer.count] = sent.back();
    if(dealer.cards[dealer.count] == 11) dealer.soft = true;
    sent.pop_back();
    }    
}

bool play(Dhand& dealer, Phand& player, vector<int>& sent, bool split)
{
}

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
    //story all choices in array, mod values based on cahrs in array
    while(player.total < 21 && temp != 2 && temp != 4 && split2)
    {
        split2 = false;
        //construct move object
        resolve(dealer, player, made.left, made.top);
        temp = info.data[made.left][made.top].roll();
        made.move = temp;
        
        //add to vector
        choices.push_back(made);
        
        if(temp == 1)
        {
            player.count++;
            player.total += sent.back();
            player.cards[player.count] = sent.back();
            if(player.cards[player.count] == 11) player.soft = true;
            sent.pop_back(); 
            player.update();      
        }
        else if(temp == 4)
        {
            player.count++;
            player.total += sent.back();
            player.cards[player.count] = sent.back();
            if(player.cards[player.count] == 11) player.soft = true;
            sent.pop_back(); 
            player.update(); 
            choices.push_back(made);      
        }
        else if(temp == 3)
        {
            if(!split && player.cards[0] == player.cards[1])
            {
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
                
                first.count++;
                first.total += sent.back();
                first.cards[first.count] = sent.back();
                if(first.cards[first.count] == 11) first.soft = true;
                sent.pop_back(); 
                first.update(); 
                
                second.count++;
                second.total += sent.back();
                second.cards[second.count] = sent.back();
                if(second.cards[second.count] == 11) second.soft = true;
                sent.pop_back(); 
                second.update(); 
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
    while(dealer.total < 17 /*|| (dealer.total == 17 && dealer.soft)*/)
    {
        dealer.count++;
        dealer.total += sent.back();
        dealer.cards[dealer.count] = sent.back();
        if(dealer.cards[dealer.count] == 11) dealer.soft = true;
        sent.pop_back();  
        dealer.update();
    }
            
    if(player.total == 21)
    {
        done = true;
    }
    else if(player.total > 21)
    {
        done = false;
    } 
    else if(dealer.total > 21)
    {
        done = true;
    }
    else if((dealer.total == 21 && player.total == 21) || (dealer.total == player.total))
    {  
        done = true;
    }
    else if(dealer.total > player.total)
    {  
        done = false;
    }
    else if(dealer.total < player.total)
    {  
        done = true;
    }
    if(!split)
    {
        while(choices.size()>0)
        {
            made = choices.back();
            if(done)
            {
                if(player.total == 21 && dealer.total != 21)
                    info.data[made.left][made.top].data[made.move-1] += 4;
                else if(player.total == 21 && dealer.total == 21)
                    info.data[made.left][made.top].data[made.move-1] += 3;
                else if(player.total == dealer.total)
                    info.data[made.left][made.top].data[made.move-1] += 2;
                else
                    info.data[made.left][made.top].data[made.move-1] += 1;
                 
            }
            else if(!done)
            {
                    info.data[made.left][made.top].data[made.move-1] -= 1;
                    if(info.data[made.left][made.top].data[made.move-1] <= 0)
                        info.data[made.left][made.top].data[made.move-1] = 1;
            }
            choices.pop_back();
        }
    }
    
    return done;
}

void resolve(Dhand& dealer, Phand& player, int& x, int& y)
{
    char temp;
    y=dealer.cards[1]-2;
    if(!(player.soft) && (player.cards[0] != player.cards[1]))
    {
        x=player.total-4;
    }
    else if((player.soft) && (player.cards[0] != player.cards[1]))
    {
        x=player.total+4;
    }
    else if(!(player.soft) && (player.cards[0] == player.cards[1]))
    {
        x=player.cards[0]+23;
    }
    else if((player.soft) && (player.cards[0] == player.cards[1]))
    {
        x=34;
    }
}

void board(Dhand& dealer, Phand& player)
{
    int left, top;
    dealer.print();
    player.print();
    resolve(dealer, player, left, top);
    cout << "Board position: " << left << " " << top << endl;
}


















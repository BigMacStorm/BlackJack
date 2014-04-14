#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <ctime>

using namespace std;

struct cell{
    int data[4];
    
    cell()
    {
        data[0] = 100;
        data[1] = 100;
        data[2] = 100;
        data[3] = 100;
    }
    
    void print()
    {
       int temp = 0;
       for(int x=1; x<4; x++)
       {
           if(data[x] > data[temp]) temp = x;
       }
       cout << temp+1;
    }
    
    int roll();
};

struct set{
    cell data[35][10];
    
    set()
    {
        for(int x = 0; x<25; x++)
        {
            for(int y=0; y<10; y++)
            {
                data[x][y].data[2] = 0;
            }
        }
    }
    
    void show()
    {
        for(int x=0; x<35; x++)
        {
            for(int y=0; y<10; y++)
            {
                data[x][y].print();
                cout << ", ";
            }
            cout << endl;
        }
    }
};

struct moves
{
    int left;
    int top;
    int move;
};

struct Phand{
    int total;
    int count;
    bool soft;
    int cards[30];
    
    Phand():total(0), soft(false), count(-1){}  
    
    void print(); 
    void update();
};

struct Dhand{
    int total;
    int count;
    bool soft;
    bool show;
    int cards[30];
    
    Dhand():total(0), soft(false), show(false), count(-1){}
    
    void print();
    void update();
     
};

int cell::roll()
{
    int total = 0;
    for(int x=0; x<4; x++)
    {
        total += data[x];
    }
    if(total <= 0) return rand() % 4;
    int rng = rand() % total + 1;
    if(rng > 0 && rng <= data[0]) return 1;
    if(rng > data[0] && rng <= data[0]+data[1]) return 2;
    if(rng > data[0]+data[1] && rng <= data[0]+data[1]+data[2]) return 3;
    if(rng > data[0]+data[1]+data[2] && rng <= data[0]+data[1]+data[2]+data[3]) return 4;
    return rand() % 4 + 1;
}

void Dhand::print()
{
    cout << "        Dealer's cards" << endl;
    if(!show)
    {
        cout << " Cards: X ";
        for(int x=1; x<count+1; x++)
        {
            cout << cards[x] << " ";
        }
        cout << endl << " Total: ?" << endl;
        cout << " Soft: ???" << endl;
    }
    else
    {
        cout << " Cards: ";
        for(int x=0; x<count+1; x++)
        {
            cout << cards[x] << " ";
        }
        cout << endl << " Total: " << total << endl;
        cout << " Soft: ";
        if(soft) cout << "True" << endl;
        else cout << "False" << endl;
    }
    cout << endl;   
} 

void Phand::print()
{
    cout << "        Player's cards" << endl;
    cout << " Cards: ";
    for(int x=0; x<count+1; x++)
    {
        cout << cards[x] << " ";
    }
    cout << endl << " Total: " << total << endl;
    cout << " Soft: ";
    if(soft) cout << "True" << endl;
    else cout << "False" << endl;
    cout << endl;   
} 

void Phand::update()
{
    if(total > 21 && soft)
    {
        for(int x=0; x<count; x++)
        {
            if(cards[x] == 11)
            {
                cards[x] = 1;
                x = count+1;
                soft = false;
                total -= 10;
            }
        }
        for(int x=0; x<count; x++)
        {
            if(cards[x] == 11)
            {
                soft = true;
            }
        }
    }
}

void Dhand::update()
{
    if(total > 21 && soft)
    {
        total -= 10;
        for(int x=0; x<count; x++)
        {
            if(cards[x] == 11)
            {
                cards[x] = 1;
                x = count+1;
                soft = false;
            }
        }
        for(int x=0; x<count; x++)
        {
            if(cards[x] == 11)
            {
                soft = true;
            }
        }
    }
}





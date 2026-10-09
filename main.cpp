// #include<bits/stdc++>
#include<iostream>
#include<string>
#include<fstream>
#include<map>
#include<queue>
#include<vector>

using namespace std;

struct Node{
    // so far we had char and thier frequency okay. 
    // but huffman needs to combine these chars into BINARY TREE;
    // for that we need chars, freq, left and right child.
    int freqncy;
    char ch;

    Node* left;
    Node* right;

    Node( char c, int f){
        ch=c;
        freqncy= f;
        left=nullptr;
        right=nullptr;
    }

};
 
struct Compare {
    bool operator()(Node* a, Node* b){
        return a->freqncy > b->freqncy;
    }
};

int main (){
    map<char,int> freq;
    


    ifstream file("input.txt"); // means open a file for reading.
    if( !file){
        cout<<"cant open it \n";
    }
    string content;
    string line;
    while(getline(file, line)){  //getline- reads one complete line from file.
        content+=line + "\n";
    }

    file.close(); // close the file after we're finished.
    cout<<"file content  : \n";

    cout<<content;

    for(char c : content){
        freq[c]++;
    }

    for(auto pair: freq){
        if( pair.first== ' '){
            cout<< "[spaces] :"<<pair.second<<endl;
        }
        else if( pair.first== '\n'){
            cout<<"[newline]: " <<pair.second<<endl;
        }
        else
        cout<<pair.first<< " -"<< pair.second<<endl;
    }
//in the o/p u might see , count of newline and spaces appearing before characteres, 
// THE reason is thier ASCII VALUES. ' ' -32  and  '\n' - 10




    priority_queue<Node*, vector<Node*>, Compare> pq;

    return 0;
}   
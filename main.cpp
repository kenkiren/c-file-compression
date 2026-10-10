// #include<bits/stdc++>
#include<iostream>
#include<string>
#include<fstream>
#include<map>
#include<queue>
#include<vector>
#include<unordered_map>
#include<bitset>
#include <cstdint>

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


void generateCode( Node* root, string code, unordered_map<char, string>& codes){
    if( root ==NULL){
        return ;
    }
    if( root ->left== nullptr && root->right ==nullptr){ //leaf node. 
        codes[root->ch]= code;
        return;
    }
    generateCode( root->left, code+ "0" , codes);
    generateCode( root->right, code+ "1" , codes);
}


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
    // cout<<"file content  : \n";

    // cout<<content;

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


    for(auto pair: freq){
        Node* node= new Node( pair.first, pair.second);
        pq.push(node);
    }// filling pq. which is min heap.

    while( pq.size()>1 ){
        Node* left= pq.top();
        pq.pop();

        Node* right = pq.top();
        pq.pop();

        Node* parent= new Node( '\0', left->freqncy+ right->freqncy);
        parent->left= left;
        parent->right= right;
        pq.push(parent);
    }

    Node* root= pq.top(); //here root is the single node left in the pq. which is the root of huffman tree. the value of root is the sum of all frequencies.
    

    cout<< " huffman tree built \n";
    cout<< " root frequency :" <<root->freqncy<<endl; //huffman tree is working.
    //now we need to generate codes for each char.




//now we generate huffman codes. (0 and 1)  .

    unordered_map< char, string> codes;
    generateCode( root, "", codes);
    for( auto item : codes){
        cout<< item.first<< " :"<<item.second<<endl;
    }

    //encoding code
    string encode= "";
    for( char c: content){
        encode+= codes[c];
    }
    cout<< " \n encoded bits : \n";
    cout<< encode<<endl;
    cout<<" original chars: " << content.size()<<endl;
    cout<<" encode bits:" << encode.size()<<endl;


    //step9- convert bits into byte
    string binaryBytes="";
    for( size_t i =0; i<encode.size();i+=8){
        string byte = encode.substr(i,8);
        if(byte.size() < 8){
            byte.append(8- byte.size(), '0');

        }
        binaryBytes+=byte;
    }
    cout << "\nBits padded to full bytes:\n";

    for (size_t i = 0; i < binaryBytes.size(); i += 8) {
        cout << binaryBytes.substr(i, 8) << " ";
    } // now u see padded full bytes each having 8 bits.

    cout << endl;


    //step 10 convert binary string to actual byte


    string byteString = "";
    vector<unsigned char> bytes;

    for (size_t i = 0; i < binaryBytes.size(); i += 8) {
        byteString = binaryBytes.substr(i, 8);

        unsigned char value = 0;

        for (char bit : byteString) {
            value = value * 2 + (bit - '0');
        }
        // that value, it holds the actual value of that BYTE( 8 bits). 
        bytes.push_back(value);
    }

    cout << "Actual bytes: " << bytes.size() << endl;
    

    //now we save our compressed bytes into a FILEEE. 


    ofstream output( " compressed.bin ", ios::binary);

    if( !output){
        cout<<" cant create the output file \n";
        return 1;
    }
        // File signature
    output.write("HUF1", 4);

    // Original content size
    uint64_t originalSize = content.size();

    output.write(
        reinterpret_cast<const char*>(&originalSize),
        sizeof(originalSize)
    );

    // Number of unique characters
    uint16_t uniqueChars = freq.size();

    output.write(
        reinterpret_cast<const char*>(&uniqueChars),
        sizeof(uniqueChars)
    );

    // Save each character and its frequency
    for (auto item : freq) {
        char ch = item.first;
        uint64_t count = item.second;

        output.write(&ch, sizeof(ch));

        output.write(
            reinterpret_cast<const char*>(&count),
            sizeof(count)
        );
    }
    output.write( 
        reinterpret_cast< const char* > ( bytes.data()),
        bytes.size()
    );
    output.close();
    cout<< " binary file is successfully created \n";


    return 0;
}   
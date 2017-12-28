#include <iostream>
#include <cstdint>
#include <iomanip>
#include <vector>
#include <ctime>
#include <sstream>
#include <openssl/sha.h>


using namespace std;

//Function to generate SHA256 hash using openSSL library

string sha256(const string str)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);
    stringstream ss;
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        ss << hex << setw(2) << setfill('0') << (int)hash[i];
    }
    return ss.str();
}

class block {

public:

    string prev_hash; //Hash of previous block

    block(uint32_t index,const string &data): b_index(index), b_data(data) {
        b_nonce = 0;
        b_time = time(nullptr);
        b_hash=calculateHash();


    }

    string getHash(){ //Get Hash of the block
        return b_hash;
    }

    void mineBlock(uint32_t difficulty){
        char cstr[difficulty];
        for(uint32_t i=0; i<difficulty;i++){
            cstr[i]='0';
        }

        cstr[difficulty]='\0';

        string str(cstr);

        while(b_hash.substr(0,difficulty)!=str) {
            cout<<b_nonce<<endl;//To actually see how much work is done to mine a block
            b_nonce++;
            b_hash=calculateHash();

        }

        cout<<"Block Mined "<<b_hash<<endl;

    }


private:

    uint32_t b_index;
    int64_t b_nonce;
    string b_data;
    string b_hash;
    time_t b_time;

   inline string calculateHash() const {
        stringstream ss;
        ss<<b_index<<prev_hash<<b_data<<b_time<<b_nonce;


        return sha256(ss.str());

    }

};

class blockchain {
public:
    blockchain(){
        vchain.emplace_back(block(0,"Genesis Block"));//initializing the Blockchain
        difficulty = 6;     //Setting difficulty to mine the block
    }

    void addBlock(block newBlock) {
        newBlock.prev_hash=getPrevBlock().getHash();
        newBlock.mineBlock(difficulty);
        vchain.push_back(newBlock);

    }

private:
    uint32_t difficulty;
    vector<block> vchain;
    block getPrevBlock () const {
        return vchain.back();
    }

};

int main() {

    blockchain myChain= blockchain();

    cout<<"Mining Block "<<endl;
    myChain.addBlock(block(1,"First Block Data"));

}
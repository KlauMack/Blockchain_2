#include "blockchain.h"

// function to make text long enough for hashing (hash size - 64)
void makeTextLonger(Hash &key)
{
    if(key.getInput().length() < 60)
    {
        std::string filler (60, '!');
        std::string copy = key.getInput();
        filler.replace(0, copy.length(), copy);
        key.setFixedInput(filler);
    }
    else
    {
        std::string copy = key.getInput();
        key.setFixedInput(copy);
    }
}

// function to make hash
void hashAlgorithm(Hash &key)
{
    std::string copy = key.getFixedInput();
    std::wstring_convert<std::codecvt_utf8_utf16<char32_t>,char32_t> converter; // for lithuanian letters
    std::u32string input32 = converter.from_bytes(copy);

    // divide the whole text into 4 parts of 16 symbols each
    unsigned long long int sum[5] = {1};
    int part = 1;
    int index = 1;

    for(char32_t &character : input32)
    {
        if(part == 1)
        {
            sum[0] = (index + index) * sum[0] + character;
        }

        if(part == 2)
        {
            sum[1] = (index + index) * sum[1] + character;
        }

        if(part == 3)
        {
            sum[2] = (index + index) * sum[2] + character;
        }

        if(part == 4)
        {
            sum[3] = (index + index) * sum[3] + character;
            part = 0;
        }

        part++;
        index++;
    }
    // each part has 20 symbols(numbers).

    std::string hexa[5] = {""};

    for(int i = 0; i < 4; i++)
    {
        hexa[i] = intoHexa(sum[i]);

        while(hexa[i].length() < 16) // if length of the combination is lower than 16
        {
            hexa[i] += "f";
        }
    }

    std::string baseHash = hexa[0] + hexa[1] + hexa[2] + hexa[3]; // add all symbol combinations into one
    std::string shuffledHash = hashShuffle(baseHash, key);

    std::vector<char> symbols(shuffledHash.begin(), shuffledHash.end());
    int numberOf0s = 0;

    for (unsigned int i = 0; i < symbols.size(); i++)
    {
        if(symbols[i] == '0')
        {
            std::swap(symbols[i], symbols[numberOf0s]); // found '0' symbol changes places with the first element, second element and so on...
            numberOf0s++;
        }
    }

    std::string shuffledHash2(symbols.begin(), symbols.end());

    key.setOutput(shuffledHash2);
}

std::string intoHexa(unsigned long long int &index)
{
    std::stringstream ss;

    ss << std::hex << index;
    std::string result(ss.str()); // make number combination into string.

    return result;
}
 
 // function to make hash even more random-ish
std::string hashShuffle(std::string &baseHash, Hash &key)
{
    std::string copy = key.getInput();
    std::wstring_convert<std::codecvt_utf8_utf16<char32_t>,char32_t> converter;
    std::u32string input32 = converter.from_bytes(copy);

    int index = 1;
    int sum = 0;

    for(char32_t &character : input32)
    {
        sum = sum + character * index;
        index++;
    }

    unsigned seed = sum; // make shuffle seed by the sum of all symbols, specific characters and their places in the hash

    std::shuffle(baseHash.begin(), baseHash.end(), std::default_random_engine(seed));

    return baseHash;
}

// function for creating and calling hash functions
std::string createHash(std::string &input)
{
    Hash key;

    key.setInput(input);
    makeTextLonger(key);
    hashAlgorithm(key);

    return key.getOutput();
}

//function to generate merkle root.
std::string generateMerkleRoot( std::vector<Transaction> data)
{
    std::vector<std::string> merkel = {};
    std::vector<std::string> merkel2 = {};
    std::string word, hashed;

    for(unsigned int i = 0; i < data.size(); i++)
    {
        merkel.push_back( data[i].getID() );
    }

    while( merkel.size() > 1)
    {
        if( merkel.size() % 2 == 0)
        {
            for( unsigned i = 0; i < merkel.size(); i=i+2)
            {
                word = merkel[i] + merkel[i+1];
                hashed = createHash(word);
                merkel2.push_back(hashed);
            }
        }
        else
        {
            for( unsigned i = 0; i < merkel.size() - 1; i=i+2)
            {
                word = merkel[i] + merkel[i+1];
                hashed = createHash(word);
                merkel2.push_back(hashed);
            } 

            word = merkel[merkel.size()-1];
            hashed = createHash(word);
            merkel2.push_back(hashed);
        }

        merkel.clear();
        merkel = merkel2;
        merkel2.clear();
    }

    return merkel[0];
}
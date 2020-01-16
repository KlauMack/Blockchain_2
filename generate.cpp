#include "blockchain.h"

// function to generate users into file
void generateUsers(std::vector<User> &users)
{
    std::random_device rd;
        std::mt19937 rng(rd());
        std::uniform_real_distribution<double> uni(1000.0, 1000000.0);
    
    for(int i = 0; i < 1000; i++)
    {
        std::string name = "Vardas" + std::to_string(i);
        double amount = uni(rng);
        User user(name, amount);
        users.push_back(user);
    }

    std::ofstream outfile("Users.txt");

    for(unsigned int i = 0; i < users.size(); i++)
    {
        outfile << users[i].getName() << "\n";
        outfile << users[i].getPublicKey() << "\n";
        outfile << users[i].getBalance() << std::setprecision(7) << "\n";
        outfile << "\n";
    }
    outfile.close();
}

// function to generate transactions
void generateTransactions(std::vector<Transaction> &trans, std::vector<User> &users)
{
    std::random_device rd;
	    std::mt19937 rng(rd());
	    std::uniform_real_distribution<double> uni(10.0,100000.0);
        std::uniform_int_distribution<int> uni1(0, 999);
    
    double amount; // the amount of money that are in transaction
    int a, b; // index of users array
    std::string sender, receiver; // senders and receivers hashs

    for(int i = 0; i < 1000; i++)
    {
        amount = uni(rng);
        a = uni1(rng);
        b = uni1(rng);

        while(a == b)
        {
            b = uni1(rng);
        }

        sender = users[a].getPublicKey();
        receiver = users[b].getPublicKey();
        Transaction transaction(amount, sender, receiver);
        trans.push_back(transaction);
    }

    std::ofstream outfile("transactions.txt");

    for (unsigned int i = 0; i < trans.size(); i++)
    {
        outfile << trans[i].getAmount() << std::setprecision(7) << "\n";
        outfile << trans[i].getSenderKey() << "\n";
        outfile << trans[i].getReceiverKey() << "\n";
        outfile << "\n";
    }
    outfile.close();
}

// function to make multiple first blocks
void minerAmount(std::vector<Transaction> & trans, std::vector<Block> & miners){
    if( trans.size() >= 500)
    {
		Block a,b,c,d,e;
		miners.push_back(a);
		miners.push_back(b);
		miners.push_back(c);
		miners.push_back(d);
		miners.push_back(e);
	}
    if( trans.size() < 500 && trans.size() >= 400){
		Block a,b,c,d;
		miners.push_back(a);
		miners.push_back(b);
		miners.push_back(c);
		miners.push_back(d);
	}
    if( trans.size() < 400 && trans.size() >= 300){
		Block a,b,c;
		miners.push_back(a);
		miners.push_back(b);
		miners.push_back(c);
	}
    if( trans.size() < 300 && trans.size() >= 200){
		Block a,b;
		miners.push_back(a);
		miners.push_back(b);
	}
    if( trans.size() < 200 && trans.size() >= 100){
		Block a;
		miners.push_back(a);
	}
}

// function to mine new blocks
bool mine(Block & a)
{
    std::string b = "", difficulty = "";
    int switchCount = 0;

    for(int i = 0; i < a.diffTarget; i++)
    {
        difficulty = difficulty + "0";
    }

    for(unsigned int i = 0; i < a.data_.size(); i++)
    {
        for(unsigned int j = 0; j < a.data_.size(); i = j++)
        {
            
            if(switchCount >= a.nonce)
            {
                return false;
            }
            else
            {
                if( i != j)
                {
                    std::swap(a.data_[i], a.data_[j]);
                    a.hashBlock();
                    b = a.getHash();
                    switchCount++;
                    if(b.find(difficulty, 0) == 0 && b[a.diffTarget] != '0') // if the block has has needed numbers of 0s in the front.
                    {
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

void printChain( Blockchain &blockchain)
{
    std::ofstream outfile ("blockchain.txt");
    for(unsigned int i = 0; i < blockchain.getBlockchainSize(); i++)
    {
        outfile << blockchain.getBlockHash(i) << "\n";
        outfile << "\n";
    }
    outfile.close();
}

std::string create_merkle(std::vector<Transaction> data)
{
    // Stop if hash list is empty or contains one element
    if (data.empty())
        return 0;
    else if (data.size() == 1)
        return data.at[0];

    // While there is more than 1 hash in the list, keep looping...
    while (data.size() > 1)
    {
        // If number of hashes is odd, duplicate last hash in the list.
        if (data.size() % 2 != 0)
            data.push_back(data.back());
        // List size is now even.
        assert(data.size() % 2 == 0);

        // New hash list.
        std::vector<Transaction> new_data;
        std::string new_word;
        // Loop through hashes 2 at a time.
        for (unsigned int i = 0; i < data.size(); i + 2)
        {
            // Join both current hashes together (concatenate).
            new_word = data[i].getSenderKey() + data[i + 1].getSenderKey();
            // Hash both of the hashes.
            std::string new_root = createHash(new_word);
            // Add this to the new list.
            new_data.push_back(new_root);
        }
        // This is the new list.
        data = new_data;
    }
    // Finally we end up with a single item.
    return data.at[0];
}
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

// function to validate transactions
void validateTransactions(std::vector<Transaction> &trans, std::vector<User> &users, std::vector<Transaction> &validTrans)
{
    double newAmount = 0;
    std::string testID = "", stringAmmount = "", a ="", b = "";
    for(unsigned int i = 0; i < trans.size(); i++)
    {
        for(unsigned int j = 0; j < users.size(); j++)
        {
            if((trans[i].getSenderKey() == users[j].getPublicKey()) && (trans[i].getAmount() <= users[j].getBalance()))
            {
                for(unsigned int k = 0; k < users.size(); k++)
                {
                    if(trans[i].getReceiverKey() == users[k].getPublicKey())
                    {
                        stringAmmount = std::to_string(trans[i].getAmount());
                        a = users[j].getPublicKey();
                        b = users[k].getPublicKey();
                        testID = a + b + stringAmmount;
                        if(createHash( testID ) == trans[i].getID()){
                            validTrans.push_back(trans[i]);
                            newAmount = users[j].getBalance() - trans[i].getAmount();
                            users[j].setBalance( newAmount );
                            newAmount = users[k].getBalance() + trans[i].getAmount();
                            users[k].setBalance( newAmount );
                        }
                        break;
                    }
                }
                break;
            }
        }
    }

    std::ofstream outfile ("validTransactions.txt");
    for(unsigned int i = 0; i < validTrans.size(); i++)
    {
        outfile << validTrans[i].getAmount() << std::setprecision(6) << "\n";
        outfile << validTrans[i].getSenderKey() << "\n";
        outfile << validTrans[i].getReceiverKey() << "\n";
        outfile << "\n";
    }
    outfile.close();
}

// function to make multiple first blocks
void minerAmount(std::vector<Transaction> & validTrans, std::vector<Block> & miners){
    if( validTrans.size() >= 500)
    {
		Block a,b,c,d,e;
		miners.push_back(a);
		miners.push_back(b);
		miners.push_back(c);
		miners.push_back(d);
		miners.push_back(e);
	}
    if( validTrans.size() < 500 && validTrans.size() >= 400){
		Block a,b,c,d;
		miners.push_back(a);
		miners.push_back(b);
		miners.push_back(c);
		miners.push_back(d);
	}
    if( validTrans.size() < 400 && validTrans.size() >= 300){
		Block a,b,c;
		miners.push_back(a);
		miners.push_back(b);
		miners.push_back(c);
	}
    if( validTrans.size() < 300 && validTrans.size() >= 200){
		Block a,b;
		miners.push_back(a);
		miners.push_back(b);
	}
    if( validTrans.size() < 200 && validTrans.size() >= 100){
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
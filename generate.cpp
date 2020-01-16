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

void generateTransactions(std::vector<Transaction> &trans, std::vector<User> &users)
{
    std::random_device rd;
	    std::mt19937 rng(rd());
	    std::uniform_real_distribution<double> uni(10.0,100000.0);
        std::uniform_int_distribution<int> uni1(0, 999);
    
    double amount;
    int a, b;
    std::string sender, receiver;

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
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
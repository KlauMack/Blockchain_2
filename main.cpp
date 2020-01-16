#include "blockchain.h"

int main()
{
    std::vector<User> users;
    generateUsers(users);

    std::vector<Transaction> trans;
    generateTransactions(trans, users);
}
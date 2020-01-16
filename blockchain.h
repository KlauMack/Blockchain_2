#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <random>
#include <stdio.h>
#include <algorithm>
#include <bits/stdc++.h>
#include <chrono>

class Hash
{
    private:
        std::string input_;
        std::string fixedInput_;
        std::string output_;

    public:
        std::vector<int> values = {};

    Hash()
    {
        input_ = "";
        fixedInput_ = "";
        output_ = "";
    }

    Hash(std::string input)
    {
        input_ = input;
        fixedInput_ = "";
        output_ = "";
    }

    void setInput(std::string &input) { input_ = input; }
    void setFixedInput(std::string &fixed) { fixedInput_ = fixed; }
    void setOutput(std::string &output) { output_ = output; }
    std::string const getInput() { return input_; }
    std::string const getFixedInput() { return fixedInput_; }
    std::string const getOutput() { return output_; }
};

void makeTextLonger(Hash &key);
void hashAlgorithm(Hash &key);
std::string intoHexa(unsigned long long int &index);
std::string hashShuffle(std::string &baseHash, Hash &key);
std::string createHash(std::string &input);

class User
{
    private:
        std::string name_;
        double balance_;

    public:

        std::string publicKey_;

    User()
    {
        name_ = "";
        balance_ = 0;
        publicKey_ = "";
    };

    User(std::string name, double balance)
    {
        name_ = name;
        balance_ = balance;
        publicKey_ = createHash(name);
    }

    User(std::string name)
    {
        name_ = name;
        balance_ = 0;
        publicKey_ = createHash(name);
    }

    void setBalance(double balance) { balance_ = balance; }
    const double getBalance() { return balance_; }
    const std::string getName() { return name_; }
    const std::string getPublicKey() { return publicKey_; }
    ~User() {};
};

void generateUsers(std::vector<User> &users);

class Transaction
{
    private:
        std::string ID_;

    public:
        double amount_;
        std::string senderKey_;
        std::string receiverKey_;

        Transaction() {};

        Transaction(double amount, std::string senderKey, std::string receiverKey)
        {
            amount_ = amount;
            senderKey_ = senderKey;
            receiverKey_ = receiverKey;
        };

        const double getAmount() { return amount_; }
        const std::string getSenderKey() { return senderKey_; }
        const std::string getReceiverKey() { return receiverKey_; }
        ~Transaction() {};
};

void generateTransactions(std::vector<Transaction> &trans, std::vector<User> &users);
#endif
#include "blockchain.h"

int main()
{
    std::vector<User> users;
    generateUsers(users);

    std::vector<Transaction> trans;
    generateTransactions(trans, users);

    std::cout << "Type 0 to start the chain, type 1 to stop \n";
	std::string input;
	std::cin >>  input;

	while (input != "0" && input != "1")
    {
		std::cin.clear();
		std::cin.ignore();
		std::cout << "Please type 0 to start the chain, type 1 to stop \n";
		std::cin >> input;
	}

    if (input == "0")
    {
        Blockchain blockchain;

        while(input == "0")
        {
            if(trans.size() < 100)
            {
                std::cout << "Not enough transactions for new blocks! (should be 100) \n";
                printChain(blockchain);
                return 0;
            }

            std::vector<Block> miners = {};

            Block temp;
            miners.push_back(temp);

            for(unsigned int i = 0; i < miners.size() * 100; i = i + miners.size())
            {
                for(unsigned int j = 0; j < miners.size(); j++)
                {
                    miners[j].data_.push_back(trans[i+j]); // push transaction data into miners array
                }
            }

            std::cout << "How many 0s should there be in the start of a hash? \n";
            unsigned int input1;
            std::cin >> input1;

            for(unsigned int i = 0; i < miners.size(); i++)
            {
                miners[i].diffTarget = input1; // the number of 0s in the front of a hash.
                miners[i].nonce = 100; // allowed switches

                if(mine(miners[i]) == true)
                {
                    time_t time = std::time(nullptr);
                    miners[i].setTimeStamp(time);
                    blockchain.addBlock(miners[i]);
                    std::cout << "Generated block of a miner " << i << "\n";

                    break;
                }
                else
                {
                    std::cout << "miner " << i << " failed to generate a new block \n";
                }
            }

            std::cout<<"Would you like to continue? 0 - continue, 1 - stop \n";
            std::string input;
			std::cin >> input;

			while (input != "0" && input != "1")
            {
				std::cin.clear();
				std::cin.ignore();
				std::cout << "Would you like to continue? 0 - continue, 1 - stop \n";
				std::cin >> input;
            }

            if(input == "1")
            {
                break;
            }
        }
        printChain(blockchain);
    }
}
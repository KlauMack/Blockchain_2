#include "blockchain.h"

int main()
{
    std::vector<User> users;
    generateUsers(users);

    std::vector<Transaction> trans;
    generateTransactions(trans, users);

    std::vector<Transaction> validTrans;
	validateTransactions(trans, users, validTrans);

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
            if(validTrans.size() < 100)
            {
                std::cout << "Not enough transactions for new blocks! (should be 100) \n";
                printChain(blockchain);
                return 0;
            }

            std::vector<Block> miners = {};
            minerAmount(validTrans, miners);

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

            std::cout << "How many times can the program swap transactions inside the block? \n";
            unsigned int input2;
            std::cin >> input2;

            for(unsigned int i = 0; i < miners.size(); i++)
            {

                miners[i].diffTarget = input1; // the number of 0s in the front of a hash.
                miners[i].nonce = input2;

                if(mine(miners[i]) == true)
                {
                    time_t time = std::time(nullptr);
                    miners[i].setTimeStamp(time);
                    blockchain.addBlock(miners[i]);
                    std::cout << "Generated block of a miner " << i << "\n";

                    for( unsigned int j = 0; j < miners[i].data_.size(); j++) // checking and deleting changed transactions in the pool
                    {
						for( unsigned int k = 0; k < validTrans.size(); k++)
                        {
							if( miners[i].data_[j].getID() == validTrans[k].getID())
                            {
								validTrans.erase(validTrans.begin() + k);

                                break;
                            }
                        }
                    }
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
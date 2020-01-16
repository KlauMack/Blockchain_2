# Blockchain_2

Vilniaus universitetas

Antroji Blockchain užduotis

## Užduoties formuluotė

Sukurti "centralizuotą" blokų grandinę (blockchain'ą) ir susimuliuokite blokų grandinės veikimą kuo natūralesnėmis sąlygomis.

- Sugeneruoti ~1000 tinklo vartotojų (aka user'ių), kurie turėtų bent tris atributus: vardą, viešąjį hash raktą (public_key) ir tam tikros valiutos atsitiktinį balansą, pvz. nuo 100 iki 1000000 vienetų.
- Sugeneruoti tam tikrą skaičių, pvz., transkacijų pool'ą sudarytą iš 10000 atsitiktinių transakcijų tarp visų vartotojų, kurių metu jie vienas kitam atlikinėtų tam tikro atsitiktinio dydžio pinigų pervedimus.
- Iš šių transakcijų atsitiktinai pasirinkti 100-ą (tarsime, kad blokas talpins apie 100 transakcijų) ir jas priskirti naujam sugeneruotam blokui, kurio struktūra nurodyta paveiksle aukščiau.
- Realizuoti naujų blokų kasimo (angl. mining) Proof-of-Work (PoW) procesą, kurio tikslas yra surasti naujam blokui hash'ą, tenkinantį Difficulty Targer reikalavimą, t.y., hash'o pradžioje esančių nulių skaičių.
- Suradus tokį naujo bloko hash'ą, bloką pridėti prie grandinės.
- Kartoti 3-5 žingsnius tol, kol yra laisvų transakcijų.


## Įdiegimas (Unix kompiuteryje)

- `git clone https://github.com/KlauMack/Blockchain_2.git`
- `cd Blockchain_2`
- `make`
- `./main`

## Kaip naudotis?

- Pasirinkite, ar norite pradėti blockchain generavimą (įrašyti "0"), ar sustabdyti programą ("1");
- Jei pasirinkote pradėti blockchain generavimą:
  - Pasirinkite, kiek nulių turi būti hasho pradžioje;
  - Pasirinkite bandymų skaičių;

## Codes:

### User
```c++
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
```

### Transactions
```c++
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
            std::string stringAmount = std::to_string(amount);
            std::string temp = senderKey_ + receiverKey_ + stringAmount;
            ID_ = createHash(temp);
        };

        const double getAmount() { return amount_; }
        const std::string getSenderKey() { return senderKey_; }
        const std::string getReceiverKey() { return receiverKey_; }
        const std::string getID() { return ID_; }
        ~Transaction() {};
};

void generateTransactions(std::vector<Transaction> &trans, std::vector<User> &users);
void validateTransactions(std::vector<Transaction> &trans, std::vector<User> &users, std::vector<Transaction> &validTrans);
std::string generateMerkleRoot(std::vector<Transaction> data);
```

### Block
```c++
class Block
{
    private:
        std::string blockHash_;
        std::string previousHash_;
        std::string merkleRootHash_;
        time_t timestamp_;

    public:
        std::vector<Transaction> data_;
        int nonce, diffTarget;

        Block()
        {
            data_ = {};
            blockHash_ = "";
            previousHash_ = "";
            merkleRootHash_ = "";
            nonce = 0;
            timestamp_ = 0;
            diffTarget = 0;
        };

        Block(std::vector<Transaction> &data, std::string &previousHash)
        {
            previousHash_ = previousHash;
            data_ = data;
            merkleRootHash_ = generateMerkleRoot(data);
            std::string word = "";
            word = merkleRootHash_ + previousHash_; 
            blockHash_ = createHash(word);
            nonce = 0;
            timestamp_ = 0;
            diffTarget = 0;
        }

        void hashBlock()
        {
            merkleRootHash_ = generateMerkleRoot(data_);
            std::string word = "";
            word = merkleRootHash_ + previousHash_;
            blockHash_ = createHash(word);
        }

        void setTimeStamp(time_t &time) { timestamp_ = time; }
        std::string const getHash() { return blockHash_; }
        std::string const getPreviousHash() { return previousHash_; }
        ~Block() { data_.clear(); }
};
```

### Blockchain
```c++
class Blockchain
{
    private:
        std::vector<Block> chain_;
        void createGenesisBlock()
        {
            Block genesis;
            chain_.push_back(genesis);
        }

    public:
        Blockchain() { createGenesisBlock(); }
        const unsigned int getBlockchainSize() { return chain_.size(); }
        const std::string getBlockHash(int a) { return chain_[a].getHash(); }
        void addBlock(Block &a)
        {
            chain_.push_back(a);
        }
        ~Blockchain() { chain_.clear(); }
};
```
### Merkle generator
```c++
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
```
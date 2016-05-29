//Basic Spell Checker
//This program is a very basic spell checker, it takes in an input, checks if 
//the value exists in a hashtable -if not it finds possible rearrangments 
//and suggests them as possible 'replacements' 

//Including libraries
#include <iostream> 
#include <string>
#include <vector>
#include <iomanip>
#include<fstream>
#include <cctype> 
#include <algorithm> 
using namespace std;

//Setting up HashTable

struct Node { 

	string word; 
	Node* next; 


};

class HashTable { 
	public: 
		HashTable();
		HashTable(int K);
		virtual ~HashTable();
		void insert(string word);
		void remove(string word); //implement this
		bool lookup(string word) const; 
		void print() const;
		void report() const; 
		static const int DefaultSize;
		
	protected: 
		int getTableSize() const;
		
	private: 
		vector<Node*> table; 
		virtual int hash(string key) const = 0; 
}; 
		
const int HashTable::DefaultSize = 1000;
HashTable::HashTable() : table(DefaultSize) {}
HashTable::HashTable(int K) : table(K) {}

HashTable::~HashTable(){
    for (int i=0; i<getTableSize(); i++) 
	{
		Node* p = table[i];
		while (p!=NULL) 
		{
			Node* temp = p;
			p = p->next;
			delete temp;
		}
    }
}

int HashTable::getTableSize() const 
{
    return (int) table.size();
}

void HashTable::insert(string key) 
{
    const int slot = hash(key);
    Node* newNode = new Node;
    newNode->word = key;
    newNode->next = table[slot];
    table[slot] = newNode;
}

bool HashTable::lookup (string key) const 
{
    const int slot = hash(key);
    Node* curNode = table[slot];
    while (curNode != NULL) {
	if (curNode->word == key) {
	    return true;
	}
	curNode = curNode -> next;
    }
    return false;
}

void HashTable::remove(string key) 
{
	
	int location = hash(key); 
	Node* head = table[location]; 
	if(head == NULL) { 
		//nothing
	
	}
	
	if(head->word == key) { //if first value in list contains the key
		
		Node* temp = head->next;
		table[location] = temp; 
		delete head; 

	}
	
	//If it's not the first one in the list we have to cycle through until we find out
	Node* temp = head; //to cycle through 
	while(temp->next!= NULL && temp->next->word!=key) { 
		//finding the value in the list
		temp = temp->next; 

	
	}
	
	//once we find the value
	
	if(temp->next!=NULL) { 
	
		Node* s = temp->next;
		
		temp->next = s->next; 
		delete s; 
		
	
	}

	
}

class SimpleHashTable: public HashTable
{
	public:
		SimpleHashTable (); //Constructor
		SimpleHashTable (int K); 
		virtual ~SimpleHashTable(); //Destructor
	private:
		virtual int hash (string key) const; //Virtual hash function 
};

SimpleHashTable::SimpleHashTable(): HashTable() 
	
{
}
SimpleHashTable::SimpleHashTable(int K): HashTable(K)  
	
{
}
SimpleHashTable::~SimpleHashTable()
{
}
int SimpleHashTable::hash (string key) const 
{ 
	int bucketNumber = getTableSize();
	int sum = 0; 
	//convert to lowercase
	
	
	for(int i=0; i < key.length(); i++) { 
		if(isalpha(key[i])) { 
			sum +=(int)key[i]; 
		} 
	}
	

	
	return sum%bucketNumber; 
}

//smart hash function declaration
//more efficient than simple hash


class SmartHashTable: public HashTable
{
public:
	SmartHashTable ();
	SmartHashTable (int K);
	virtual ~SmartHashTable();
private:
	virtual int hash (string key) const;
};

SmartHashTable::SmartHashTable() 
	: HashTable()
{
}
SmartHashTable::SmartHashTable(int K) 
	: HashTable(K) 
{
}
SmartHashTable::~SmartHashTable()
{
}
int SmartHashTable::hash (string key) const
{
	int bucketNumber = getTableSize();
	unsigned int sum = 13; 
	
	for(int i = 0; i < key.length(); i++) { 
				sum = (sum << 6);
		        sum ^= key[i]; 
		        sum ^= (sum >> 4);
		        sum ^= (sum >> 7);
				sum ^=(sum << 5);
				sum += (sum << 3 | sum >> 8);
		
	
	
	
	}
	
	return sum%bucketNumber; 
    
}
// Ancillary function for powerset.  It adds a character onto the beginning
// of each string in a vector.
vector<string> addChar (const vector<string>& v, char c) {
    vector<string> ans;
    for (int i=0; i<(int)v.size() ; i++) {
	ans.push_back(c + v.at(i));
    }
    return ans;
}

vector<string> powerset (string s) {
    vector<string> ans;
    if (s.size() == 0) {
	ans.push_back("");
    } else {
	char c = s.at(0);
	string rest = s.substr(1);
	vector<string> psetRest = powerset (rest);
	ans = addChar (psetRest, c);
	ans.insert(ans.end(), psetRest.begin(), psetRest.end());
    }
    return ans;
}


//Helper, if letters aren't lowercase it won't read properly
string convert(string value) { 

	
	for(int k=0; k<value.length(); k++) { 
		value[k] = tolower(value[k]);
	
	}
	return value; 

} 
///Reads from the wordlist passed as a command line argument
//inserts all words from list to hashtable to use as reference
//user may input words and program will tell if it is spelled correctly or give
//suggestions if it is not
int main(int argc, char* argv[]) { 
	
	//create hash function
	SmartHashTable hashTable(100000); 
	
	string wordlistFileName = "";
	
	if(argv[1] != NULL) { 
		wordlistFileName = argv[1]; 
	} 
	
	
	if(wordlistFileName=="") { //if no value provided
		cerr << "Error, no word list file name provided." << endl;
		return -1; //abort
	
	}
	
	//Building Hashtable
	ifstream myfile;
	myfile.open (wordlistFileName, ios::in);
	
	if(myfile.is_open()) { 

		string word;

		while(myfile>>word) { 
			if(!(myfile.fail()) &&  word!="EOF" && word!="eof") { 
				hashTable.insert(word); //insert into the hash function 
			}

			else { 
				break; //If invalid input-break

			}


		}

	}
	else { //if file won't open 
		cerr << "Error, couldn't open word list file."<<endl;
		
		return -1;

	}
	
	myfile.close(); //close the file
	
	//Collecting words
	string value; 
	
	while(cin >> value) { 
		//compute powerset of value
		
		value = convert(value);
		if(hashTable.lookup(value)) { 
			cout << value << " is spelled correctly." << endl; 
		
		}
		
		//find possible permutations per entry in powerset
		else { 
			string s = value; 
			cout << s << " is spelled incorrectly." << endl;
			vector<string> wordCombinations = powerset(value); 
			for(int i=0; i<wordCombinations.size(); i++) {
				
				s = wordCombinations[i]; 
				if(hashTable.lookup(s)) { 
			
					cout << "Did you mean? " << s << endl; 
		
				}
			
					sort(s.begin(), s.end());
					do { 
						if(hashTable.lookup(s)) {  //only if valid value 
							cout << "Did you mean? " << s << endl; 
						
				
						}
			
					} while(next_permutation(s.begin(), s.end()));
			
				} 
				
			} 
	
		}
	
	return 0; 
} 

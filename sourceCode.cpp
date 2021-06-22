// Name : TRIJAL BHARDWAJ
// GitHub Repository Link/URL - https://github.com/Trijal-Bhardwaj/SummerOfBitCoin/
// Time Complexity  - O(n^2)

#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

string inputFile = "mempool.csv"; // Input File
set<string> includedTransactionIDs;
int maximumBlockWeight = 4000000; // Maximum Value of Total Weight Of Transactions in a Block

class TransactionValue
{
public:
  string tx_id;
  int fee;
  int weight; // To Store Individual Data of Input
  vector<string> parent;
};

pair<string, TransactionValue *> addTransactionValue(vector<string> &transactionValue)
{
  auto transaction = new TransactionValue(); // transaction is the NEW TransactionValue Object
  transaction->tx_id = transactionValue[0];
  transaction->fee = stoi(transactionValue[1]); // String Input
  transaction->weight = stoi(transactionValue[2]);
  vector<string> parentID;
  for (int i = 3; i < transactionValue.size(); i++)
  {
    parentID.push_back(transactionValue[i]);
  }
  transaction->parent = parentID;
  return {
      transactionValue[0], transaction};
}

// Reading the mempool.csv File Input
void readCSV(string filename, unordered_map<string, TransactionValue *> &transaction)
{ // O(1)

  ifstream fin(inputFile);
  vector<string> transactionDetails;
  string temp, line, word;
  getline(fin, line);
  while (getline(fin, line))
  {
    transactionDetails.clear();
    stringstream s(line);
    while (getline(s, word, ','))
    {
      transactionDetails.push_back(word);
    }
    pair<string, TransactionValue *> p = addTransactionValue(transactionDetails);
    transaction[p.first] = p.second;
  }
  fin.close();

  cout << "Total Transactions Read: " << transaction.size() << endl;
}

void Out(vector<string> &output, string filename)
{
  ofstream myfile(filename);
  for (auto s : output)
  {
    myfile << s << endl;
  }
  myfile.close();
}

bool isValidTransaction(TransactionValue *transaction, set<string> &includedTransactionIDs)
{ // For Checking If All Parents Are Already Present in the Block or Not
  for (auto parent : transaction->parent)
  {
    if (includedTransactionIDs.find(parent) == includedTransactionIDs.end())
      return false;
  }
  return true;
}

int main()
{
  unordered_map<string, TransactionValue *> transaction;
  readCSV(inputFile, transaction);
  set<pair<float, TransactionValue *>, greater<pair<float, TransactionValue *>>> transactions;
  set<string> Transaction_included;
  vector<string> output; // OUTPUT
  for (auto index : transaction)
  {
    transactions.insert({(float)index.second->fee / (float)index.second->weight, index.second}); // FEE/WEIGHT RATIO (REFERENCE TAKEN FROM FRACTIONAL KNAPSACK LEETCODE)
  }
  int currentBlockWeight = 0;
  int totalFee = 0;
  while (!transactions.empty() && (currentBlockWeight < maximumBlockWeight))
  {
    bool flag = false;
    for (auto iteration = transactions.begin(); iteration != transactions.end(); iteration++)
    {
      TransactionValue *currentTransaction = (*iteration).second;
      int currentFee = currentTransaction->fee;
      int currentWeight = currentTransaction->weight;
      if (isValidTransaction(currentTransaction, includedTransactionIDs) && currentBlockWeight + currentWeight <= maximumBlockWeight)
      {
        currentBlockWeight += currentWeight;
        Transaction_included.insert(currentTransaction->tx_id); // Updation
        output.push_back(currentTransaction->tx_id);
        totalFee += currentFee;
        transactions.erase(iteration); // Erasing From the Current Set
        flag = true;
        break;
      }
    }
    if (!flag)
    {
      break;
    }
  }
  cout << "Total Numbers Of Transactions : " << Transaction_included.size() << endl;
  cout << "Total Fee in the Current Block : " << totalFee << endl;
  cout << "Total Weight in the Current Block : " << currentBlockWeight << endl;
  Out(output, "block.txt");

  return 0;
}
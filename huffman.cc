// Abby Leary
// 11/7/2022
// CS 3610
// Homework 4: Huffman Code Tree

#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <vector>
#include "node.h"

using namespace std;

/**
 *  Function:   usage
 *              prints usage
 */
void usage();

/**
 *  Function:   fileFail
 *              finds whether or not the file is able to be opened
 *
 *  @param in - the input file stream
 *  @param out - the output file stream
 *  @param argv - the array of arguments
 *  @return - true if the file fails, false if it opens correctly
 */
bool fileFail(ifstream &in, ofstream &out, char *argv[]);

/**
 *  Function:   compareProb
 *              compares the probabilities in two nodes
 *
 *  @param n1 - the first node to be compared
 *  @param n2 - the second node to be compared
 *  @return - true if the probability of n1 is greater than the probability of n2, false otherwise
 */
bool compareProb(node *n1, node *n2);

/**
 *  Function:   compareLetter
 *              compares the letters in two pairs
 *
 *  @param p1 - the first pair to be compared
 *  @param p2 - the second pair to be compared
 *  @return - true if the letter in p1 is greater than the letter in p2, false otherwise
 */
bool compareLetter(pair<char, string> p1, pair<char, string> p2);

/**
 *  Function:   compareCodeSizeMin
 *              compares the size of codes in two pairs
 *
 *  @param n1 - the first pair to be compared
 *  @param n2 - the second pair to be compared
 *  @return - true if the size of the code in p1 is smaller than the size of the code in p2, false otherwise
 */
bool compareCodeSizeMin(pair<char, string> p1, pair<char, string> p2);

/**
 *  Function:   compareCodeSizeMax
 *              compares the size of codes in two pairs
 *
 *  @param n1 - the first pair to be compared
 *  @param n2 - the second pair to be compared
 *  @return - true if the size of the code in p1 is larger than the size of the code in p2, false otherwise
 */
bool compareCodeSizeMax(pair<char, string> p1, pair<char, string> p2);

/**
 *  Function:   createCodeVect
 *              creates a vector of pairs of codes and the corresponding letter from
 *              the huffman code tree
 *
 *  @param root - the root of the huffman tree
 *  @param alphaCode - the vector storing the pairs
 *  @param code - the code for one letter
 */
void createCodeVect(node *root, vector<pair<char, string>> &alphaCode, string code);

/**
 *  Function:   averageCodeLength
 *              finds the average code length in a vector
 *
 *  @param code - the vector of pairs holding the codes
 *  @return - the average code length
 */
int averageCodeLength(vector<pair<char, string>> &alphaCode);

int main(int argc, char *argv[])
{

    // if there is no argument or the argument is "-h", print usage
    if (argc == 1 || ((argv[1])[1] == 'h'))
    {
        usage();
        return 0;
    }

    ifstream in;
    ofstream out;
    vector<node *> v;
    string prob;

    // open file of alphabet and check for failure
    in.open("alpha.txt");
    if (in.fail())
    {
        cout << "Alphabet file unable to be opened.";
        return -1;
    }

    // load alphabet file into a vector of nodes
    while (!in.eof())
    {
        node *n = new node;
        in >> n->letter;
        getline(in, prob);
        while (prob[0] != '.')
        {
            prob.erase(0, 1);
        }
        n->probability = stod(prob);
        if (in.peek() != in.eof())
        {
            v.push_back(n);
        }
    }
    in.close();

    // sort vector of nodes from highest to lowest
    sort(v.begin(), v.end(), compareProb);

    int alphaLength = v.size();

    // create the huffman code tree and store root in vector of size 1
    while (v.size() > 1)
    {
        node *insert = new node;
        insert->probability = (v[v.size() - 1]->probability + v[v.size() - 2]->probability);
        insert->left = v[v.size() - 1];
        insert->right = v[v.size() - 2];
        insert->isLeaf = false;
        v.pop_back();
        v.pop_back();
        v.push_back(insert);
        sort(v.begin(), v.end(), compareProb);
    }

    vector<pair<char, string>> vect;
    createCodeVect(v[0], vect, "");
    string line;
    vector<char> characters;
    vector<string> codes;

    switch ((argv[1])[1])
    {
    case 'p':
        // check for correct amount of arguments
        if (argc != 2)
        {
            cout << "Incorrect amount of arguments. Use \"h\" for help." << endl;
            return -1;
        }

        sort(vect.begin(), vect.end(), compareLetter);

        // print all letters and codes
        for (int i = 0; i < alphaLength; i++)
        {
            cout << vect[i].first << ": " << vect[i].second << endl;
        }

        cout << endl
             << "The average code length is " << averageCodeLength(vect) << endl;

        break;

    case 'd':

        // check for correct amount of arguments
        if (argc != 4)
        {
            cout << "Incorrect amount of arguments. Use \"h\" for help." << endl;
            return -1;
        }

        // check to make sure both files open
        if (fileFail(in, out, argv))
        {
            return -1;
        }

        // get the codes from file and push into a vector
        while (!in.eof())
        {
            getline(in, line);
            codes.push_back(line + '\n');
        }

        sort(vect.begin(), vect.end(), compareCodeSizeMax);

        // walk through vector of codes to decode
        for (int j = 0; j < codes.size();)
        {
            while (!codes[j].empty())
            {
                // while the code contains a character besides 0 and 1, print the character to the file
                while (codes[j][0] != '0' && codes[j][0] != '1' && !codes[j].empty())
                {
                    out << codes[j][0];
                    codes[j].erase(0, 1);
                }

                // go through the vector of alphabet codes and see if the letters match the code
                for (int i = 0; i < vect.size(); i++)
                {
                    string substr = codes[j].substr(0, (vect[i].second).length());
                    if (substr == vect[i].second)
                    {
                        // if the code is found, print the corresponding letter to file
                        out << vect[i].first;
                        codes[j].erase(0, (vect[i].second).length());
                    }
                }
            }
            j++;
        }

        in.close();
        out.close();
        break;

    case 'e':

        // check for correct amount of arguments
        if (argc != 4)
        {
            cout << "Incorrect amount of arguments. Use \"h\" for help." << endl;
            return -1;
        }

        // make sure files are able to be opened
        if (fileFail(in, out, argv))
        {
            return -1;
        }

        // create a vector of all characters in the file
        while (!in.eof())
        {
            getline(in, line);
            for (int i = 0; i < line.size(); i++)
            {
                characters.push_back(tolower(line[i]));
            }
            characters.push_back('\n');
        }

        sort(vect.begin(), vect.end(), compareCodeSizeMin);

        for (int i = 0; i < characters.size(); i++)
        {
            // if the character is not a letter, print the character to file
            if (!isalpha(characters[i]))
            {
                out << characters[i];
            }

            // otherwise put the corresponding code for each letter into the file
            else
            {
                for (int j = 0; j < vect.size(); j++)
                {
                    if (characters[i] == vect[j].first)
                    {
                        out << vect[j].second;
                    }
                }
            }
        }

        in.close();
        out.close();
        break;

    default:
        cout << "Invalid argument. Use \"h\" for help." << endl;
        return -1;
    }
}

void usage()
{
    cout << endl
         << "huffman.cc: uses a Huffman coding tree to encode and decode text files." << endl
         << endl;
    cout << "Usage: [option] [input file] [output file]" << endl;
    cout << "[-e input.txt output.txt] (encodes entire input file and puts results in output file)" << endl;
    cout << "[-d input.txt output.txt] (decodes entire input file and puts results in output file)" << endl;
    cout << "[-p] (prints the codes of all letters and the average code length)" << endl;
    cout << "[-h] (prints the help menu)" << endl;
}

bool fileFail(ifstream &in, ofstream &out, char *argv[])
{
    in.open(argv[2]);
    out.open(argv[3]);
    if (in.fail())
    {
        cout << "Input file unable to be opened." << endl;
        return true;
    }
    if (out.fail())
    {
        cout << "Output file unable to be opened." << endl;
        return true;
    }
    return false;
}

bool compareProb(node *n1, node *n2)
{
    return (n1->probability > n2->probability);
}

void createCodeVect(node *root, vector<pair<char, string>> &alphaCode, string code)
{
    if (root == nullptr)
    {
        return;
    }

    // if there is a node on the left, increase the code by 0
    if (root->left != nullptr)
    {
        createCodeVect(root->left, alphaCode, code + "0");
    }

    // if there is a node on the right, increase the code by 1
    if (root->right != nullptr)
    {
        createCodeVect(root->right, alphaCode, code + "1");
    }

    // if the root has a letter, put the code in the vector
    if (root->isLeaf == true)
    {
        alphaCode.push_back(pair<char, string>(root->letter, code));
    }
}

bool compareLetter(pair<char, string> p1, pair<char, string> p2)
{
    return (p1.first < p2.first);
}

bool compareCodeSizeMin(pair<char, string> p1, pair<char, string> p2)
{
    return ((p1.second).length() < (p2.second).length());
}

bool compareCodeSizeMax(pair<char, string> p1, pair<char, string> p2)
{
    return ((p1.second).length() > (p2.second).length());
}

int averageCodeLength(vector<pair<char, string>> &alphaCode)
{
    int totalLength = 0;
    for (int i = 0; i < alphaCode.size(); i++)
    {
        totalLength += alphaCode[i].second.length();
    }
    return (totalLength / (alphaCode.size()));
}
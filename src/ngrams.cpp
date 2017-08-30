#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "simpio.h"
#include "strlib.h"
#include "map.h"
#include "vector.h"
#include "filelib.h"
#include "random.h"

using namespace std;

void printInstructions();

void buildMapFromFile(Map<Vector<string>, Vector<string>> &nGrams, int &n);

void generateNGram(Map<Vector<string>, Vector<string>> &nGrams, int &n);

bool isInteger(string &input);

int main() {
    srand(int(time(NULL)));
    Map<Vector<string>, Vector<string>> nGrams;
    int n;
    printInstructions();
    buildMapFromFile(nGrams, n);
    generateNGram(nGrams, n);
    cout << "Exiting." << endl;
    return 0;
}

void printInstructions() {
    cout << "Welcome to CS 106B Random Writer ('N-Grams')." << endl;
    cout << "This program makes random text based on a document." << endl;
    cout << "Give me an input file and an 'N' value for groups" << endl;
    cout << "of words, and I'll create random text for you." << endl;
    cout << endl;
}

void buildMapFromFile(Map<Vector<string>, Vector<string>> &nGrams, int &n) {
    string fileName;
    while (true) {
        fileName = getLine("Input file name? ");
        if (fileExists(fileName)) break;
        cout << "Unable to open that file. Try again." << endl;
    }
    ifstream infile;
    infile.open(fileName.c_str());
    string text = "";
    while (true) {
        string line;
        getline(infile, line);
        if (infile.fail()) break;
        text += line + " ";
    }
    infile.close();
    istringstream stream(text);
    Vector<string> words;
    int wordCount = 0;
    while (true) {
        string word;
        stream >> word >> ws;
        if (stream.fail()) break;
        words.add(word);
        wordCount++;
    }
    stream.clear();
    string nInput;
    while (true) {
        nInput = getLine("Value of N? ");
        if (isInteger(nInput)) {
            n = stringToInteger(nInput);
            if (n < 2 || n > wordCount) {
                if (n < 2) {
                    cout << "N must be 2 or greater." << endl;
                } else {
                    cout << "N must be not be greater than the number of words in the file." << endl;
                }
            } else {
                break;
            }
        } else {
            cout << "Illegal integer format. Try again." << endl;
        }
    }
    for (int i = 0; i < n - 1; i++) {
        words.add(words.get(i));
    }
    for (int i = 0; i < wordCount; i++) {
        Vector<string> prefix;
        Vector<string> suffixes;
        for (int j = 0; j < n - 1; j++) {
            prefix.add(words.get(i + j));
        }
        if (nGrams.containsKey(prefix)) {
            suffixes = nGrams.get(prefix);
        }
        suffixes.add(words.get(i + n - 1));
        nGrams.put(prefix, suffixes);
    }
}

void generateNGram(Map<Vector<string>, Vector<string>> &nGrams, int &n) {
    int numWords;
    while (true) {
        string numWordsInput = getLine("# of random words to generate (0 to quit)? ");
        if (isInteger(numWordsInput)) {
            numWords = stringToInteger(numWordsInput);
            if (numWords < n && numWords != 0) {
                cout << "Must be at least " << n << " words." << endl;
            } else {
                break;
            }
        } else {
            cout << "Illegal integer format. Try again." << endl;
        }
    }
    if (numWords != 0) {
        Vector<Vector<string>> prefixes;
        for (Vector<string> prefix: nGrams.keys()) {
            prefixes.add(prefix);
        }
        Vector<string> nGram;
        Vector<string> prefix = prefixes.get(randomInteger(0, prefixes.size() - 1));
        nGram = prefix;
        Vector<string> suffixSet = nGrams.get(prefix);
        string suffix = suffixSet.get(randomInteger(0, suffixSet.size() - 1));
        nGram += suffix;
        prefix = nGram;
        for (int i = 1; i <= numWords - n; i++) {
            for (int j = 0; j < i; j++) {
                prefix.remove(0);
            }
            suffixSet = nGrams.get(prefix);
            suffix = suffixSet.get(randomInteger(0, suffixSet.size() - 1));
            nGram += suffix;
            prefix = nGram;
        }
        string text = "... ";
        for (int i = 0; i < nGram.size(); i++) {
            text += nGram.get(i) + " ";
        }
        text += " ...";
        cout << text << endl;
    }
}

bool isInteger(string &input) {
    bool b = true;
    for (int i = 0; i < input.length(); i++) {
        if (!isdigit(input[i]) && !(i == 0 && input[i] == '-')) {
            b = false;
            break;
        }
    }
    return b;
}

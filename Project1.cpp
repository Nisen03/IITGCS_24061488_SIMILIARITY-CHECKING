#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <cmath>

using namespace std;

set<string> stopWords = {"a", "and", "an", "of", "in", "the"};

void normalizeWord(string &word) {
    transform(word.begin(), word.end(), word.begin(), ::toupper);
    if (stopWords.find(word) != stopWords.end())
        word = "";
}

map<string, int> getWordFrequency(const string &filename) {
    ifstream file(filename);
    map<string, int> wordCount;
    string word;
    while (file >> word) {
        normalizeWord(word);
        if (!word.empty())
            wordCount[word]++;
    }
    return wordCount;
}

double calculateSimilarity(const map<string, int> &freq1, const map<string, int> &freq2) {
    double sum = 0;
    for (const auto &pair : freq1) {
        if (freq2.find(pair.first) != freq2.end())
            sum += sqrt(pair.second * freq2.at(pair.first));
    }
    return sum;
}

vector<string> getFilesList() {
    vector<string> files;
    for (int i = 1; i <= 64; i++) {
        files.push_back("file" + to_string(i) + ".txt");
    }
    return files;
}

int main() {
    vector<string> files = getFilesList();
    vector<vector<double>> similarityMatrix(64, vector<double>(64, 0));

    for (int i = 0; i < 64; i++) {
        map<string, int> freq1 = getWordFrequency(files[i]);
        for (int j = i + 1; j < 64; j++) {
            map<string, int> freq2 = getWordFrequency(files[j]);
            double similarity = calculateSimilarity(freq1, freq2);
            similarityMatrix[i][j] = similarity;
            similarityMatrix[j][i] = similarity;
        }
    }

    vector<pair<double, pair<int, int>>> similarities;
    for (int i = 0; i < 64; i++) {
        for (int j = i + 1; j < 64; j++) {
            similarities.push_back({similarityMatrix[i][j], {i, j}});
        }
    }
    sort(similarities.rbegin(), similarities.rend());

    for (int i = 0; i < 10; i++) {
        cout << "Pair: " << similarities[i].second.first + 1 << " and " << similarities[i].second.second + 1;
        cout << " - Similarity: " << similarities[i].first << endl;
    }

    return 0;
}

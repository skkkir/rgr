#include "beaufort.h"
#include "file.h"
#include <sstream>
#include <iomanip>
#include <iostream>
#include <random>

using namespace std;

string genRandomKey(size_t length) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 255);
    
    string key;
    for (size_t i = 0; i < length; ++i) {
        key += static_cast<char>(dis(gen));
    }
    return key;
}

string beaufortProcess(const string& data, const string& key) {
    vector<unsigned char> result;
    size_t keyLen = key.length();
    
    if (keyLen == 0) {
        return data;
    }
    
    for (size_t i = 0; i < data.length(); ++i) {
        unsigned char p = data[i];
        unsigned char k = key[i % keyLen];
        unsigned char c = (k - p) % 256;
        result.push_back(c);
    }
    
    return string(result.begin(), result.end());
}

string bytesToHex(const vector<unsigned char>& data) {
    stringstream ss;
    for (unsigned char byte : data) {
        ss << hex << setw(2) << setfill('0') << static_cast<int>(byte);
    }
    return ss.str();
}

void saveKeyToFile(const string& key, const string& keyFile) {
    vector<unsigned char> keyVec(key.begin(), key.end());
    writeFile(keyFile, keyVec);
}

string loadKeyFromFile(const string& keyFile) {
    auto keyVec = readFile(keyFile);
    return string(keyVec.begin(), keyVec.end());
}

extern "C" {
    void beaufortEncFile(const string& inputFile, const string& outputFile, const string& keyFile) {
        auto data = readFile(inputFile);
        string dataStr(data.begin(), data.end());
        
        string key = genRandomKey(32);
        saveKeyToFile(key, keyFile);
        
        string result = beaufortProcess(dataStr, key);
        vector<unsigned char> resultVec(result.begin(), result.end());
        writeFile(outputFile, resultVec);
    }

    void beaufortDecFile(const string& inputFile, const string& outputFile, const string& keyFile) {
        auto data = readFile(inputFile);
        string dataStr(data.begin(), data.end());
        
        string key = loadKeyFromFile(keyFile);
        string result = beaufortProcess(dataStr, key);
        vector<unsigned char> resultVec(result.begin(), result.end());
        writeFile(outputFile, resultVec);
    }

    string beaufortEncText(const string& text, const string& keyFile) {
        string key = genRandomKey(32);
        saveKeyToFile(key, keyFile);
        
        string result = beaufortProcess(text, key);
        vector<unsigned char> resultVec(result.begin(), result.end());
        return bytesToHex(resultVec);
    }

    string beaufortDecText(const string& hexText, const string& keyFile) {
        string key = loadKeyFromFile(keyFile);
        
        vector<unsigned char> data;
        for (size_t i = 0; i < hexText.length(); i += 2) {
            string byteStr = hexText.substr(i, 2);
            data.push_back(static_cast<unsigned char>(stoul(byteStr, nullptr, 16)));
        }
        
        string dataStr(data.begin(), data.end());
        return beaufortProcess(dataStr, key);
    }
}

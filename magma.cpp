#include "magma.h"
#include "file.h"
#include <cstdint>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <random>

using namespace std;

const unsigned char magmaSbox[8][16] = {
    {0xC, 0x4, 0x6, 0x2, 0xA, 0x5, 0xB, 0x9, 0xE, 0x8, 0xD, 0x7, 0x0, 0x3, 0xF, 0x1},
    {0x6, 0x8, 0x2, 0x3, 0x9, 0xA, 0x5, 0xC, 0x1, 0xE, 0x4, 0x7, 0xB, 0xD, 0x0, 0xF},
    {0xB, 0x3, 0x5, 0x8, 0x2, 0xF, 0xA, 0xD, 0xE, 0x1, 0x7, 0x4, 0xC, 0x9, 0x6, 0x0},
    {0xC, 0x8, 0x2, 0x1, 0xD, 0x4, 0xF, 0x6, 0x7, 0x0, 0xA, 0x5, 0x3, 0xE, 0x9, 0xB},
    {0x7, 0xF, 0x5, 0xA, 0x8, 0x1, 0x6, 0xD, 0x0, 0x9, 0x3, 0xE, 0xB, 0x4, 0x2, 0xC},
    {0x5, 0xD, 0xF, 0x6, 0x9, 0x2, 0xC, 0xA, 0xB, 0x7, 0x8, 0x1, 0x4, 0x3, 0xE, 0x0},
    {0x8, 0xE, 0x2, 0x5, 0x6, 0x9, 0x1, 0xC, 0xF, 0x4, 0xB, 0x0, 0xD, 0xA, 0x3, 0x7},
    {0x1, 0x7, 0xE, 0xD, 0x0, 0x5, 0x8, 0x3, 0x4, 0xF, 0xA, 0x6, 0x9, 0xC, 0xB, 0x2}
};


string generateRandomKey(size_t length) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 255);
    
    string key;
    for (size_t i = 0; i < length; ++i) {
        key += static_cast<char>(dis(gen));
    }
    return key;
}


uint32_t magmaF(uint32_t A, uint32_t X) {
    uint32_t sum = (A + X) & 0xFFFFFFFF;
    
    uint32_t res = 0;
    for (int i = 0; i < 8; ++i) {
        unsigned char nibble = (sum >> (4 * i)) & 0xF;
        unsigned char substituted = magmaSbox[i][nibble];
        res |= (uint32_t(substituted) << (4 * i));
    }
    

    res = ((res << 11) | (res >> (32 - 11))) & 0xFFFFFFFF;
    
    return res;
}

vector<uint32_t> magmaExpKey(const string& key, bool forEnc) {
    vector<uint32_t> roundKeys(32);
    
 
    vector<uint32_t> K(8);
    for (int i = 0; i < 8; ++i) {
        K[i] = (uint32_t(key[i * 4 + 0]) << 24) |
               (uint32_t(key[i * 4 + 1]) << 16) |
               (uint32_t(key[i * 4 + 2]) << 8) |
               (uint32_t(key[i * 4 + 3]));
    }
    
    if (forEnc) {
        
        for (int i = 0; i < 24; ++i) {
            roundKeys[i] = K[i % 8];
        }
        for (int i = 0; i < 8; ++i) {
            roundKeys[24 + i] = K[7 - i];
        }
    } else {
        
        for (int i = 0; i < 8; ++i) {
            roundKeys[i] = K[i];
        }
        for (int i = 0; i < 24; ++i) {
            roundKeys[8 + i] = K[7 - (i % 8)];
        }
    }
    
    return roundKeys;
}


vector<unsigned char> magmaEncBlock(const vector<unsigned char>& block, const vector<uint32_t>& roundKeys) {
    uint32_t A = (uint32_t(block[3]) << 24) | (uint32_t(block[2]) << 16) | (uint32_t(block[1]) << 8) | uint32_t(block[0]);
    uint32_t B = (uint32_t(block[7]) << 24) | (uint32_t(block[6]) << 16) | (uint32_t(block[5]) << 8) | uint32_t(block[4]);
    
    for (int round = 0; round < 32; ++round) {
        uint32_t tempA = A;
        A = B ^ magmaF(A, roundKeys[round]);
        B = tempA;
    }

    vector<unsigned char> res(8);
    res[0] = (A >> 0) & 0xFF;
    res[1] = (A >> 8) & 0xFF;
    res[2] = (A >> 16) & 0xFF;
    res[3] = (A >> 24) & 0xFF;
    res[4] = (B >> 0) & 0xFF;
    res[5] = (B >> 8) & 0xFF;
    res[6] = (B >> 16) & 0xFF;
    res[7] = (B >> 24) & 0xFF;
    
    return res;
}


vector<unsigned char> magmaDecBlock(const vector<unsigned char>& block, const vector<uint32_t>& roundKeys) {
    uint32_t A = (uint32_t(block[3]) << 24) | (uint32_t(block[2]) << 16) | (uint32_t(block[1]) << 8) | uint32_t(block[0]);
    uint32_t B = (uint32_t(block[7]) << 24) | (uint32_t(block[6]) << 16) | (uint32_t(block[5]) << 8) | uint32_t(block[4]);

    for (int round = 0; round < 32; ++round) {
        uint32_t tempB = B;
        B = A ^ magmaF(B, roundKeys[round]);
        A = tempB;
    }
    vector<unsigned char> res(8);
    res[0] = (A >> 0) & 0xFF;
    res[1] = (A >> 8) & 0xFF;
    res[2] = (A >> 16) & 0xFF;
    res[3] = (A >> 24) & 0xFF;
    res[4] = (B >> 0) & 0xFF;
    res[5] = (B >> 8) & 0xFF;
    res[6] = (B >> 16) & 0xFF;
    res[7] = (B >> 24) & 0xFF;
    
    return res;
}

vector<unsigned char> padData(const vector<unsigned char>& data) {
    if (data.size() % 8 == 0) {
        return data;
    }
    size_t lenPad = 8 - (data.size() % 8);
    vector<unsigned char> padded = data;
    for (size_t i = 0; i < lenPad; ++i) {
        padded.push_back(static_cast<unsigned char>(lenPad));
    }
    return padded;
}

vector<unsigned char> unpadData(const vector<unsigned char>& data) {
    if (data.empty() || data.size() % 8 != 0) {
        return data;
    }
    unsigned char lenPad = data.back();
    if (lenPad == 0 || lenPad > 8 || lenPad > data.size()) {
        return data;
    }
    for (size_t i = data.size() - lenPad; i < data.size(); ++i) {
        if (static_cast<unsigned char>(data[i]) != lenPad) {
            return data;
        }
    }
    return vector<unsigned char>(data.begin(), data.end() - lenPad);
}

vector<unsigned char> magmaProcess(const vector<unsigned char>& data, const string& key, bool encrypt) {
    vector<unsigned char> processed;
    if (key.length() != 32) {
        throw runtime_error("Ключ должен быть длиной 32 байта");
    }
    
    auto roundKeys = magmaExpKey(key, encrypt);
    
    vector<unsigned char> workingData = encrypt ? padData(data) : data;
    
    for (size_t i = 0; i < workingData.size(); i += 8) {
        vector<unsigned char> block(workingData.begin() + i, workingData.begin() + i + 8);
        vector<unsigned char> processedBlock = encrypt ? 
            magmaEncBlock(block, roundKeys) : 
            magmaDecBlock(block, roundKeys);
        processed.insert(processed.end(), processedBlock.begin(), processedBlock.end());
    }
    
    if (!encrypt) {
        processed = unpadData(processed);
    }
    
    return processed;
}

string bytesToHex(const vector<unsigned char>& data) {
    stringstream ss;
    for (unsigned char byte : data) {
        ss << hex << setw(2) << setfill('0') << static_cast<int>(byte);
    }
    return ss.str();
}

vector<unsigned char> hexToBytes(const string& hex) {
    vector<unsigned char> bytes;
    for (size_t i = 0; i < hex.length(); i += 2) {
        string byteStr = hex.substr(i, 2);
        bytes.push_back(static_cast<unsigned char>(stoul(byteStr, nullptr, 16)));
    }
    return bytes;
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
    void magmaEncFile(const string& inputFile, const string& outputFile, const string& keyFile) {
        auto data = readFile(inputFile);
        
        string key = generateRandomKey(32);
        saveKeyToFile(key, keyFile);
        
        cout << "Ключ сгенерирован: " << bytesToHex(vector<unsigned char>(key.begin(), key.end())) << endl;
        
        auto res = magmaProcess(data, key, true);
        writeFile(outputFile, res);
    }

    void magmaDecFile(const string& inputFile, const string& outputFile, const string& keyFile) {
        auto data = readFile(inputFile);
        string key = loadKeyFromFile(keyFile);
        
        cout << "Ключ загружен: " << bytesToHex(vector<unsigned char>(key.begin(), key.end())) << endl;
        
        auto res = magmaProcess(data, key, false);
        writeFile(outputFile, res);
    }

    string magmaEncText(const string& text, const string& keyFile) {
        vector<unsigned char> data(text.begin(), text.end());
        
        string key = generateRandomKey(32);
        saveKeyToFile(key, keyFile);
        
        cout << "Ключ сгенерирован: " << bytesToHex(vector<unsigned char>(key.begin(), key.end())) << endl;
        
        auto res = magmaProcess(data, key, true);
        return bytesToHex(res);
    }

    string magmaDecText(const string& hexText, const string& keyFile) {
        string key = loadKeyFromFile(keyFile);
        
        cout << "Ключ загружен: " << bytesToHex(vector<unsigned char>(key.begin(), key.end())) << endl;
        
        vector<unsigned char> data = hexToBytes(hexText);
        auto res = magmaProcess(data, key, false);
        return string(res.begin(), res.end());
    }
}

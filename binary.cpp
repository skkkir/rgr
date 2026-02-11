#include "binary.h"
#include "file.h"
#include <cstdint>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <bitset>
#include <stdexcept>

using namespace std;

// Функция для шифрования/дешифрования методом бинарной последовательности
string binarySequenceProcess(const string& data, const string& key) {
    vector<unsigned char> result;
    size_t keyLen = key.length();
    
    if (keyLen == 0) {
        return data;
    }
    
    // Применяем операцию XOR между данными и ключом
    for (size_t i = 0; i < data.length(); ++i) {
        unsigned char dataByte = static_cast<unsigned char>(data[i]);
        unsigned char keyByte = static_cast<unsigned char>(key[i % keyLen]);
        unsigned char encryptedByte = dataByte ^ keyByte; // XOR операция
        result.push_back(encryptedByte);
    }
    
    return string(result.begin(), result.end());
}

// Генерация случайного ключа фиксированной длины (32 байта = 256 бит)
string genBinaryKey() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 255);
    
    string key;
    for (size_t i = 0; i < 32; ++i) {
        key += static_cast<char>(dis(gen));
    }
    return key;
}

// Конвертация байтов в шестнадцатеричную строку
string bytesToHex(const vector<unsigned char>& data) {
    stringstream ss;
    for (unsigned char byte : data) {
        ss << hex << setw(2) << setfill('0') << static_cast<int>(byte);
    }
    return ss.str();
}

// Конвертация шестнадцатеричной строки в байты
vector<unsigned char> hexToBytes(const string& hex) {
    vector<unsigned char> bytes;
    
    if (hex.length() % 2 != 0) {
        throw invalid_argument("Invalid hex string length");
    }
    
    for (size_t i = 0; i < hex.length(); i += 2) {
        string byteStr = hex.substr(i, 2);
        unsigned char byte = static_cast<unsigned char>(stoul(byteStr, nullptr, 16));
        bytes.push_back(byte);
    }
    
    return bytes;
}

// Конвертация строки в бинарную строку (последовательность 0 и 1)
string stringToBinaryString(const string& data) {
    string binaryStr;
    for (unsigned char byte : data) {
        binaryStr += bitset<8>(byte).to_string();
    }
    return binaryStr;
}

// Конвертация бинарной строки (последовательности 0 и 1) в исходную строку
string binaryStringToString(const string& binaryStr) {
    if (binaryStr.length() % 8 != 0) {
        throw invalid_argument("Invalid binary string length");
    }
    
    string result;
    for (size_t i = 0; i < binaryStr.length(); i += 8) {
        string byteStr = binaryStr.substr(i, 8);
        char byte = static_cast<char>(stoul(byteStr, nullptr, 2));
        result += byte;
    }
    return result;
}

// Сохранение ключа в файл
void saveBinaryKeyToFile(const string& key, const string& keyFile) {
    vector<unsigned char> keyVec(key.begin(), key.end());
    writeFile(keyFile, keyVec);
}

// Загрузка ключа из файла
string loadBinaryKeyFromFile(const string& keyFile) {
    auto keyVec = readFile(keyFile);
    return string(keyVec.begin(), keyVec.end());
}

// Функция для вывода бинарной последовательности одной строкой
void printBinarySequence(const string& data) {
    cout << stringToBinaryString(data);
}

extern "C" {
    void binarySequenceEncFile(const string& inputFile, const string& outputFile) {
        // Генерируем ключ
        string key = genBinaryKey();
        
        // Сохраняем ключ в файл с тем же именем, но с расширением .key
        string keyFile = outputFile + ".key";
        saveBinaryKeyToFile(key, keyFile);
        
        // Читаем входной файл
        auto dataVec = readFile(inputFile);
        string dataStr(dataVec.begin(), dataVec.end());
        
        // Шифруем данные
        string encrypted = binarySequenceProcess(dataStr, key);
        
        // Выводим зашифрованную бинарную последовательность одной строкой
        printBinarySequence(encrypted);
        cout << endl;
        
        // Записываем зашифрованные данные в выходной файл
        vector<unsigned char> encryptedVec(encrypted.begin(), encrypted.end());
        writeFile(outputFile, encryptedVec);
        
        cout << "File encrypted successfully. Key saved to: " << keyFile << endl;
    }

    void binarySequenceDecFile(const string& inputFile, const string& outputFile) {
        // Определяем файл с ключом (по соглашению тот же файл + .key)
        string keyFile = inputFile + ".key";
        
        // Загружаем ключ
        string key = loadBinaryKeyFromFile(keyFile);
        
        // Читаем зашифрованный файл
        auto dataVec = readFile(inputFile);
        string dataStr(dataVec.begin(), dataVec.end());
        
        // Дешифруем данные
        string decrypted = binarySequenceProcess(dataStr, key);
        
        // Выводим расшифрованную бинарную последовательность одной строкой
        printBinarySequence(decrypted);
        cout << endl;
        
        // Записываем дешифрованные данные в выходной файл
        vector<unsigned char> decryptedVec(decrypted.begin(), decrypted.end());
        writeFile(outputFile, decryptedVec);
        
        cout << "File decrypted successfully using key from: " << keyFile << endl;
    }

    string binarySequenceEncText(const string& text) {
        // Генерируем ключ
        string key = genBinaryKey();
        
        // Шифруем текст
        string encrypted = binarySequenceProcess(text, key);
        
        // Конвертируем зашифрованные данные в бинарную строку
        string binaryEncrypted = stringToBinaryString(encrypted);
        
        // Сохраняем ключ в глобальной переменной или возвращаем его отдельно
        // Для простоты будем сохранять ключ вместе с зашифрованными данными
        // Но выводить будем только бинарную последовательность
        string keyBinary = stringToBinaryString(key);
        
        // Выводим ТОЛЬКО зашифрованную бинарную последовательность
 
        
        // Возвращаем и ключ, и зашифрованные данные для возможности дешифрования
        return keyBinary + "|" + binaryEncrypted;
    }

    string binarySequenceDecText(const string& encryptedData) {
        // Разделяем ключ и зашифрованные данные
        size_t separatorPos = encryptedData.find('|');
        
        if (separatorPos == string::npos) {
            throw invalid_argument("Invalid encrypted data format");
        }
        
        // Извлекаем ключ и зашифрованные данные в бинарном формате
        string keyBinary = encryptedData.substr(0, separatorPos);
        string dataBinary = encryptedData.substr(separatorPos + 1);
        
        // Конвертируем бинарные строки обратно в исходные данные
        string keyStr = binaryStringToString(keyBinary);
        string dataStr = binaryStringToString(dataBinary);
        
        // Дешифруем данные
        string decrypted = binarySequenceProcess(dataStr, keyStr);
        
        // Выводим расшифрованную бинарную последовательность
        printBinarySequence(decrypted);
        cout << endl;
        
        return decrypted;
    }
}

// Альтернативная версия, где ключ хранится отдельно (например, в файле)
// Это позволит выводить только зашифрованную бинарную последовательность
// и дешифровать её, зная ключ из внешнего источника
string binarySequenceEncTextSimple(const string& text) {
    // Генерируем и сохраняем ключ
    static string savedKey = genBinaryKey();
    
    // Шифруем текст
    string encrypted = binarySequenceProcess(text, savedKey);
    
    // Выводим ТОЛЬКО зашифрованную бинарную последовательность
    string binaryEncrypted = stringToBinaryString(encrypted);
    cout << binaryEncrypted << endl;
    
    // Возвращаем только зашифрованные данные
    return binaryEncrypted;
}

string binarySequenceDecTextSimple(const string& binaryEncrypted) {
    // Используем ранее сохранённый ключ
    static string savedKey; // В реальном приложении ключ нужно где-то хранить
    
    // Для демонстрации создадим новый ключ, если его нет
    if (savedKey.empty()) {
        savedKey = genBinaryKey();
        cout << "Warning: Using new generated key" << endl;
    }
    
    // Конвертируем бинарную строку в данные
    string dataStr = binaryStringToString(binaryEncrypted);
    
    // Дешифруем данные
    string decrypted = binarySequenceProcess(dataStr, savedKey);
    
    // Выводим расшифрованную бинарную последовательность
    printBinarySequence(decrypted);
    cout << endl;
    
    return decrypted;
}
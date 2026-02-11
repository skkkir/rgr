#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <random>
#include <cstring>
#include <limits>
#include <stdexcept>
#include <dlfcn.h>

using namespace std;

enum class CiphType {
    BEAUFORT = 1,
    MAGMA = 2,
    BINARY = 3,
    EXIT = 4
};

enum class CiphAction { 
    ENCRYPT = 1, 
    DECRYPT = 2, 
    BACK = 3 
};

enum class InCiph { 
    CONSOLE_INPUT = 1, 
    FILE_INPUT = 2, 
    BACK = 3 
};

int main() {
    while (true) {
        try {
            system("clear");
            cout << "=== КРИПТОГРАФИЧЕСКАЯ СИСТЕМА ===" << endl;
            cout << "1. Шифр Бофора" << endl;
            cout << "2. Шифр Магма" << endl;
            cout << "3. Шифр бинарной последовательности" << endl;
            cout << "4. Завершение работы" << endl;
            cout << "==================================" << endl;
            cout << "Выберите алгоритм шифрования: ";
            
            int input;
            if (!(cin >> input) || input < 1 || input > 4) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw runtime_error("Пожалуйста, введите число от 1 до 4!");
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            CiphType choice = static_cast<CiphType>(input);
            
            switch (choice) {
                case CiphType::BEAUFORT: {
                    void* lib = dlopen("./libbeaufort.so", RTLD_LAZY);
                    if (!lib) {
                        cerr << "Ошибка загрузки библиотеки Beaufort: " << dlerror() << endl;
                        cout << "Нажмите Enter для возврата в меню...";
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        break;
                    }

                    auto encFile = (void (*)(const string&, const string&, const string&))dlsym(lib, "beaufortEncFile");
                    auto decFile = (void (*)(const string&, const string&, const string&))dlsym(lib, "beaufortDecFile");
                    auto encText = (string (*)(const string&, const string&))dlsym(lib, "beaufortEncText");
                    auto decText = (string (*)(const string&, const string&))dlsym(lib, "beaufortDecText");

                    bool repeat = true;
                    while (repeat) {
                        try {
                            system("clear");
                            cout << "=== ШИФР БОФОРА ===" << endl;
                            cout << "1. Шифрование данных" << endl;
                            cout << "2. Дешифрование данных" << endl;
                            cout << "3. Возврат в главное меню" << endl;
                            cout << "=========================" << endl;
                            cout << "Выберите операцию: ";
                            
                            int action;
                            if (!(cin >> action) || action < 1 || action > 3) {
                                cin.clear();
                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                throw runtime_error("Неверный выбор операции");
                            }
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                           
                            CiphAction ciphAction = static_cast<CiphAction>(action);
                            
                            switch (ciphAction) {
                                case CiphAction::ENCRYPT: {
                                    system("clear");
                                    cout << "=== ШИФРОВАНИЕ ДАННЫХ ===" << endl;
                                    cout << "1. Ввод текста вручную" << endl;
                                    cout << "2. Обработка файла" << endl;
                                    cout << "3. Возврат" << endl;
                                    cout << "========================" << endl;
                                    cout << "Выберите источник данных: ";
                                    
                                    int choice;
                                    if (!(cin >> choice) || choice < 1 || choice > 3) {
                                        cin.clear();
                                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                        throw runtime_error("Неверный выбор источника данных");
                                    }
                                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                    
                                    InCiph inChoice = static_cast<InCiph>(choice);
                                    
                                    switch (inChoice) {
                                        case InCiph::CONSOLE_INPUT: {
                                            string text, keyFile;
                                            cout << "Введите текст для шифрования: ";
                                            getline(cin, text);
                                            cout << "Введите имя файла для сохранения ключа: ";
                                            getline(cin, keyFile);
                                            
                                            string result = encText(text, keyFile);
                                            cout << "Зашифрованный текст (hex): " << result << endl;
                                            cout << "Ключ сохранен в файл: " << keyFile << endl;
                                            break;
                                        }
                                        case InCiph::FILE_INPUT: {
                                            string inputFile, outputFile, keyFile;
                                            cout << "Введите имя исходного файла: ";
                                            getline(cin, inputFile);
                                            cout << "Введите имя файла для результата: ";
                                            getline(cin, outputFile);
                                            cout << "Введите имя файла для ключа: ";
                                            getline(cin, keyFile);
                                            
                                            encFile(inputFile, outputFile, keyFile);
                                            cout << "Файл успешно зашифрован: " << outputFile << endl;
                                            cout << "Ключ сохранен в файл: " << keyFile << endl;
                                            break;
                                        }
                                        case InCiph::BACK:
                                            break;
                                    }
                                    break;
                                }
                                case CiphAction::DECRYPT: {
                                    system("clear");
                                    cout << "=== ДЕШИФРОВАНИЕ ДАННЫХ ===" << endl;
                                    cout << "1. Ввод текста вручную" << endl;
                                    cout << "2. Обработка файла" << endl;
                                    cout << "3. Возврат" << endl;
                                    cout << "=========================" << endl;
                                    cout << "Выберите источник данных: ";
                                    
                                    int choice;
                                    if (!(cin >> choice) || choice < 1 || choice > 3) {
                                        cin.clear();
                                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                        throw runtime_error("Неверный выбор источника данных");
                                    }
                                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                    
                                    InCiph inChoice = static_cast<InCiph>(choice);
                                    
                                    switch (inChoice) {
                                        case InCiph::CONSOLE_INPUT: {
                                            string text, keyFile;
                                            cout << "Введите зашифрованный текст (hex): ";
                                            getline(cin, text);
                                            cout << "Введите имя файла с ключом: ";
                                            getline(cin, keyFile);
                                            
                                            string result = decText(text, keyFile);
                                            cout << "Расшифрованный текст: " << result << endl;
                                            break;
                                        }
                                        case InCiph::FILE_INPUT: {
                                            string inputFile, outputFile, keyFile;
                                            cout << "Введите имя зашифрованного файла: ";
                                            getline(cin, inputFile);
                                            cout << "Введите имя файла для результата: ";
                                            getline(cin, outputFile);
                                            cout << "Введите имя файла с ключом: ";
                                            getline(cin, keyFile);
                                            
                                            decFile(inputFile, outputFile, keyFile);
                                            cout << "Файл успешно расшифрован: " << outputFile << endl;
                                            break;
                                        }
                                        case InCiph::BACK:
                                            break;
                                    }
                                    break;
                                }
                                case CiphAction::BACK:
                                    repeat = false;
                                    break;
                            }
                            
                            if (ciphAction != CiphAction::BACK) {
                                cout << "Нажмите Enter для продолжения...";
                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            }
                        }
                        catch (const exception& e) {
                            system("clear");
                            cerr << "Ошибка: " << e.what() << endl;
                            cout << "Нажмите Enter для продолжения...";
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        }
                    }
                    
                    dlclose(lib);
                    break;
                }
                case CiphType::MAGMA: {
                    void* lib = dlopen("./libmagma.so", RTLD_LAZY);
                    if (!lib) {
                        cerr << "Ошибка загрузки библиотеки Magma: " << dlerror() << endl;
                        cout << "Нажмите Enter для возврата в меню...";
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        break;
                    }

                    auto encFile = (void (*)(const string&, const string&, const string&))dlsym(lib, "magmaEncFile");
                    auto decFile = (void (*)(const string&, const string&, const string&))dlsym(lib, "magmaDecFile");
                    auto encText = (string (*)(const string&, const string&))dlsym(lib, "magmaEncText");
                    auto decText = (string (*)(const string&, const string&))dlsym(lib, "magmaDecText");

                    bool repeat = true;
                    while (repeat) {
                        try {
                            system("clear");
                            cout << "=== ШИФР МАГМА ===" << endl;
                            cout << "1. Шифрование данных" << endl;
                            cout << "2. Дешифрование данных" << endl;
                            cout << "3. Возврат в главное меню" << endl;
                            cout << "=====================" << endl;
                            cout << "Выберите операцию: ";
                            
                            int action;
                            if (!(cin >> action) || action < 1 || action > 3) {
                                cin.clear();
                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                throw runtime_error("Неверный выбор операции");
                            }
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                           
                            CiphAction ciphAction = static_cast<CiphAction>(action);
                            
                            switch (ciphAction) {
                                case CiphAction::ENCRYPT: {
                                    system("clear");
                                    cout << "=== ШИФРОВАНИЕ ДАННЫХ ===" << endl;
                                    cout << "1. Ввод текста вручную" << endl;
                                    cout << "2. Обработка файла" << endl;
                                    cout << "3. Возврат" << endl;
                                    cout << "========================" << endl;
                                    cout << "Выберите источник данных: ";
                                    
                                    int choice;
                                    if (!(cin >> choice) || choice < 1 || choice > 3) {
                                        cin.clear();
                                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                        throw runtime_error("Неверный выбор источника данных");
                                    }
                                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                    
                                    InCiph inChoice = static_cast<InCiph>(choice);
                                    
                                    switch (inChoice) {
                                        case InCiph::CONSOLE_INPUT: {
                                            string text, keyFile;
                                            cout << "Введите текст для шифрования: ";
                                            getline(cin, text);
                                            cout << "Введите имя файла для сохранения ключа: ";
                                            getline(cin, keyFile);
                                            
                                            string result = encText(text, keyFile);
                                            cout << "Зашифрованный текст (hex): " << result << endl;
                                            cout << "Ключ сгенерирован и сохранен в файл: " << keyFile << endl;
                                            break;
                                        }
                                        case InCiph::FILE_INPUT: {
                                            string inputFile, outputFile, keyFile;
                                            cout << "Введите имя исходного файла: ";
                                            getline(cin, inputFile);
                                            cout << "Введите имя файла для результата: ";
                                            getline(cin, outputFile);
                                            cout << "Введите имя файла для ключа: ";
                                            getline(cin, keyFile);
                                            
                                            encFile(inputFile, outputFile, keyFile);
                                            cout << "Файл успешно зашифрован: " << outputFile << endl;
                                            cout << "Ключ сгенерирован и сохранен в файл: " << keyFile << endl;
                                            break;
                                        }
                                        case InCiph::BACK:
                                            break;
                                    }
                                    break;
                                }
                                case CiphAction::DECRYPT: {
                                    system("clear");
                                    cout << "=== ДЕШИФРОВАНИЕ ДАННЫХ ===" << endl;
                                    cout << "1. Ввод текста вручную" << endl;
                                    cout << "2. Обработка файла" << endl;
                                    cout << "3. Возврат" << endl;
                                    cout << "=========================" << endl;
                                    cout << "Выберите источник данных: ";
                                    
                                    int choice;
                                    if (!(cin >> choice) || choice < 1 || choice > 3) {
                                        cin.clear();
                                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                        throw runtime_error("Неверный выбор источника данных");
                                    }
                                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                    
                                    InCiph inChoice = static_cast<InCiph>(choice);
                                    
                                    switch (inChoice) {
                                        case InCiph::CONSOLE_INPUT: {
                                            string text, keyFile;
                                            cout << "Введите зашифрованный текст (hex): ";
                                            getline(cin, text);
                                            cout << "Введите имя файла с ключом: ";
                                            getline(cin, keyFile);
                                            
                                            string result = decText(text, keyFile);
                                            cout << "Расшифрованный текст: " << result << endl;
                                            break;
                                        }
                                        case InCiph::FILE_INPUT: {
                                            string inputFile, outputFile, keyFile;
                                            cout << "Введите имя зашифрованного файла: ";
                                            getline(cin, inputFile);
                                            cout << "Введите имя файла для результата: ";
                                            getline(cin, outputFile);
                                            cout << "Введите имя файла с ключом: ";
                                            getline(cin, keyFile);
                                            
                                            decFile(inputFile, outputFile, keyFile);
                                            cout << "Файл успешно расшифрован: " << outputFile << endl;
                                            break;
                                        }
                                        case InCiph::BACK:
                                            break;
                                    }
                                    break;
                                }
                                case CiphAction::BACK:
                                    repeat = false;
                                    break;
                            }
                            
                            if (ciphAction != CiphAction::BACK) {
                                cout << "Нажмите Enter для продолжения...";
                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            }
                        }
                        catch (const exception& e) {
                            system("clear");
                            cerr << "Ошибка: " << e.what() << endl;
                            cout << "Нажмите Enter для продолжения...";
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        }
                    }
                    
                    dlclose(lib);
                    break;
                }
                case CiphType::BINARY: {
                    void* lib = dlopen("./libbinary.so", RTLD_LAZY);
                    if (!lib) {
                        cerr << "Ошибка загрузки библиотеки Binary Sequence: " << dlerror() << endl;
                        cout << "Нажмите Enter для возврата в меню...";
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        break;
                    }

                    auto encFile = (void (*)(const string&, const string&))dlsym(lib, "binarySequenceEncFile");
                    auto decFile = (void (*)(const string&, const string&))dlsym(lib, "binarySequenceDecFile");
                    auto encText = (string (*)(const string&))dlsym(lib, "binarySequenceEncText");
                    auto decText = (string (*)(const string&))dlsym(lib, "binarySequenceDecText");

                    bool repeat = true;
                    while (repeat) {
                        try {
                            system("clear");
                            cout << "=== ШИФР БИНАРНОЙ ПОСЛЕДОВАТЕЛЬНОСТИ ===" << endl;
                            cout << "1. Шифрование данных" << endl;
                            cout << "2. Дешифрование данных" << endl;
                            cout << "3. Возврат в главное меню" << endl;
                            cout << "==================================" << endl;
                            cout << "Выберите операцию: ";
                            
                            int action;
                            if (!(cin >> action) || action < 1 || action > 3) {
                                cin.clear();
                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                throw runtime_error("Неверный выбор операции");
                            }
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                           
                            CiphAction ciphAction = static_cast<CiphAction>(action);
                            
                            switch (ciphAction) {
                                case CiphAction::ENCRYPT: {
                                    system("clear");
                                    cout << "=== ШИФРОВАНИЕ ДАННЫХ ===" << endl;
                                    cout << "1. Ввод текста вручную" << endl;
                                    cout << "2. Обработка файла" << endl;
                                    cout << "3. Возврат" << endl;
                                    cout << "========================" << endl;
                                    cout << "Выберите источник данных: ";
                                    
                                    int choice;
                                    if (!(cin >> choice) || choice < 1 || choice > 3) {
                                        cin.clear();
                                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                        throw runtime_error("Неверный выбор источника данных");
                                    }
                                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                    
                                    InCiph inChoice = static_cast<InCiph>(choice);
                                    
                                    switch (inChoice) {
                                        case InCiph::CONSOLE_INPUT: {
                                            string text;
                                            cout << "Введите текст для шифрования: ";
                                            getline(cin, text);
                                            
                                            string result = encText(text);
                                            cout << "Зашифрованный текст: " << result << endl;
                                            break;
                                        }
                                        case InCiph::FILE_INPUT: {
                                            string inputFile, outputFile;
                                            cout << "Введите имя исходного файла: ";
                                            getline(cin, inputFile);
                                            cout << "Введите имя файла для результата: ";
                                            getline(cin, outputFile);
                                            
                                            encFile(inputFile, outputFile);
                                            cout << "Файл успешно зашифрован: " << outputFile << endl;
                                            break;
                                        }
                                        case InCiph::BACK:
                                            break;
                                    }
                                    break;
                                }
                                case CiphAction::DECRYPT: {
                                    system("clear");
                                    cout << "=== ДЕШИФРОВАНИЕ ДАННЫХ ===" << endl;
                                    cout << "1. Ввод текста вручную" << endl;
                                    cout << "2. Обработка файла" << endl;
                                    cout << "3. Возврат" << endl;
                                    cout << "=========================" << endl;
                                    cout << "Выберите источник данных: ";
                                    
                                    int choice;
                                    if (!(cin >> choice) || choice < 1 || choice > 3) {
                                        cin.clear();
                                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                        throw runtime_error("Неверный выбор источника данных");
                                    }
                                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                    
                                    InCiph inChoice = static_cast<InCiph>(choice);
                                    
                                    switch (inChoice) {
                                        case InCiph::CONSOLE_INPUT: {
                                            string text;
                                            cout << "Введите зашифрованный текст: ";
                                            getline(cin, text);
                                            
                                            string result = decText(text);
                                            cout << "Расшифрованный текст: " << result << endl;
                                            break;
                                        }
                                        case InCiph::FILE_INPUT: {
                                            string inputFile, outputFile;
                                            cout << "Введите имя зашифрованного файла: ";
                                            getline(cin, inputFile);
                                            cout << "Введите имя файла для результата: ";
                                            getline(cin, outputFile);
                                            
                                            decFile(inputFile, outputFile);
                                            cout << "Файл успешно расшифрован: " << outputFile << endl;
                                            break;
                                        }
                                        case InCiph::BACK:
                                            break;
                                    }
                                    break;
                                }
                                case CiphAction::BACK:
                                    repeat = false;
                                    break;
                            }
                            
                            if (ciphAction != CiphAction::BACK) {
                                cout << "Нажмите Enter для продолжения...";
                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            }
                        }
                        catch (const exception& e) {
                            system("clear");
                            cerr << "Ошибка: " << e.what() << endl;
                            cout << "Нажмите Enter для продолжения...";
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        }
                    }
                    
                    dlclose(lib);
                    break;
                }
                case CiphType::EXIT:
                    cout << "Завершение работы программы. До свидания!" << endl;
                    return 0;
            }
        }
        catch (const exception& e) {
            system("clear");
            cerr << "Ошибка: " << e.what() << endl;
            cout << "Нажмите Enter для продолжения...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

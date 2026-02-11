#include "file.h"

std::vector<unsigned char> readFile(const std::string& filename) {
    std::ifstream in(filename, std::ios::binary);
    if (!in){
        throw std::runtime_error("Ошибка открытия файла " + filename);    
    }
    
    in.seekg(0, std::ios::end);
    size_t size = in.tellg();
    in.seekg(0, std::ios::beg);
    
    std::vector<unsigned char> bytes(size);
    if (!in.read(reinterpret_cast<char*>(bytes.data()), size)) {
        throw std::runtime_error("Ошибка чтения файла " + filename);
    }  
    return bytes;
}

void writeFile(const std::string& filename, const std::vector<unsigned char>& data) {
    std::ofstream out(filename, std::ios::binary);
    if (!out){
        throw std::runtime_error("Ошибка создания файла " + filename);
    }
    if (!out.write(reinterpret_cast<const char*>(data.data()), data.size())) {
        throw std::runtime_error("Ошибка записи в файл " + filename);
    }
}

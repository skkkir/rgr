#ifndef FILE_H
#define FILE_H

#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>

std::vector<unsigned char> readFile(const std::string& filename);
void writeFile(const std::string& filename, const std::vector<unsigned char>& data);

#endif

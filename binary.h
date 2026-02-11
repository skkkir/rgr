#ifndef BINARY_H
#define BINARY_H

#include <string>

extern "C" {
    void binarySequenceEncFile(const std::string& inputFile, const std::string& outputFile);
    void binarySequenceDecFile(const std::string& inputFile, const std::string& outputFile);
    std::string binarySequenceEncText(const std::string& text);
    std::string binarySequenceDecText(const std::string& text);
}

#endif

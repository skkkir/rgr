#ifndef MAGMA_H
#define MAGMA_H

#include <string>

extern "C" {
    void magmaEncFile(const std::string& inputFile, const std::string& outputFile, const std::string& keyFile);
    void magmaDecFile(const std::string& inputFile, const std::string& outputFile, const std::string& keyFile);
    std::string magmaEncText(const std::string& text, const std::string& keyFile);
    std::string magmaDecText(const std::string& text, const std::string& keyFile);
}

#endif

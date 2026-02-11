#ifndef BEAUFORT_H
#define BEAUFORT_H

#include <string>

extern "C" {
    void beaufortEncFile(const std::string& inputFile, const std::string& outputFile, const std::string& keyFile);
    void beaufortDecFile(const std::string& inputFile, const std::string& outputFile, const std::string& keyFile);
    std::string beaufortEncText(const std::string& text, const std::string& keyFile);
    std::string beaufortDecText(const std::string& text, const std::string& keyFile);
}

#endif

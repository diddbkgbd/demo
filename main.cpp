#include <iostream>
#include "zlib/zlib.h"
#include <stdio.h>
#include <string.h>
#include <string>   
#include <fstream>  
#include <ctime> 
#include <map> 
#include <vector>


using namespace std;

void zipFile(const char *inputFilename, const char *outputZipFilename) {
    std::ifstream inputFile(inputFilename, std::ios::binary);
    if (!inputFile.is_open()) {
        std::cerr << "Error opening input file." << std::endl;
        return;
    }

    gzFile outputFile = gzopen(outputZipFilename, "wb");
    if (outputFile == nullptr) {
        std::cerr << "Error opening output zip file." << std::endl;
        inputFile.close();
        return;
    }

    char buffer[1024];
    size_t bytesRead;

    while ((bytesRead = inputFile.read(buffer, sizeof(buffer)).gcount()) > 0) {
        if (gzwrite(outputFile, buffer, bytesRead) != bytesRead) {
            std::cerr << "Error writing to zip file." << std::endl;
            inputFile.close();
            gzclose(outputFile);
            return;
        }
    }

    inputFile.close();
    gzclose(outputFile);

    std::cout << "Zip file created successfully: " << outputZipFilename << std::endl;
}


void decompressFile(const char *inputFilename, const char *outputFilename) {
    std::ifstream inputFile(inputFilename, std::ios::binary);
    if (!inputFile.is_open()) {
        std::cerr << "Error opening input file." << std::endl;
        return;
    }

    std::ofstream outputFile(outputFilename, std::ios::binary);
    if (!outputFile.is_open()) {
        std::cerr << "Error opening output file." << std::endl;
        inputFile.close();
        return;
    }

    std::vector<uint16_t> inputBuffer(std::istreambuf_iterator<char>(inputFile), {});

    size_t output_size = mz_zip_reader_extract_mem_to_heap(inputBuffer.data(), inputBuffer.size(), outputFile.rdbuf());

    if (output_size == 0) {
        std::cerr << "Error decompressing file." << std::endl;
    } else {
        std::cout << "File decompressed successfully: " << outputFilename << std::endl;
    }

    inputFile.close();
    outputFile.close();
}





int main() {
    const char *inputFilename = "test.txt";
    const char *outputZipFilename = "example.zip";
	
	zipFile(inputFilename, outputZipFilename);
    printf("The archive has created");

    decompressFile(inputFilename, outputZipFilename);
    printf("The archive has uncreated");
    
    return 0;
}
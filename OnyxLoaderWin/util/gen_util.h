#ifndef GEN_UTIL_H_
#define GEN_UTIL_H_

#include <iostream>
#include <fstream>
#include <string>


#include <Vcclr.h>
#include <wininet.h>
#include <msclr/marshal.h>
using namespace msclr::interop;


System::String ^ char_star_to_system_string(const char * in) {
    System::String^ out;
    out = gcnew System::String(in);
    return out;
}


char* get_firmware_version_from_file(char* filename) {

    char* unknown_version_string = new char[8];
    std::string unknown = "unknown";
    for(size_t i = 0; i < unknown.size(); i++) {
        unknown_version_string[i] = unknown[i];
    }
    unknown_version_string[7] = '\0';

    std::string::size_type version_length = 20;

    const char* search_term = "VERSION:";
    size_t search_term_size = strlen(search_term);

    std::ifstream file(filename, std::ios::binary);
    if (file) {
        file.seekg(0, std::ios::end);
        size_t file_size = 0;
        long long ll_file_size = file.tellg();
        if(ll_file_size < 0) {
            return unknown_version_string;
        }
        else {
            file_size = static_cast<size_t>(ll_file_size);
        }

        file.seekg(0, std::ios::beg);
        std::string file_content;
        file_content.reserve(file_size);
        char buffer[16384];
        std::streamsize chars_read;

        while (file.read(buffer, sizeof buffer), chars_read = file.gcount()) {
            file_content.append(buffer, static_cast<unsigned int>(chars_read) );
        }

        if (file.eof()) {
            for (std::string::size_type offset = 0, found_at;
                    file_size > offset && (found_at = file_content.find(search_term, offset)) != std::string::npos;
                    offset = found_at + search_term_size) {
                std::cout << found_at << std::endl;

                std::string s_version_string = file_content.substr(found_at+13, 5);

                char * version_string = new char[s_version_string.size() + 1];
                std::copy(s_version_string.begin(), s_version_string.end(), version_string);
                version_string[s_version_string.size()] = '\0'; // don't forget the terminating 0
                return version_string;
            }
        }
    }

    return unknown_version_string;
}


bool check_if_firmware_exists(char* filename) {
    std::ifstream firmware_file(filename, std::ios::binary);
    if (!firmware_file) {
        return false;
    }
    else {
        return true;
    }
}

#endif /* GEN_UTIL_H_ */
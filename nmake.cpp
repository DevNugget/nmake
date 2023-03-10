#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>

using namespace std;

string execCommand(const string& cmd, int& out_exitStatus);

int main(int argc, char *argv[]) {
    string command;
    vector<string> file_names;
    file_names.reserve(10); // pre-allocate memory for the vector
    
    fstream new_file;
    int filename_count = 0;
    
    new_file.open(argv[1], ios::in); 
    
    if (new_file.is_open()) { 
        string sa;
        while (getline(new_file, sa)) { 
            file_names.emplace_back(sa); // use emplace_back() instead of push_back()
            filename_count++;
        }
        
        new_file.close();
    }
    command = "g++ -Wall -ggdb3 ";
    for (auto& file_name : file_names) { // use range-based for loop
      command += file_name + " ";
    }
    command += "-o " + (string)argv[2];
    
    int exitStatus;
    string result = execCommand(command, exitStatus);
    if (exitStatus != 0) {
        cerr << "Compilation failed:\n" << result << endl;
    } else {
        cout << "Compilation successful" << endl;
    }
}

string execCommand(const string& cmd, int& out_exitStatus)
{
    out_exitStatus = 0;
    auto pPipe = ::popen(cmd.c_str(), "r");
    if(pPipe == nullptr)
    {
        throw runtime_error("Cannot open pipe");
    }

    array<char, 256> buffer;

    string result;

    while(not feof(pPipe))
    {
        auto bytes = fread(buffer.data(), 1, buffer.size(), pPipe);
        result.append(buffer.data(), bytes);
    }

    auto rc = ::pclose(pPipe);

    if(WIFEXITED(rc))
    {
        out_exitStatus = WEXITSTATUS(rc);
    }

    return result;
}

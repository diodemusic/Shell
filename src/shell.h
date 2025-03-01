#pragma once

#include <string>
#include <vector>

class Shell
{
private:
    bool runAgain;
    std::vector<std::string> builtins;
    std::string fullEnvVarPath;

    void echo(std::vector<std::string> args);
    void type(std::vector<std::string> args);
    void exitShell();
    void pwd();
    void cd(std::string newDir);
    bool isBuiltin(std::string command) const;
    bool isEnvVar(std::string command);
    void executeBuiltin(std::string command, std::vector<std::string> args);
    void executeEnvVar(std::string input);
    void printArgs(std::vector<std::string> args);
    std::string getInput() const;
    std::vector<std::string> splitString(std::string str, char seperator) const;
    std::string getCommand(std::string input);
    std::vector<std::string> getArgs(std::string input);
    std::string getCwd();
    void init();

public:
    Shell();
    void run();
};

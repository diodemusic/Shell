#include <algorithm>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>

#include "debug.h"
#include "echo.h"
#include "shell.h"

Shell::Shell() {};

void Shell::printArgs(std::vector<std::string> args)
{
    if (!(debug))
    {
        return;
    }

    for (const std::string &arg : args)
    {
        debug("Arg: " + arg);
    }
}

std::string Shell::getInput() const
{
    // Flush after every std::cout / std:cerr
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;
    std::cout << "$ ";
    std::string input;
    std::getline(std::cin, input);

    return input;
}

std::vector<std::string> Shell::splitString(std::string str, char delimiter) const
{
    std::stringstream ss(str);
    std::vector<std::string> result;
    std::string token;

    while (std::getline(ss, token, delimiter))
    {
        result.push_back(token);
    }

    return result;
}

std::string Shell::getCommand(std::string input)
{
    return this->splitString(input, ' ')[0];
}

std::vector<std::string> Shell::getArgs(std::string input)
{
    std::vector<std::string> args = {};
    std::vector<std::string> fullCommand = this->splitString(input, ' ');

    for (int i = 1; i < fullCommand.size(); ++i)
    {
        args.push_back(fullCommand[i]);
    }

    return args;
}

bool Shell::isBuiltin(std::string command) const
{
    if (std::find(this->builtins.begin(), this->builtins.end(), command) != this->builtins.end())
    {
        return true;
    }
    return false;
}

bool Shell::isEnvVar(std::string command)
{
    std::vector<std::string> envVars = this->splitString(std::getenv("PATH"), ':');

    for (const std::string &dir : envVars)
    {
        debug("Dir: " + dir);
        debug("Command: " + command);
        std::string fullPath = dir + "/" + command;

        if (access(fullPath.c_str(), X_OK) == 0)
        {
            this->fullEnvVarPath = fullPath;

            return true;
        }
    }

    return false;
}

std::string Shell::getCwd()
{
    debug("Getting cwd...");
    std::string cwd = std::filesystem::current_path().string();
    debug("cwd: " + cwd);

    return cwd;
}

void Shell::type(std::vector<std::string> args)
{
    if (this->isBuiltin(args[0]))
    {
        std::cout << args[0] << " is a shell builtin\n";
        return;
    }

    if (this->isEnvVar(args[0]))
    {
        std::cout << args[0] << " is " << this->fullEnvVarPath << "\n";
        return;
    }

    std::cout << args[0] << ": not found\n";
}

void Shell::exitShell()
{
    this->runAgain = false;
}

void Shell::pwd()
{
    std::string cwd = this->getCwd();
    std::cout << cwd << "\n";
}

void Shell::cd(std::string newDir)
{
    if (newDir[0] == '~')
    {
        std::string homePath = std::getenv("HOME");
        debug("Going to HOME path: " + homePath);
        std::filesystem::current_path(homePath);

        return;
    }

    if (access(newDir.c_str(), X_OK) != 0)
    {
        std::cout << "cd: " << newDir << ": No such file or directory\n";

        return;
    }

    if (newDir.rfind('/', 0) == 0)
    {
        debug("Changing to absoloute path: " + newDir);
        std::filesystem::current_path(newDir);

        return;
    }

    std::string cwd = this->getCwd();
    debug("Changing directory to: " + cwd + '/' + newDir);
    std::filesystem::current_path(cwd + '/' + newDir);
}

void Shell::executeBuiltin(std::string input, std::string command, std::vector<std::string> args)
{
    if (command == "echo")
    {
        Echo echo = Echo{};
        echo.run(input);
    }
    else if (command == "type")
    {
        this->type(args);
    }
    else if (command == "exit")
    {
        this->exitShell();
    }
    else if (command == "pwd")
    {
        this->pwd();
    }
    else if (command == "cd")
    {
        this->cd(args[0]);
    }
}

void Shell::executeEnvVar(std::string input)
{
    debug("Executing envVar: " + input);
    std::system(input.c_str());
}

void Shell::init()
{
    this->runAgain = true;
    this->builtins = {
        "echo",
        "type",
        "exit",
        "pwd",
        "cd"};
}

void Shell::run()
{
    this->init();

    while (this->runAgain)
    {
        std::string input = this->getInput();
        std::string command = this->getCommand(input);
        std::vector<std::string> args = this->getArgs(input);

        debug("Command: " + command);
        this->printArgs(args);

        if (this->isBuiltin(command))
        {
            debug("Is builtin, running: " + input);
            this->executeBuiltin(input, command, args);
            continue;
        }

        if (this->isEnvVar(command))
        {
            debug("Is envVar, running: " + input);
            this->executeEnvVar(input);
            continue;
        }

        std::cout << command << ": not found\n";
    }
}

#include <iostream>
#include <fstream>
#include <direct.h>
#include "vcs/Repository.h"

void createRepo(const std::string& repoName, const std::string& fileName, const std::string& content, const std::string& commitMsg) {
    std::string repoPath = "repos/" + repoName;
    std::string dataPath = repoPath + "/data";
    
    // Create directories
    _mkdir("repos");
    _mkdir(repoPath.c_str());
    _mkdir(dataPath.c_str());
    _mkdir((dataPath + "/commits").c_str());
    
    // Create repository
    Repository repo(dataPath);
    
    if (repo.initializeRepo()) {
        repo.addFile(fileName, content);
        repo.commitChanges(commitMsg);
        std::cout << "Created repository '" << repoName << "' with file '" << fileName << "'" << std::endl;
    }
}

int main() {
    std::cout << "=== Setting Up Test Repositories ===" << std::endl;
    
    // Create Python Calculator Repository
    createRepo("python-calculator", "calculator.py", 
        "def add(a, b):\n    return a + b\n\ndef subtract(a, b):\n    return a - b\n\nprint('Python Calculator')\n",
        "Initial Python calculator");
    
    // Create C++ Game Repository  
    createRepo("cpp-game", "game.cpp",
        "#include <iostream>\nusing namespace std;\n\nint main() {\n    cout << \"C++ Game Started!\" << endl;\n    return 0;\n}\n",
        "Initial C++ game");
    
    // Create Web Project Repository
    createRepo("web-project", "index.html",
        "<!DOCTYPE html>\n<html>\n<head>\n    <title>My Web Project</title>\n</head>\n<body>\n    <h1>Hello World!</h1>\n</body>\n</html>\n",
        "Initial web project");
    
    // Update repositories.txt
    std::ofstream repoList("repositories.txt");
    repoList << "my project" << std::endl;
    repoList << "python-calculator" << std::endl;
    repoList << "cpp-game" << std::endl;
    repoList << "web-project" << std::endl;
    repoList.close();
    
    std::cout << "\n=== Repository Setup Complete ===" << std::endl;
    std::cout << "Available repositories:" << std::endl;
    std::cout << "1. my project (has myproject.cpp)" << std::endl;
    std::cout << "2. python-calculator (has calculator.py)" << std::endl;
    std::cout << "3. cpp-game (has game.cpp)" << std::endl;
    std::cout << "4. web-project (has index.html)" << std::endl;
    
    std::cout << "\nNow run: .\\MiniVCS.exe" << std::endl;
    std::cout << "Choose: 1 (Manage Repositories) -> 3 (Switch Repository)" << std::endl;
    std::cout << "Each repository should show different files!" << std::endl;
    
    return 0;
}
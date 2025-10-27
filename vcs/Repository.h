#ifndef REPOSITORY_H
#define REPOSITORY_H

#include "Commit.h"
#include "FileHandler.h"
#include <stack>
#include <vector>
#include <string>

class Repository {
private:
    Commit* head;           // Latest commit (linked list head)
    Commit* current;        // Current commit position
    std::stack<Commit*> undoStack;  // For undo operations
    std::unordered_map<std::string, std::string> workingDirectory; // Current files
    FileHandler fileHandler;
    int nextVersionId;
    bool initialized;
    std::string dataPath;   // Path to repository data folder
    
public:
    Repository();
    Repository(const std::string& repoDataPath);
    ~Repository();
    
    // Repository path management
    void setDataPath(const std::string& path);
    
    // Core VCS operations
    bool initializeRepo();
    bool addFile(const std::string& filename, const std::string& content);
    bool commitChanges(const std::string& message);
    bool revertToCommit(int versionId);
    
    // Query operations
    std::vector<Commit*> getCommitHistory() const;
    Commit* getCurrentCommit() const;
    std::string getFileContent(const std::string& filename) const;
    std::vector<std::string> getWorkingFiles() const;
    
    // Status
    bool isInitialized() const;
    int getTotalCommits() const;
    
    // File operations
    bool saveRepository();
    bool loadRepository();
    
private:
    void clearCommitHistory();
    Commit* findCommitById(int versionId) const;
    void updateWorkingDirectory(Commit* commit);
};

#endif
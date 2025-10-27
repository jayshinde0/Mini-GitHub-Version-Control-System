#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include "Commit.h"
#include <vector>
#include <string>
#include <fstream>

class FileHandler {
private:
    std::string dataPath;
    
public:
    FileHandler();
    FileHandler(const std::string& path);
    ~FileHandler();
    
    // Path management
    void setDataPath(const std::string& path);
    
    // Directory operations
    bool createDirectory(const std::string& path);
    bool directoryExists(const std::string& path);
    
    // Commit file operations
    bool saveCommit(Commit* commit);
    Commit* loadCommit(int versionId);
    std::vector<Commit*> loadAllCommits();
    
    // Repository metadata
    bool saveRepositoryMetadata(int nextVersionId, bool initialized);
    bool loadRepositoryMetadata(int& nextVersionId, bool& initialized);
    
    // File content operations
    bool saveFileContent(const std::string& filename, const std::string& content);
    std::string loadFileContent(const std::string& filename);
    
private:
    std::string getCommitFilePath(int versionId);
    std::string getMetadataFilePath();
    bool fileExists(const std::string& path);
};

#endif
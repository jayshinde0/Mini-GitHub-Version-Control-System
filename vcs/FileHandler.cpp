#include "FileHandler.h"
#include <iostream>
#include <sstream>
#include <direct.h>
#include <sys/stat.h>

FileHandler::FileHandler() : dataPath("data") {
}

FileHandler::FileHandler(const std::string& path) : dataPath(path) {
}

FileHandler::~FileHandler() {
}

void FileHandler::setDataPath(const std::string& path) {
    dataPath = path;
}

bool FileHandler::createDirectory(const std::string& path) {
    return _mkdir(path.c_str()) == 0 || directoryExists(path);
}

bool FileHandler::directoryExists(const std::string& path) {
    struct stat info;
    return stat(path.c_str(), &info) == 0 && (info.st_mode & S_IFDIR);
}

bool FileHandler::saveCommit(Commit* commit) {
    if (!commit) return false;
    
    std::string filepath = getCommitFilePath(commit->versionId);
    std::ofstream file(filepath);
    
    if (!file.is_open()) {
        return false;
    }
    
    // Save commit metadata
    file << "VERSION_ID:" << commit->versionId << std::endl;
    file << "MESSAGE:" << commit->message << std::endl;
    file << "TIMESTAMP:" << commit->timestamp << std::endl;
    file << "FILES_COUNT:" << commit->files.size() << std::endl;
    
    // Save files
    for (const auto& fileEntry : commit->files) {
        file << "FILE_START:" << fileEntry.first << std::endl;
        file << "CONTENT_LENGTH:" << fileEntry.second.length() << std::endl;
        file << fileEntry.second << std::endl;
        file << "FILE_END" << std::endl;
    }
    
    file.close();
    return true;
}

Commit* FileHandler::loadCommit(int versionId) {
    std::string filepath = getCommitFilePath(versionId);
    std::ifstream file(filepath);
    
    if (!file.is_open()) {
        return nullptr;
    }
    
    std::string line;
    int loadedVersionId;
    std::string message, timestamp;
    int filesCount;
    
    // Read metadata
    if (std::getline(file, line) && line.substr(0, 11) == "VERSION_ID:") {
        loadedVersionId = std::stoi(line.substr(11));
    } else return nullptr;
    
    if (std::getline(file, line) && line.substr(0, 8) == "MESSAGE:") {
        message = line.substr(8);
    } else return nullptr;
    
    if (std::getline(file, line) && line.substr(0, 10) == "TIMESTAMP:") {
        timestamp = line.substr(10);
    } else return nullptr;
    
    if (std::getline(file, line) && line.substr(0, 12) == "FILES_COUNT:") {
        filesCount = std::stoi(line.substr(12));
    } else return nullptr;
    
    // Create commit
    Commit* commit = new Commit(loadedVersionId, message);
    commit->timestamp = timestamp;
    
    // Read files
    for (int i = 0; i < filesCount; i++) {
        std::string filename, content;
        int contentLength;
        
        if (std::getline(file, line) && line.substr(0, 11) == "FILE_START:") {
            filename = line.substr(11);
        } else {
            delete commit;
            return nullptr;
        }
        
        if (std::getline(file, line) && line.substr(0, 15) == "CONTENT_LENGTH:") {
            contentLength = std::stoi(line.substr(15));
        } else {
            delete commit;
            return nullptr;
        }
        
        // Read content
        std::stringstream contentStream;
        char ch;
        for (int j = 0; j < contentLength; j++) {
            file.get(ch);
            contentStream << ch;
        }
        content = contentStream.str();
        
        // Skip newline and FILE_END
        std::getline(file, line); // newline
        std::getline(file, line); // FILE_END
        
        commit->addFile(filename, content);
    }
    
    file.close();
    return commit;
}

std::vector<Commit*> FileHandler::loadAllCommits() {
    std::vector<Commit*> commits;
    
    // Try to load commits starting from version 1
    for (int i = 1; i < 1000; i++) { // Reasonable upper limit
        Commit* commit = loadCommit(i);
        if (commit) {
            commits.push_back(commit);
        } else {
            break; // No more commits
        }
    }
    
    return commits;
}

bool FileHandler::saveRepositoryMetadata(int nextVersionId, bool initialized) {
    std::string filepath = getMetadataFilePath();
    std::ofstream file(filepath);
    
    if (!file.is_open()) {
        return false;
    }
    
    file << "NEXT_VERSION_ID:" << nextVersionId << std::endl;
    file << "INITIALIZED:" << (initialized ? "1" : "0") << std::endl;
    
    file.close();
    return true;
}

bool FileHandler::loadRepositoryMetadata(int& nextVersionId, bool& initialized) {
    std::string filepath = getMetadataFilePath();
    std::ifstream file(filepath);
    
    if (!file.is_open()) {
        return false;
    }
    
    std::string line;
    
    if (std::getline(file, line) && line.substr(0, 16) == "NEXT_VERSION_ID:") {
        nextVersionId = std::stoi(line.substr(16));
    } else return false;
    
    if (std::getline(file, line) && line.substr(0, 12) == "INITIALIZED:") {
        initialized = (line.substr(12) == "1");
    } else return false;
    
    file.close();
    return true;
}

bool FileHandler::saveFileContent(const std::string& filename, const std::string& content) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    file << content;
    file.close();
    return true;
}

std::string FileHandler::loadFileContent(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return "";
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    
    return buffer.str();
}

std::string FileHandler::getCommitFilePath(int versionId) {
    return dataPath + "/commits/commit_" + std::to_string(versionId) + ".txt";
}

std::string FileHandler::getMetadataFilePath() {
    return dataPath + "/repo_metadata.txt";
}

bool FileHandler::fileExists(const std::string& path) {
    std::ifstream file(path);
    return file.good();
}
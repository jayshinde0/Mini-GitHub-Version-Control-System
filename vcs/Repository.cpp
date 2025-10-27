#include "Repository.h"
#include <iostream>
#include <algorithm>

Repository::Repository() 
    : head(nullptr), current(nullptr), nextVersionId(1), initialized(false) {
}

Repository::~Repository() {
    clearCommitHistory();
}

bool Repository::initializeRepo() {
    if (initialized) {
        return false; // Already initialized
    }
    
    // Create initial commit
    Commit* initialCommit = new Commit(0, "Initial commit");
    head = initialCommit;
    current = initialCommit;
    nextVersionId = 1;
    initialized = true;
    
    // Create data directory structure
    fileHandler.createDirectory("data");
    fileHandler.createDirectory("data/commits");
    
    return saveRepository();
}

bool Repository::addFile(const std::string& filename, const std::string& content) {
    if (!initialized) {
        return false;
    }
    
    workingDirectory[filename] = content;
    return true;
}

bool Repository::commitChanges(const std::string& message) {
    if (!initialized || workingDirectory.empty()) {
        return false;
    }
    
    // Create new commit
    Commit* newCommit = new Commit(nextVersionId++, message);
    
    // Copy all files from working directory
    for (const auto& file : workingDirectory) {
        newCommit->addFile(file.first, file.second);
    }
    
    // Link to previous commit
    newCommit->prev = current;
    if (current) {
        current->next = newCommit;
    }
    
    // Update pointers
    current = newCommit;
    if (!head || head->versionId == 0) {
        head = newCommit;
    }
    
    // Save commit to file
    fileHandler.saveCommit(newCommit);
    
    return saveRepository();
}

bool Repository::revertToCommit(int versionId) {
    if (!initialized) {
        return false;
    }
    
    Commit* targetCommit = findCommitById(versionId);
    if (!targetCommit) {
        return false;
    }
    
    // Push current state to undo stack
    undoStack.push(current);
    
    // Update current commit and working directory
    current = targetCommit;
    updateWorkingDirectory(targetCommit);
    
    return true;
}

std::vector<Commit*> Repository::getCommitHistory() const {
    std::vector<Commit*> history;
    Commit* commit = head;
    
    while (commit) {
        history.push_back(commit);
        commit = commit->prev;
    }
    
    return history;
}

Commit* Repository::getCurrentCommit() const {
    return current;
}

std::string Repository::getFileContent(const std::string& filename) const {
    auto it = workingDirectory.find(filename);
    if (it != workingDirectory.end()) {
        return it->second;
    }
    return "";
}

std::vector<std::string> Repository::getWorkingFiles() const {
    std::vector<std::string> files;
    for (const auto& file : workingDirectory) {
        files.push_back(file.first);
    }
    return files;
}

bool Repository::isInitialized() const {
    return initialized;
}

int Repository::getTotalCommits() const {
    int count = 0;
    Commit* commit = head;
    while (commit) {
        if (commit->versionId > 0) { // Skip initial commit
            count++;
        }
        commit = commit->prev;
    }
    return count;
}

bool Repository::saveRepository() {
    return fileHandler.saveRepositoryMetadata(nextVersionId, initialized);
}

bool Repository::loadRepository() {
    int loadedVersionId;
    bool loadedInitialized;
    
    if (fileHandler.loadRepositoryMetadata(loadedVersionId, loadedInitialized)) {
        nextVersionId = loadedVersionId;
        initialized = loadedInitialized;
        
        // Load commits from files
        std::vector<Commit*> commits = fileHandler.loadAllCommits();
        
        // Rebuild commit chain
        if (!commits.empty()) {
            // Sort commits by version ID
            std::sort(commits.begin(), commits.end(), 
                     [](Commit* a, Commit* b) { return a->versionId < b->versionId; });
            
            // Link commits
            for (size_t i = 0; i < commits.size(); i++) {
                if (i > 0) {
                    commits[i]->prev = commits[i-1];
                    commits[i-1]->next = commits[i];
                }
            }
            
            head = commits.back();
            current = head;
            updateWorkingDirectory(current);
        }
        
        return true;
    }
    
    return false;
}

void Repository::clearCommitHistory() {
    while (head) {
        Commit* temp = head;
        head = head->prev;
        delete temp;
    }
    current = nullptr;
}

Commit* Repository::findCommitById(int versionId) const {
    Commit* commit = head;
    while (commit) {
        if (commit->versionId == versionId) {
            return commit;
        }
        commit = commit->prev;
    }
    return nullptr;
}

void Repository::updateWorkingDirectory(Commit* commit) {
    workingDirectory.clear();
    if (commit) {
        workingDirectory = commit->files;
    }
}
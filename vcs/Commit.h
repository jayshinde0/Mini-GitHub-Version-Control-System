#ifndef COMMIT_H
#define COMMIT_H

#include <string>
#include <unordered_map>
#include <ctime>

class Commit {
public:
    int versionId;
    std::string message;
    std::string timestamp;
    std::unordered_map<std::string, std::string> files; // filename -> file content
    Commit* prev;
    Commit* next;
    
    Commit(int id, const std::string& msg);
    ~Commit();
    
    void addFile(const std::string& filename, const std::string& content);
    std::string getFile(const std::string& filename) const;
    bool hasFile(const std::string& filename) const;
    std::string getTimestamp() const;
    
private:
    std::string getCurrentTime();
};

#endif
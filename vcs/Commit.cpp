#include "Commit.h"
#include <sstream>
#include <iomanip>

Commit::Commit(int id, const std::string& msg) 
    : versionId(id), message(msg), prev(nullptr), next(nullptr) {
    timestamp = getCurrentTime();
}

Commit::~Commit() {
    // Destructor - cleanup handled by Repository
}

void Commit::addFile(const std::string& filename, const std::string& content) {
    files[filename] = content;
}

std::string Commit::getFile(const std::string& filename) const {
    auto it = files.find(filename);
    if (it != files.end()) {
        return it->second;
    }
    return "";
}

bool Commit::hasFile(const std::string& filename) const {
    return files.find(filename) != files.end();
}

std::string Commit::getTimestamp() const {
    return timestamp;
}

std::string Commit::getCurrentTime() {
    time_t now = time(0);
    tm* timeinfo = localtime(&now);
    
    std::stringstream ss;
    ss << std::put_time(timeinfo, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}
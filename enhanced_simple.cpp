#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <iomanip>
#include <algorithm>
#include <sstream>
#include "vcs/Repository.h"

class EnhancedConsoleInterface {
private:
    Repository repo;
    
public:
    void run() {
        std::cout << "=== Enhanced Mini VCS System ===" << std::endl;
        std::cout << "Version Control with File Editing & History Viewing" << std::endl;
        std::cout << std::endl;
        
        // Try to load existing repository data
        repo.loadRepository();
        
        bool running = true;
        while (running) {
            showMainMenu();
            int choice = getChoice();
            
            switch (choice) {
                case 1:
                    initializeRepository();
                    break;
                case 2:
                    manageFiles();
                    break;
                case 3:
                    commitChanges();
                    break;
                case 4:
                    viewHistory();
                    break;
                case 5:
                    revertToCommit();
                    break;
                case 6:
                    showStatus();
                    break;
                case 7:
                    viewFileContent();
                    break;
                case 8:
                    compareVersions();
                    break;
                case 0:
                    running = false;
                    std::cout << "Goodbye!" << std::endl;
                    break;
                default:
                    std::cout << "Invalid choice. Please try again." << std::endl;
            }
            
            if (running) {
                std::cout << "\nPress Enter to continue...";
                std::cin.ignore();
                std::cin.get();
            }
        }
    }
    
private:
    void showMainMenu() {
        system("cls"); // Clear screen (Windows)
        std::cout << "+============================================================+" << std::endl;
        std::cout << "|                    MINI VCS SYSTEM                        |" << std::endl;
        std::cout << "+============================================================+" << std::endl;
        
        std::cout << "Repository Status: " << (repo.isInitialized() ? "[INITIALIZED]" : "[NOT INITIALIZED]") << std::endl;
        
        if (repo.isInitialized()) {
            std::cout << "Total Commits: " << repo.getTotalCommits() << std::endl;
            std::cout << "Working Files: " << repo.getWorkingFiles().size() << std::endl;
        }
        
        std::cout << "\n+------------------- MAIN MENU -------------------+" << std::endl;
        std::cout << "| 1. Initialize Repository                        |" << std::endl;
        std::cout << "| 2. Manage Files (Add/Edit)                     |" << std::endl;
        std::cout << "| 3. Commit Changes                               |" << std::endl;
        std::cout << "| 4. View Commit History                          |" << std::endl;
        std::cout << "| 5. Revert to Commit                             |" << std::endl;
        std::cout << "| 6. Show Repository Status                       |" << std::endl;
        std::cout << "| 7. View File Content                            |" << std::endl;
        std::cout << "| 8. Compare File Versions                        |" << std::endl;
        std::cout << "| 0. Exit                                          |" << std::endl;
        std::cout << "+-------------------------------------------------+" << std::endl;
        std::cout << "\nEnter your choice: ";
    }
    
    int getChoice() {
        int choice;
        while (!(std::cin >> choice)) {
            std::cout << "Invalid input. Please enter a number: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        std::cin.ignore(); // Clear the newline
        return choice;
    }
    
    void initializeRepository() {
        std::cout << "\n+== Initialize Repository ==+" << std::endl;
        
        if (repo.isInitialized()) {
            std::cout << "Repository is already initialized!" << std::endl;
            return;
        }
        
        if (repo.initializeRepo()) {
            std::cout << "[SUCCESS] Repository initialized successfully!" << std::endl;
            std::cout << "\nData structures created:" << std::endl;
            std::cout << "  -> Linked List: Commit history chain" << std::endl;
            std::cout << "  -> Hash Map: File content storage" << std::endl;
            std::cout << "  -> Stack: Undo/revert operations" << std::endl;
            std::cout << "  -> File I/O: Local persistence system" << std::endl;
        } else {
            std::cout << "[ERROR] Failed to initialize repository!" << std::endl;
        }
    }
    
    void manageFiles() {
        std::cout << "\n+== File Management ==+" << std::endl;
        
        if (!repo.isInitialized()) {
            std::cout << "Please initialize repository first!" << std::endl;
            return;
        }
        
        std::cout << "1. Add New File" << std::endl;
        std::cout << "2. Edit Existing File" << std::endl;
        std::cout << "3. List All Files" << std::endl;
        std::cout << "Enter choice: ";
        
        int choice = getChoice();
        
        switch (choice) {
            case 1:
                addNewFile();
                break;
            case 2:
                editExistingFile();
                break;
            case 3:
                listAllFiles();
                break;
            default:
                std::cout << "Invalid choice!" << std::endl;
        }
    }
    
    void addNewFile() {
        std::cout << "\n--- Add New File ---" << std::endl;
        
        std::string filename;
        std::cout << "Enter filename (e.g., main.cpp, script.py, README.md): ";
        std::getline(std::cin, filename);
        
        if (filename.empty()) {
            std::cout << "Filename cannot be empty!" << std::endl;
            return;
        }
        
        // Check if file already exists
        std::string existingContent = repo.getFileContent(filename);
        if (!existingContent.empty()) {
            std::cout << "File '" << filename << "' already exists!" << std::endl;
            std::cout << "Use 'Edit Existing File' option to modify it." << std::endl;
            return;
        }
        
        std::cout << "\nEnter file content (end with '###' on a new line):" << std::endl;
        std::cout << "+-----------------------------------------------------+" << std::endl;
        
        std::string content, line;
        while (std::getline(std::cin, line) && line != "###") {
            content += line + "\n";
        }
        
        std::cout << "+-----------------------------------------------------+" << std::endl;
        
        if (repo.addFile(filename, content)) {
            std::cout << "[SUCCESS] File '" << filename << "' added successfully!" << std::endl;
            std::cout << "  Content length: " << content.length() << " characters" << std::endl;
            std::cout << "  Lines: " << std::count(content.begin(), content.end(), '\n') << std::endl;
        } else {
            std::cout << "[ERROR] Failed to add file!" << std::endl;
        }
    }
    
    void editExistingFile() {
        std::cout << "\n--- Edit Existing File ---" << std::endl;
        
        std::vector<std::string> files = repo.getWorkingFiles();
        if (files.empty()) {
            std::cout << "No files available to edit. Add some files first." << std::endl;
            return;
        }
        
        std::cout << "Available files:" << std::endl;
        for (size_t i = 0; i < files.size(); i++) {
            std::cout << "  " << (i + 1) << ". " << files[i] << std::endl;
        }
        
        std::cout << "Enter file number to edit: ";
        int fileChoice = getChoice();
        
        if (fileChoice < 1 || fileChoice > static_cast<int>(files.size())) {
            std::cout << "Invalid file selection!" << std::endl;
            return;
        }
        
        std::string filename = files[fileChoice - 1];
        std::string currentContent = repo.getFileContent(filename);
        
        // Show current content
        std::cout << "\n+== Current Content of '" << filename << "' ==+" << std::endl;
        displayFileContent(currentContent);
        std::cout << "+--------------------------------------------------+" << std::endl;
        
        std::cout << "\nChoose editing option:" << std::endl;
        std::cout << "1. Replace entire content" << std::endl;
        std::cout << "2. Append to existing content" << std::endl;
        std::cout << "3. Cancel editing" << std::endl;
        std::cout << "Enter choice: ";
        
        int editChoice = getChoice();
        std::string newContent = currentContent;
        
        switch (editChoice) {
            case 1: {
                std::cout << "\nEnter new content (end with '###' on a new line):" << std::endl;
                std::cout << "+-----------------------------------------------------+" << std::endl;
                
                newContent = "";
                std::string line;
                while (std::getline(std::cin, line) && line != "###") {
                    newContent += line + "\n";
                }
                std::cout << "+-----------------------------------------------------+" << std::endl;
                break;
            }
            case 2: {
                std::cout << "\nEnter content to append (end with '###' on a new line):" << std::endl;
                std::cout << "+-----------------------------------------------------+" << std::endl;
                
                std::string appendContent;
                std::string line;
                while (std::getline(std::cin, line) && line != "###") {
                    appendContent += line + "\n";
                }
                newContent += appendContent;
                std::cout << "+-----------------------------------------------------+" << std::endl;
                break;
            }
            case 3:
                std::cout << "Editing cancelled." << std::endl;
                return;
            default:
                std::cout << "Invalid choice!" << std::endl;
                return;
        }
        
        if (repo.addFile(filename, newContent)) {
            std::cout << "[SUCCESS] File '" << filename << "' updated successfully!" << std::endl;
            std::cout << "  Previous length: " << currentContent.length() << " characters" << std::endl;
            std::cout << "  New length: " << newContent.length() << " characters" << std::endl;
            std::cout << "  Change: " << (static_cast<int>(newContent.length()) - static_cast<int>(currentContent.length())) << " characters" << std::endl;
        } else {
            std::cout << "[ERROR] Failed to update file!" << std::endl;
        }
    }
    
    void listAllFiles() {
        std::cout << "\n--- All Files in Working Directory ---" << std::endl;
        
        std::vector<std::string> files = repo.getWorkingFiles();
        if (files.empty()) {
            std::cout << "No files in working directory." << std::endl;
            return;
        }
        
        std::cout << "+-----+-------------------------+---------+---------+" << std::endl;
        std::cout << "| No. | Filename                | Size    | Lines   |" << std::endl;
        std::cout << "+-----+-------------------------+---------+---------+" << std::endl;
        
        for (size_t i = 0; i < files.size(); i++) {
            std::string content = repo.getFileContent(files[i]);
            int lines = std::count(content.begin(), content.end(), '\n');
            
            std::cout << "| " << std::setw(3) << (i + 1) << " | " 
                      << std::setw(23) << std::left << files[i] << " | "
                      << std::setw(7) << std::right << content.length() << " | "
                      << std::setw(7) << lines << " |" << std::endl;
        }
        
        std::cout << "+-----+-------------------------+---------+---------+" << std::endl;
    }
    
    void displayFileContent(const std::string& content) {
        if (content.empty()) {
            std::cout << "| (empty file)                                     |" << std::endl;
            return;
        }
        
        std::istringstream iss(content);
        std::string line;
        int lineNum = 1;
        
        while (std::getline(iss, line) && lineNum <= 20) { // Show first 20 lines
            std::cout << "| " << std::setw(2) << lineNum << "| " 
                      << std::setw(45) << std::left << (line.length() > 45 ? line.substr(0, 42) + "..." : line) 
                      << " |" << std::endl;
            lineNum++;
        }
        
        if (lineNum > 20) {
            std::cout << "|   | ... (content truncated, showing first 20 lines) |" << std::endl;
        }
    }
    
    void viewFileContent() {
        std::cout << "\n+== View File Content ==+" << std::endl;
        
        if (!repo.isInitialized()) {
            std::cout << "Please initialize repository first!" << std::endl;
            return;
        }
        
        std::vector<std::string> files = repo.getWorkingFiles();
        if (files.empty()) {
            std::cout << "No files available to view." << std::endl;
            return;
        }
        
        std::cout << "Available files:" << std::endl;
        for (size_t i = 0; i < files.size(); i++) {
            std::cout << "  " << (i + 1) << ". " << files[i] << std::endl;
        }
        
        std::cout << "Enter file number to view: ";
        int fileChoice = getChoice();
        
        if (fileChoice < 1 || fileChoice > static_cast<int>(files.size())) {
            std::cout << "Invalid file selection!" << std::endl;
            return;
        }
        
        std::string filename = files[fileChoice - 1];
        std::string content = repo.getFileContent(filename);
        
        std::cout << "\n+== Content of '" << filename << "' ==+" << std::endl;
        
        if (content.empty()) {
            std::cout << "| (empty file)                                     |" << std::endl;
        } else {
            std::istringstream iss(content);
            std::string line;
            int lineNum = 1;
            
            while (std::getline(iss, line)) {
                std::cout << std::setw(3) << lineNum << "| " << line << std::endl;
                lineNum++;
            }
        }
        
        std::cout << "+--------------------------------------------------+" << std::endl;
        std::cout << "File size: " << content.length() << " characters" << std::endl;
        std::cout << "Lines: " << std::count(content.begin(), content.end(), '\n') << std::endl;
    }    
   
 void compareVersions() {
        std::cout << "\n+== Compare File Versions ==+" << std::endl;
        
        if (!repo.isInitialized()) {
            std::cout << "Please initialize repository first!" << std::endl;
            return;
        }
        
        std::vector<Commit*> history = repo.getCommitHistory();
        if (history.size() < 2) {
            std::cout << "Need at least 2 commits to compare versions." << std::endl;
            return;
        }
        
        std::cout << "Available commits:" << std::endl;
        for (const auto& commit : history) {
            if (commit->versionId == 0) continue;
            std::cout << "  " << commit->versionId << ": " << commit->message 
                      << " (" << commit->files.size() << " files)" << std::endl;
        }
        
        std::cout << "Enter first commit ID: ";
        int commit1 = getChoice();
        std::cout << "Enter second commit ID: ";
        int commit2 = getChoice();
        
        Commit* c1 = nullptr;
        Commit* c2 = nullptr;
        
        for (const auto& commit : history) {
            if (commit->versionId == commit1) c1 = commit;
            if (commit->versionId == commit2) c2 = commit;
        }
        
        if (!c1 || !c2) {
            std::cout << "Invalid commit IDs!" << std::endl;
            return;
        }
        
        std::cout << "\n+== Comparing Commits #" << commit1 << " vs #" << commit2 << " ==+" << std::endl;
        
        // Find common files
        for (const auto& file1 : c1->files) {
            auto it = c2->files.find(file1.first);
            if (it != c2->files.end()) {
                std::cout << "\nFile: " << file1.first << std::endl;
                std::cout << "Commit #" << commit1 << " size: " << file1.second.length() << " chars" << std::endl;
                std::cout << "Commit #" << commit2 << " size: " << it->second.length() << " chars" << std::endl;
                
                if (file1.second == it->second) {
                    std::cout << "Status: [IDENTICAL]" << std::endl;
                } else {
                    std::cout << "Status: [DIFFERENT]" << std::endl;
                    int diff = static_cast<int>(it->second.length()) - static_cast<int>(file1.second.length());
                    std::cout << "Size change: " << (diff >= 0 ? "+" : "") << diff << " characters" << std::endl;
                }
            }
        }
        
        // Files only in commit1
        for (const auto& file1 : c1->files) {
            if (c2->files.find(file1.first) == c2->files.end()) {
                std::cout << "\nFile: " << file1.first << " (only in commit #" << commit1 << ")" << std::endl;
            }
        }
        
        // Files only in commit2
        for (const auto& file2 : c2->files) {
            if (c1->files.find(file2.first) == c1->files.end()) {
                std::cout << "\nFile: " << file2.first << " (only in commit #" << commit2 << ")" << std::endl;
            }
        }
    }
    
    void commitChanges() {
        std::cout << "\n+== Commit Changes ==+" << std::endl;
        
        if (!repo.isInitialized()) {
            std::cout << "Please initialize repository first!" << std::endl;
            return;
        }
        
        std::vector<std::string> files = repo.getWorkingFiles();
        if (files.empty()) {
            std::cout << "No files to commit! Add some files first." << std::endl;
            return;
        }
        
        std::cout << "Files to be committed:" << std::endl;
        for (const auto& file : files) {
            std::string content = repo.getFileContent(file);
            std::cout << "  [+] " << file << " (" << content.length() << " chars)" << std::endl;
        }
        
        std::string message;
        std::cout << "\nEnter commit message: ";
        std::getline(std::cin, message);
        
        if (message.empty()) {
            std::cout << "Commit message cannot be empty!" << std::endl;
            return;
        }
        
        if (repo.commitChanges(message)) {
            std::cout << "[SUCCESS] Changes committed successfully!" << std::endl;
            std::cout << "  -> Commit saved to linked list structure" << std::endl;
            std::cout << "  -> Files stored in hash map" << std::endl;
            std::cout << "  -> Total commits: " << repo.getTotalCommits() << std::endl;
        } else {
            std::cout << "[ERROR] Failed to commit changes!" << std::endl;
        }
    }
    
    void viewHistory() {
        std::cout << "\n+== Commit History (Linked List Traversal) ==+" << std::endl;
        
        if (!repo.isInitialized()) {
            std::cout << "Please initialize repository first!" << std::endl;
            return;
        }
        
        std::vector<Commit*> history = repo.getCommitHistory();
        
        if (history.empty()) {
            std::cout << "No commits found!" << std::endl;
            return;
        }
        
        for (const auto& commit : history) {
            if (commit->versionId == 0) continue; // Skip initial commit
            
            std::cout << "\n+-------------------------------------------------------------+" << std::endl;
            std::cout << "| Commit #" << std::setw(3) << commit->versionId << std::setw(45) << " |" << std::endl;
            std::cout << "+-------------------------------------------------------------+" << std::endl;
            std::cout << "| Message: " << std::setw(47) << std::left << commit->message << " |" << std::endl;
            std::cout << "| Time: " << std::setw(50) << std::left << commit->timestamp << " |" << std::endl;
            std::cout << "| Files: " << std::setw(49) << std::left << std::to_string(commit->files.size()) << " |" << std::endl;
            std::cout << "+-------------------------------------------------------------+" << std::endl;
            
            // Show file details
            for (const auto& file : commit->files) {
                std::string fileInfo = "  -> " + file.first + " (" + std::to_string(file.second.length()) + " chars)";
                std::cout << "| " << std::setw(59) << std::left << fileInfo << " |" << std::endl;
            }
            
            std::cout << "+-------------------------------------------------------------+" << std::endl;
        }
    }
    
    void revertToCommit() {
        std::cout << "\n+== Revert to Commit (Stack Operation) ==+" << std::endl;
        
        if (!repo.isInitialized()) {
            std::cout << "Please initialize repository first!" << std::endl;
            return;
        }
        
        std::vector<Commit*> history = repo.getCommitHistory();
        if (history.empty()) {
            std::cout << "No commits to revert to!" << std::endl;
            return;
        }
        
        std::cout << "Available commits:" << std::endl;
        for (const auto& commit : history) {
            if (commit->versionId == 0) continue;
            std::cout << "  " << commit->versionId << ": " << commit->message 
                      << " (" << commit->files.size() << " files)" << std::endl;
        }
        
        std::cout << "\nEnter commit ID to revert to: ";
        int commitId = getChoice();
        
        if (repo.revertToCommit(commitId)) {
            std::cout << "[SUCCESS] Successfully reverted to commit #" << commitId << std::endl;
            std::cout << "  -> Working directory updated using stack operations" << std::endl;
            std::cout << "  -> Current files: " << repo.getWorkingFiles().size() << std::endl;
        } else {
            std::cout << "[ERROR] Failed to revert! Commit ID not found." << std::endl;
        }
    }
    
    void showStatus() {
        std::cout << "\n+== Repository Status & Data Structures ==+" << std::endl;
        
        std::cout << "Repository Initialized: " << (repo.isInitialized() ? "[YES]" : "[NO]") << std::endl;
        
        if (repo.isInitialized()) {
            std::cout << "Total Commits: " << repo.getTotalCommits() << std::endl;
            
            std::vector<std::string> files = repo.getWorkingFiles();
            std::cout << "Working Files: " << files.size() << std::endl;
            
            if (!files.empty()) {
                std::cout << "\n+--- Working Directory Files ---+" << std::endl;
                for (const auto& file : files) {
                    std::string content = repo.getFileContent(file);
                    std::cout << "| " << std::setw(25) << std::left << file 
                              << " | " << std::setw(6) << content.length() << " chars |" << std::endl;
                }
                std::cout << "+--------------------------------+" << std::endl;
            }
            
            Commit* current = repo.getCurrentCommit();
            if (current && current->versionId > 0) {
                std::cout << "\nCurrent Commit: #" << current->versionId << " - " << current->message << std::endl;
            }
            
            std::cout << "\n+--- Active Data Structures ---+" << std::endl;
            std::cout << "| -> Linked List: Commit chain   |" << std::endl;
            std::cout << "| -> Hash Map: File storage      |" << std::endl;
            std::cout << "| -> Stack: Undo operations      |" << std::endl;
            std::cout << "| -> File I/O: Persistent data   |" << std::endl;
            std::cout << "+-------------------------------+" << std::endl;
        }
    }
};

int main() {
    EnhancedConsoleInterface interface;
    interface.run();
    return 0;
}
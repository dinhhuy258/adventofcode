#include <iostream>
#include <fstream>
#include <vector>

#define DISK_AVAILABLE 70000000
#define DISK_SPACE_REQUIREMENT 30000000

class Entry {
protected:
  std::string name;
  int size;
  Entry(std::string name, int size) : name(name), size(size) {}

public:
  virtual ~Entry() {}

  virtual bool isDirectory() const = 0;

  std::string getName() const { return name; }

  int getSize() const { return size; }
};

class File : public Entry {
public:
  File(std::string name, int size) : Entry(name, size) {}
  ~File() {}
  bool isDirectory() const { return false; }
};

class Directory : public Entry {
private:
  std::vector<Entry *> entries;

public:
  Directory(std::string name) : Entry(name, 0) {}
  ~Directory() {
    for (auto entry : entries) {
      delete entry;
    }
  }
  bool isDirectory() const { return true; }
  void addEntry(Entry *entry) { entries.push_back(entry); }
  std::vector<Entry *> getEntries() { return entries; }
};

Directory *root = new Directory("/");

void readFs() {
  std::ifstream inputFile("input.txt");
  std::string command;

  std::vector<Directory *> directories;
  directories.push_back(root);

  while (std::getline(inputFile, command)) {
    if (command[0] == '$') {
      // command
      if (command[2] == 'c') {
        // cd command
        auto cdDir = command.substr(std::string("$ cd ").size());
        if (cdDir == "..") {
          // commands are always valid
          directories.pop_back();
        } else if (cdDir == "/") {
          directories.clear();
          directories.push_back(root);
        } else {
          auto currentDirectory = directories.back();
          // commands are always valid
          for (auto &entry : currentDirectory->getEntries()) {
            if (entry->isDirectory() && entry->getName() == cdDir) {
              directories.push_back((Directory *)entry);
              break;
            }
          }
        }
      }
    } else {
      // ls command output
      auto currentDirectory = directories.back();
      auto spaceIdx = command.find_first_of(' ');
      auto info = command.substr(0, spaceIdx);
      auto name = command.substr(spaceIdx + 1);

      if (info == "dir") {
        // directory
        auto dir = new Directory(name);
        currentDirectory->addEntry(dir);
      } else {
        auto file = new File(name, std::stoi(info));
        currentDirectory->addEntry(file);
      }
    }
  }

  inputFile.close();
}

int getTotalSizeOfDirectories(Directory *node, int &totalSize) {
  // post traversal
  auto entries = node->getEntries();

  int size = 0;

  for (auto &entry : entries) {
    if (entry->isDirectory()) {
      size += getTotalSizeOfDirectories((Directory *)entry, totalSize);
    } else {
      size += entry->getSize();
    }
  }

  if (size <= 100000) {
    totalSize += size;
  }

  return size;
}

int getFreeUpSpace(Directory *node, int &freeUpSize, int minFreeUpSpace) {
  // post traversal
  auto entries = node->getEntries();

  int size = 0;

  for (auto &entry : entries) {
    if (entry->isDirectory()) {
      size += getFreeUpSpace((Directory *)entry, freeUpSize, minFreeUpSpace);
    } else {
      size += entry->getSize();
    }
  }

  if (size >= minFreeUpSpace) {
    freeUpSize = std::min(freeUpSize, size);
  }

  return size;
}

int main() {
  readFs();

  // part 1
  int totalSize = 0;
  int usedSpace = getTotalSizeOfDirectories(root, totalSize);

  // part 2
  int freeSpace = DISK_AVAILABLE - usedSpace;
  int minFreeUpSpace = DISK_SPACE_REQUIREMENT - freeSpace;
  int freeUpSize = std::numeric_limits<int>::max();
  getFreeUpSpace(root, freeUpSize, minFreeUpSpace);

  // output
  std::cout << totalSize << std::endl;
  std::cout << freeUpSize << std::endl;

  delete root;
}

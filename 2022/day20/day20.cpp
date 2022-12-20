#include <fstream>
#include <iostream>
#include <vector>

class LinkedListNode {
public:
  long long val;
  // Pointer to next node in DLL
  LinkedListNode *next;
  // Pointer to previous node in DLL
  LinkedListNode *prev;

  LinkedListNode(long long val) : val(val), next(nullptr), prev(nullptr) {}
  LinkedListNode(long long val, LinkedListNode *next, LinkedListNode *prev)
      : val(val), next(next), prev(prev) {}
};

LinkedListNode *zeroNode;
std::vector<LinkedListNode *> nodes;

void cleanUp() {
  for (auto node : nodes) {
    delete node;
  }

  nodes.clear();
  zeroNode = nullptr;
}

void readInput() {
  LinkedListNode *nodeHead = nullptr;
  LinkedListNode *nodeTail = nullptr;

  std::ifstream inputFile("input.txt");
  std::string line;
  while (std::getline(inputFile, line)) {
    auto val = std::stoll(line);
    if (nodeHead == nullptr) {
      nodeHead = new LinkedListNode(val);
      nodeTail = nodeHead;
    } else {
      nodeTail->next = new LinkedListNode(val, nullptr, nodeTail);
      nodeTail = nodeTail->next;
    }
    if (val == 0) {
      zeroNode = nodeTail;
    }
    nodes.push_back(nodeTail);
  }

  nodeHead->prev = nodeTail;
  nodeTail->next = nodeHead;

  inputFile.close();
}

long long getVal(LinkedListNode *node, int count) {
  while (count > 0) {
    node = node->next;
    --count;
  }

  return node->val;
}

void mixing() {
  int nodesSize = nodes.size();
  for (auto node : nodes) {
    long long val = node->val;
    bool goRight = val > 0;
    val %= (nodesSize - 1);
    val = std::abs(val);
    if (val == 0) {
      // do nothing
      continue;
    }

    auto prevNode = node->prev;
    auto nextNode = node->next;
    prevNode->next = nextNode;
    nextNode->prev = prevNode;

    if (goRight) {
      --val;
    }

    auto *p = goRight ? nextNode : prevNode;
    for (int i = 0; i < val; ++i) {
      if (goRight) {
        p = p->next;
      } else {
        p = p->prev;
      }
    }

    // insert node here
    auto pNext = p->next;
    p->next = node;
    node->next = pNext;
    node->prev = p;
    pNext->prev = node;
  }
}

int main() {
  // read input
  readInput();
  int nodesSize = nodes.size();

  // part 1
  mixing();

  std::cout << getVal(zeroNode, 1000 % nodesSize) +
                   getVal(zeroNode, 2000 % nodesSize) +
                   getVal(zeroNode, 3000 % nodesSize)
            << std::endl;
  cleanUp();

  // re-read input (prepare for part 2)
  readInput();

  // part 2
  for (auto node : nodes) {
    node->val *= 811589153;
  }

  for (int i = 0; i < 10; ++i) {
    mixing();
  }

  std::cout << getVal(zeroNode, 1000 % nodesSize) +
                   getVal(zeroNode, 2000 % nodesSize) +
                   getVal(zeroNode, 3000 % nodesSize)
            << std::endl;
  cleanUp();
}

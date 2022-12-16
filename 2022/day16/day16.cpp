#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

std::vector<std::string> split(const std::string &str,
                               const std::string &delimeter) {
  std::vector<std::string> strs;
  char *token = std::strtok(const_cast<char *>(str.c_str()), delimeter.c_str());
  while (token != nullptr) {
    strs.push_back(token);
    token = std::strtok(nullptr, delimeter.c_str());
  }

  return strs;
}

class Valve {
private:
  std::string name;
  int flowRate;
  std::vector<std::string> desValveNames;

public:
  Valve(std::string name, int flowRate, std::vector<std::string> desValves)
      : name(std::move(name)), flowRate(flowRate),
        desValveNames(std::move(desValves)){};
  const std::string &getName() const { return name; }
  int getFlowRate() const { return flowRate; }
  const std::vector<std::string> &getDesValveNames() const {
    return desValveNames;
  }
};

std::vector<Valve *> valves;
Valve *startValve;
std::unordered_map<std::string, Valve *> valvesMap;
std::unordered_map<std::string, int> shortedPaths;

void updateShortestPath(Valve *valve) {
  std::queue<Valve *> queue;
  std::unordered_set<std::string> visited;
  visited.insert(valve->getName());
  queue.push(valve);
  int step = 0;

  while (!queue.empty()) {
    int queueSize = queue.size();
    while (queueSize > 0) {
      auto v = queue.front();
      queue.pop();
      if (v->getName() != valve->getName()) {
        shortedPaths[valve->getName() + "-" + v->getName()] = step;
      }

      for (auto name : v->getDesValveNames()) {
        if (visited.find(name) != visited.end()) {
          continue;
        }

        visited.insert(name);
        queue.push(valvesMap[name]);
      }

      --queueSize;
    }

    ++step;
  }
}

void readValves() {
  std::ifstream inputFile("input.txt");
  std::string line;
  char name[16];
  char desValves[256];
  int flowRate;
  std::vector<Valve *> vs;
  while (std::getline(inputFile, line)) {
    if (line.find(" tunnel leads to valve ") != std::string::npos) {
      std::sscanf(line.c_str(),
                  "Valve %s has flow rate=%d; tunnel leads to valve %[^\t\n]",
                  name, &flowRate, desValves);
    } else {
      std::sscanf(line.c_str(),
                  "Valve %s has flow rate=%d; tunnels lead to valves %[^\t\n]",
                  name, &flowRate, desValves);
    }
    vs.push_back(
        new Valve(std::string(name), flowRate, split(desValves, ", ")));
    valvesMap[std::string(name)] = vs.back();
    if (std::string(name) == "AA") {
      startValve = vs.back();
    }
    if (flowRate != 0) {
      valves.push_back(vs.back());
    }
  }
  inputFile.close();

  // calculate shortest distance between valves
  for (auto v : vs) {
    updateShortestPath(v);
  }
}

int countReleasedPressure(Valve *v, std::unordered_set<std::string> &open,
                          int minutes) {
  if (minutes <= 1) {
    return 0;
  }

  int pressure = 0;

  for (auto desV : valves) {
    if (open.find(desV->getName()) != open.end()) {
      continue;
    }

    auto path = v->getName() + "-" + desV->getName();
    if (shortedPaths.find(path) == shortedPaths.end()) {
      continue;
    }

    int cost = shortedPaths[path];

    open.insert(desV->getName());

    pressure = std::max(
        pressure, desV->getFlowRate() * (minutes - cost - 1) +
                      countReleasedPressure(desV, open, minutes - cost - 1));

    // back track
    open.erase(desV->getName());
  }

  return pressure;
}

int countReleasedPressure() {
  std::unordered_set<std::string> open;
  return countReleasedPressure(startValve, open, 30);
}

std::unordered_map<std::string, int> memo;
int countReleasedPressureWithElephantSupport(
    Valve *v, Valve *e, std::unordered_set<std::string> &open, int yourMinutes,
    int elephantMinutes) {

  std::string state = "";
  for (const auto &vv : valves) {
    if (open.find(vv->getName()) == open.end()) {
      state += vv->getName() + "-";
    }
  }
  std::string key = v->getName() + "-" + e->getName() + "-" +
                    std::to_string(yourMinutes) + "-" +
                    std::to_string(elephantMinutes) + "-" + state;
  if (memo.find(key) != memo.end()) {
    return memo[key];
  }

  std::vector<Valve *> yourValves;
  std::vector<Valve *> elephantValves;
  for (auto &desV : valves) {
    if (open.find(desV->getName()) != open.end()) {
      continue;
    }

    auto yourPath = v->getName() + "-" + desV->getName();
    if (shortedPaths.find(yourPath) != shortedPaths.end()) {
      int yourCost = shortedPaths[yourPath];
      if (yourMinutes - yourCost - 1 > 0) {
        yourValves.push_back(desV);
      }
    }

    auto elephantPath = e->getName() + "-" + desV->getName();
    if (shortedPaths.find(elephantPath) != shortedPaths.end()) {
      int elephantCost = shortedPaths[elephantPath];
      if (elephantMinutes - elephantCost - 1 > 0) {
        elephantValves.push_back(desV);
      }
    }
  }

  int pressure = 0;

  if (!elephantValves.empty() && !yourValves.empty()) {
    for (auto yv : yourValves) {
      auto yourPath = v->getName() + "-" + yv->getName();
      int yourCost = shortedPaths[yourPath];
      int yourPressure = yv->getFlowRate() * (yourMinutes - yourCost - 1);

      open.insert(yv->getName());
      for (auto ev : elephantValves) {
        if (ev->getName() == yv->getName()) {
          continue;
        }

        auto elephantPath = e->getName() + "-" + ev->getName();
        int elephantCost = shortedPaths[elephantPath];
        int elephantPressure =
            ev->getFlowRate() * (elephantMinutes - elephantCost - 1);

        open.insert(ev->getName());

        pressure =
            std::max(pressure, yourPressure + elephantPressure +
                                   countReleasedPressureWithElephantSupport(
                                       yv, ev, open, yourMinutes - yourCost - 1,
                                       elephantMinutes - elephantCost - 1));

        open.erase(ev->getName());
      }

      open.erase(yv->getName());
    }
  }

  if (yourValves.empty()) {
    for (auto ev : elephantValves) {
      auto elephantPath = v->getName() + "-" + ev->getName();
      int elephantCost = shortedPaths[elephantPath];
      int elephantPressure =
          ev->getFlowRate() * (elephantMinutes - elephantCost - 1);

      open.insert(ev->getName());

      pressure = std::max(
          pressure, elephantPressure + countReleasedPressureWithElephantSupport(
                                           v, ev, open, yourMinutes,
                                           elephantMinutes - elephantCost - 1));

      open.erase(ev->getName());
    }
  }

  if (elephantValves.empty()) {
    for (auto yv : yourValves) {
      open.insert(yv->getName());
      auto yourPath = v->getName() + "-" + yv->getName();
      int yourCost = shortedPaths[yourPath];
      int yourPressure = yv->getFlowRate() * (yourMinutes - yourCost - 1);

      pressure = std::max(
          pressure, yourPressure + countReleasedPressureWithElephantSupport(
                                       yv, e, open, yourMinutes - yourCost - 1,
                                       elephantMinutes));

      open.erase(yv->getName());
    }
  }

  memo[key] = pressure;

  return pressure;
}

int countReleasedPressureWithElephantSupport() {
  std::unordered_set<std::string> open;
  return countReleasedPressureWithElephantSupport(startValve, startValve, open,
                                                  26, 26);
}

int main() {
  // In this algorithm, I assume that the starting valve always has a flow value
  // of 0. However, it does not work if the starting valve's flow is not equal
  // to 0. To make it work, we need to modify the algorithm slightly.

  // read the input and preperation
  readValves();

  // part 1
  std::cout << countReleasedPressure() << std::endl;

  // part 2 - timeout
  std::cout << countReleasedPressureWithElephantSupport() << std::endl;
}

#include <fstream>
#include <iostream>
#include <queue>
#include <set>
#include <tuple>
#include <vector>

class Blueprint {
public:
  int id;
  // cost to make ore robot
  int oreCost1;
  // cost to make clay robot
  int oreCost2;
  // cost to make obsidian robot
  int oreCost3;
  int clayCost;
  // cost to make geode robot
  int oreCost4;
  int obsidianCost;

  Blueprint(int id, int oreCost1, int oreCost2, int oreCost3, int oreCost4,
            int clayCost, int obsidianCost)
      : id(id), oreCost1(oreCost1), oreCost2(oreCost2), oreCost3(oreCost3),
        oreCost4(oreCost4), clayCost(clayCost), obsidianCost(obsidianCost) {}
};

std::vector<Blueprint> blueprints;

void readInput() {
  std::ifstream inputFile("input.txt");
  std::string line;
  int id, oreCost1, oreCost2, oreCost3, oreCost4, clayCost, obsidianCost;
  while (std::getline(inputFile, line)) {
    std::sscanf(line.c_str(),
                "Blueprint %d: Each ore robot costs %d ore. Each clay robot "
                "costs %d ore. Each obsidian robot costs %d ore and %d clay. "
                "Each geode robot costs %d ore and %d obsidian.",
                &id, &oreCost1, &oreCost2, &oreCost3, &clayCost, &oreCost4,
                &obsidianCost);
    blueprints.push_back(Blueprint(id, oreCost1, oreCost2, oreCost3, oreCost4,
                                   clayCost, obsidianCost));
  }
  inputFile.close();
}

int getNumberOfGeodes(Blueprint &blueprint, int minutes) {
  int maxGeodes = 0;
  std::set<std::tuple<int /* ore */, int /* clay */, int /* obsidian */,
                      int /* geodes */, int /* ore rate */, int /* clay rate */,
                      int /* obsidian rate */, int /* geode rate */,
                      int /* minutes left */>>
      memorization;
  auto startState = std::make_tuple(0, 0, 0, 0, 1, 0, 0, 0, minutes);
  std::queue<std::tuple<int /* ore */, int /* clay */, int /* obsidian */,
                        int /* geodes */, int /* ore rate */,
                        int /* clay rate */, int /* obsidian rate */,
                        int /* geode rate */, int /* minutes left */>>
      statesQueue;
  statesQueue.push(startState);
  while (!statesQueue.empty()) {
    auto state = statesQueue.front();
    statesQueue.pop();
    int ore, clay, obsidian, geode, oreRate, clayRate, obsidianRate, geodeRate,
        minutes;
    std::tie(ore, clay, obsidian, geode, oreRate, clayRate, obsidianRate,
             geodeRate, minutes) = state;
    maxGeodes = std::max(maxGeodes, geode);

    if (minutes <= 0) {
      continue;
    }

    int core =
        std::max(blueprint.oreCost1,
                 std::max(blueprint.oreCost2,
                          std::max(blueprint.oreCost3, blueprint.oreCost4)));
    oreRate = std::min(oreRate, core);
    clayRate = std::min(clayRate, blueprint.clayCost);
    obsidianRate = std::min(obsidianRate, blueprint.obsidianCost);

    ore = std::min(ore, minutes * core - oreRate * (minutes - 1));
    clay =
        std::min(clay, minutes * blueprint.clayCost - clayRate * (minutes - 1));
    obsidian = std::min(obsidian, minutes * blueprint.obsidianCost -
                                      obsidianRate * (minutes - 1));

    state = std::make_tuple(ore, clay, obsidian, geode, oreRate, clayRate,
                            obsidianRate, geodeRate, minutes);
    if (memorization.find(state) != memorization.end()) {
      continue;
    }

    memorization.insert(state);

    // do nothing
    statesQueue.push(std::make_tuple(ore + oreRate, clay + clayRate,
                                     obsidian + obsidianRate, geode + geodeRate,
                                     oreRate, clayRate, obsidianRate, geodeRate,
                                     minutes - 1));

    if (ore >= blueprint.oreCost1) {
      statesQueue.push(std::make_tuple(ore + oreRate - blueprint.oreCost1,
                                       clay + clayRate, obsidian + obsidianRate,
                                       geode + geodeRate, oreRate + 1, clayRate,
                                       obsidianRate, geodeRate, minutes - 1));
    }
    if (ore >= blueprint.oreCost2) {
      statesQueue.push(std::make_tuple(ore + oreRate - blueprint.oreCost2,
                                       clay + clayRate, obsidian + obsidianRate,
                                       geode + geodeRate, oreRate, clayRate + 1,
                                       obsidianRate, geodeRate, minutes - 1));
    }
    if (ore >= blueprint.oreCost3 && clay >= blueprint.clayCost) {
      statesQueue.push(
          std::make_tuple(ore + oreRate - blueprint.oreCost3,
                          clay + clayRate - blueprint.clayCost,
                          obsidian + obsidianRate, geode + geodeRate, oreRate,
                          clayRate, obsidianRate + 1, geodeRate, minutes - 1));
    }
    if (ore >= blueprint.oreCost4 && obsidian >= blueprint.obsidianCost) {
      statesQueue.push(std::make_tuple(
          ore + oreRate - blueprint.oreCost4, clay + clayRate,
          obsidian + obsidianRate - blueprint.obsidianCost, geode + geodeRate,
          oreRate, clayRate, obsidianRate, geodeRate + 1, minutes - 1));
    }
  }

  return maxGeodes;
}

int main() {
  // read input
  readInput();

  // part 1
  int qualityLevels = 0;
  for (auto &blueprint : blueprints) {
    auto geodes = getNumberOfGeodes(blueprint, 24);
    qualityLevels += geodes * blueprint.id;
  }
  std::cout << qualityLevels << std::endl;

  // part 2
  int geodes = 1;
  for (int i = 0; i < 3; ++i) {
    geodes *= getNumberOfGeodes(blueprints[i], 32);
  }
  std::cout << geodes << std::endl;
}

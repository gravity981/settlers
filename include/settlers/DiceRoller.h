#pragma once

#include <random>

class DiceRoller
{
 private:
  int m_numberOfDices;
  int m_min;
  int m_max;
  std::default_random_engine m_generator;
  std::uniform_int_distribution<int> m_distribution;
 public:
  DiceRoller(int numberOfDices, int min, int max, unsigned long seed);
  virtual ~DiceRoller();

  bool initFromFile(const std::string& filePath);

  void setSeed(unsigned long seed);

  int possibleCombinationsForValue(int value) const;

  int roll();

  static unsigned long randomSeed();

 private:
  int findCombinationForValue(int diceNr, int sum, int value) const;
};

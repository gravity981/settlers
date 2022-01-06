#include "settlers/DiceRoller.h"

#include <spdlog/spdlog.h>

#include <chrono>
#include <cmath>
#include <functional>
#include <nlohmann/json.hpp>

#include "JsonUtil.h"

DiceRoller::DiceRoller(int numberOfDices, int min, int max, unsigned long seed)
    : m_numberOfDices(numberOfDices),
      m_min(min),
      m_max(max),
      m_generator(seed),
      m_distribution(m_min, m_max)
{
}

DiceRoller::~DiceRoller()
{
}
int DiceRoller::possibleCombinationsForValue(int value) const
{
  return findCombinationForValue(0, 0, value);
}

int DiceRoller::findCombinationForValue(int diceNr, int sum, int value) const
{
  int count = 0;
  diceNr++;
  for (int i = m_min; i <= m_max; i++)
  {
    if (diceNr == m_numberOfDices)
    {
      if (sum + i == value)
      {
        count++;
      }
    }
    else if (diceNr < m_numberOfDices)
    {
      count += findCombinationForValue(diceNr, sum + i, value);
    }
    else
    {
      return 0;
    }
  }
  return count;
}
int DiceRoller::roll()
{
  int value = 0;
  for (int i = 0; i < m_numberOfDices; i++)
  {
    value += m_distribution(m_generator);
  }
  return value;
}
unsigned long DiceRoller::randomSeed()
{
  std::random_device randomDevice;
  auto entropy = randomDevice.entropy();
  unsigned long seed = 0;

  // use timestamp based random seed if random_device cannot provide enough entropy
  if (entropy == 0)
  {
    SPDLOG_INFO("create seed from timestamp because random_device entropy is only {}", entropy);
    seed = static_cast<unsigned long>(std::chrono::system_clock::now().time_since_epoch().count());
  }
  else
  {
    SPDLOG_INFO("create seed from random_device, its entropy is {}", entropy);
    seed = randomDevice();
  }
  SPDLOG_INFO("random seed is {}", seed);
  return seed;
}
bool DiceRoller::initFromFile(const std::string& filePath)
{
  nlohmann::json jsonData;
  if (!JsonUtil::readFile(filePath, jsonData))
  {
    return false;
  }
  try
  {
    auto numberOfDices = jsonData["dices"].get<int>();
    auto minValue = jsonData["min"].get<int>();
    auto maxValue = jsonData["max"].get<int>();
    m_numberOfDices = numberOfDices;
    m_min = minValue;
    m_max = maxValue;
  }
  catch (nlohmann::json::type_error& ex)
  {
    SPDLOG_ERROR("failed to read dice config: {}", ex.what());
    return false;
  }
  return true;
}
void DiceRoller::setSeed(unsigned long seed)
{
  m_generator.seed(seed);
}

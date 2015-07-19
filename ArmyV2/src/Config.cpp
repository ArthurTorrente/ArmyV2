#include "Config.hpp"

std::shared_ptr<Config> Config::m_instance(nullptr);

std::shared_ptr<Config> Config::getInstance()
{
    if (!m_instance)
    {
        m_instance = std::unique_ptr<Config>(new Config());
    }

    return m_instance;
}

unsigned int Config::getWidth() const
{
    return m_width;
}

unsigned int Config::getHeight() const
{
    return m_height;
}

void Config::setWidth(unsigned int w)
{
    m_width = w;
}

void Config::setHeight(unsigned int h)
{
    m_height = h;
}

unsigned int Config::getChooseNodeRatio() const
{
    return m_chooseNodeRatio;
}

unsigned int Config::getMaxNodeRatio() const
{
    return m_maxNodeRatio;
}

bool Config::log() const
{
    return m_log;
}

void Config::setChooseNodeRatio(unsigned int ratio)
{
    m_chooseNodeRatio = ratio;
}

void Config::setMaxNodeRatio(unsigned int max)
{
    m_maxNodeRatio = max;
}

void Config::setLog(bool log)
{
    m_log = log;
}

Config::Config()
    : m_width(100),
    m_height(100),
    m_maxNodeRatio(100),
    m_chooseNodeRatio(75),
    m_log(false)
{}
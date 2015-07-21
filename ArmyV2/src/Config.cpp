#include "Config.hpp"

#include "tools.hpp"

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

bool Config::useOldIa() const
{
    return m_useOldIa;
}

void Config::setChooseNodeRatio(unsigned int ratio)
{
    m_chooseNodeRatio = tools::clamp<unsigned int>(0, m_maxNodeRatio, ratio);
}

void Config::setMaxNodeRatio(unsigned int max)
{
    m_maxNodeRatio = max;
}

void Config::setLog(bool log)
{
    m_log = log;
}

void Config::setUseOldIa(bool use)
{
    m_useOldIa = use;
}

unsigned int Config::I() const
{
    return m_I;
}

unsigned int Config::T() const
{
    return m_T;
}

unsigned int Config::N() const
{
    return m_N;
}

unsigned int Config::X() const
{
    return m_X;
}

unsigned int Config::Y() const
{
    return m_Y;
}

void Config::setI(unsigned int i)
{
    m_I = i;
}

void Config::setT(unsigned int t)
{
    m_T = t;
}

void Config::setN(unsigned int n)
{
    m_N = n;
}

void Config::setX(unsigned int x)
{
    m_X = x;
}

void Config::setY(unsigned int y)
{
    m_Y = y;
}

Config::Config()
    : m_width(100),
    m_height(100),
    m_maxNodeRatio(100),
    m_chooseNodeRatio(75),
    m_log(false),
    m_useOldIa(false),
    m_I(10),
    m_T(100),
    m_N(20),
    m_X(10),
    m_Y(100)
{}
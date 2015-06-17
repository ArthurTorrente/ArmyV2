#include "Config.h"

std::shared_ptr<Config> Config::m_instance(nullptr);

std::shared_ptr<Config> Config::getInsance()
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
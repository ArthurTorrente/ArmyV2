#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <memory>

class Config
{
public:
    static std::shared_ptr<Config> getInsance();

    unsigned int getWidth() const;
    unsigned int getHeight() const;

    void setWidth(unsigned int w);
    void setHeight(unsigned int h);

protected:
    static std::shared_ptr<Config> m_instance;

    unsigned int m_width;
    unsigned int m_height;
};

#endif //_CONFIG_H_
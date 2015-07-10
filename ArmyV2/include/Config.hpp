#ifndef _CONFIG_H_
#define _CONFIG_H_

#pragma warning(push, 0)
#include <memory>
#pragma warning(pop)

/**
 * Singleton class use for store all the parameter of application
 */
class Config
{
public:
    /* Getter of instance of Config */
    static std::shared_ptr<Config> getInsance();

    /** Getter of Size of Board */
    unsigned int getWidth() const;
    unsigned int getHeight() const;

    /** Setter for size of board */
    void setWidth(unsigned int w);
    void setHeight(unsigned int h);

protected:
    /** Instance of config */
    static std::shared_ptr<Config> m_instance;

    /** Attributs of size of board */
    unsigned int m_width;
    unsigned int m_height;
};

#endif //_CONFIG_H_
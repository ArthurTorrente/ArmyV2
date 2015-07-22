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
    static std::shared_ptr<Config> getInstance();

    /** Getter of Size of Board */
    unsigned int getWidth() const;
    unsigned int getHeight() const;

    /* Getter of choose ratio*/
    unsigned int getChooseNodeRatio() const;
    unsigned int getMaxNodeRatio() const;

    bool log() const;

    bool useOldIa() const;

    /** Setter for size of board */
    void setWidth(unsigned int w);
    void setHeight(unsigned int h);

    /* setter of choose ratio */
    void setChooseNodeRatio(unsigned int);
    /* Setter of max ratio value */
    void setMaxNodeRatio(unsigned int);

    void setLog(bool);

    void setUseOldIa(bool use);

    /* Getter and setter of application parameters */
    unsigned int I() const;
    unsigned int T() const;
    unsigned int N() const;
    unsigned int X() const;
    unsigned int Y() const;

    void setI(unsigned int);
    void setT(unsigned int);
    void setN(unsigned int);
    void setX(unsigned int);
    void setY(unsigned int);
    /* ===================== */
protected:
    /** Instance of config */
    static std::shared_ptr<Config> m_instance;

    /** Attributs of size of board */
    unsigned int m_width;
    unsigned int m_height;

    /* Max value of rand */
    unsigned int m_maxNodeRatio;

    /* Ratio for choose decision or action node in random tree IA*/
    unsigned int m_chooseNodeRatio;

    /* Write into console the logs of battle */
    bool m_log;

    /* flag use the old IA system */
    bool m_useOldIa;

    /**
     * Application param
     */
    unsigned int m_I;
    unsigned int m_T;
    unsigned int m_N;
    unsigned int m_X;
    unsigned int m_Y;

    Config();
};

#endif //_CONFIG_H_
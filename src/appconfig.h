#ifndef APPCONFIG_H
#define APPCONFIG_H

#include <QString>

class AppConfig {
public:
    static QString findConfigPath(int argc, char *argv[]);
};

#endif

#include "appconfig.h"
#include <QDir>
#include <QFile>
#include <QDebug>

QString AppConfig::findConfigPath(int argc, char *argv[]) {
    QString configPath;
    
    if (argc > 1) {
        configPath = QString(argv[1]);
        if (QFile::exists(configPath)) {
            qDebug() << "Using config from argument:" << configPath;
            return configPath;
        }
        qWarning() << "Config file not found:" << configPath;
    }
    
    QStringList searchPaths = {
        "config.json",
        QDir::currentPath() + "/config.json",
    };
    
    for (const QString& path : searchPaths) {
        if (QFile::exists(path)) {
            qDebug() << "Found config at:" << path;
            return path;
        }
    }
    
    qWarning() << "Failed to load config.json from any location";
    qWarning() << "Searched paths:" << searchPaths;
    return QString();
}

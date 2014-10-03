/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNCONFIGURE_H
#define KNCONFIGURE_H

#include <QJsonArray>
#include <QJsonObject>

#include <QObject>

class KNConfigure : public QObject
{
    Q_OBJECT
public:
    static KNConfigure *instance();
    void loadConfigure();
    void saveConfigure();
    void setSystemData(const QString &key,
                       const QJsonValue &value);
    void setUserData(const QString &module,
                     const QString &key,
                     const QJsonValue &value);
    QVariant systemData(const QString &key);
    QString configurePath() const;
    void setConfigurePath(const QString &configurePath);

signals:

public slots:

private:
    static KNConfigure *m_instance;
    explicit KNConfigure(QObject *parent = 0);

    void checkConfigureFolder();
    void loadConfigureFromFile(const QString &filePath,
                               QJsonObject &configureObject);
    void saveConfigureToFile(const QString &filePath,
                             const QJsonObject &configureObject);

    QString m_configurePath,
            m_systemConfigurePath,
            m_userConfigurePath;
    QJsonObject m_systemConfigure, m_userConfigure;
};

#endif // KNCONFIGURE_H
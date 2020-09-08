#pragma once
#ifndef ANDROIDSERVICEWRAPPER_H
#define ANDROIDSERVICEWRAPPER_H

#include <QObject>

class AndroidServiceWrapper : public QObject
{
    Q_OBJECT
protected:
    bool available;
    AndroidServiceWrapper();
    static AndroidServiceWrapper * _instance;
public:
    static AndroidServiceWrapper& instance();
    bool isAvailable() const {return available;};
    void sendEmailIntent(QString subj, QString text);
};

#endif // ANDROIDSERVICEWRAPPER_H

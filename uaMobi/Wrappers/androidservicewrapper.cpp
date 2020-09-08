#include "androidservicewrapper.h"
#ifdef Q_OS_ANDROID
#include <QtAndroidExtras/QAndroidJniObject>
#endif

AndroidServiceWrapper::AndroidServiceWrapper()
{
#ifdef Q_OS_ANDROID
    available = true;
#else
    available = false;
#endif
}

AndroidServiceWrapper &AndroidServiceWrapper::instance()
{
    if (_instance == Q_NULLPTR)
    {
        _instance = new AndroidServiceWrapper();
    }
    return *_instance;
}
AndroidServiceWrapper * AndroidServiceWrapper::_instance = Q_NULLPTR;
void AndroidServiceWrapper::sendEmailIntent(QString subj, QString text)
{
    if (!available)
        return;
#ifdef Q_OS_ANDROID
    QAndroidJniObject JSubj = QAndroidJniObject::fromString(subj);
    QAndroidJniObject JText = QAndroidJniObject::fromString(text);
    QAndroidJniObject::callStaticMethod<void>("una/unamd/unaretail/EmailIntenter",
                                              "SendIntent",
                                              "(Ljava/lang/String;Ljava/lang/String;)V",
                                              JSubj.object<jstring>(),
                                              JText.object<jstring>()
        );
#endif
}

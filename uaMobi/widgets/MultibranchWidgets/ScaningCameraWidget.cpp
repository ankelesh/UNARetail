#include "ScaningCameraWidget.h"
#ifdef CAMERA_SUPPORT
#include <QtQuick/QQuickItem>
#include <QQuickView>
void ScaningCameraWidget::show()
{
	inframedWidget::show();
}
void ScaningCameraWidget::barcodeObtained(QString str)
{
    emit hasBarcode(str);
}
ScaningCameraWidget::ScaningCameraWidget(QWidget* parent)
    : AbstractCameraWidget("qrc:///res/ScaningCameraSurface.qml", parent)
{
	auto root2 = qmlCamera->rootObject();
	auto filter = root2->findChild<QObject*>("FilterZX");
	QObject::connect(filter, SIGNAL(bcArr(QString)), this, SLOT(barcodeObtained(QString)));
}
#endif

# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Tools.
# ------------------------------------------------------

QT       += core gui sql network xml multimedia multimediawidgets quickwidgets qml quick concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
android : QT += androidextras bluetooth
CONFIG += release
CONFIG += qzxing_qml
CONFIG += qzxing_multimedia
include(qzxing-master/src/qzxing.pri)

TARGET = UNARetail
TEMPLATE = app
DEFINES += QT_VERSION5X CAMERA_SUPPORT DEBUG
# Default rules for deployment.
DEPENDPATH += .
TRANSLATIONS += translations/unaretail_ru.ts \
    translations/unaretail_ro.ts \
    translations/unaretail_en.ts

QMAKE_LFLAGS += -lstdc++
QML_IMPORT_PATH = E:\Lib\Qt\5.12.5
QML_DESIGNER_IMPORT_PATH =E:\Lib\Qt\5.12.5
RESOURCES += resources.qrc
INCLUDEPATH += submodules/UNAQtCommons

DISTFILES += \
    android-sources/AndroidManifest.xml \
    android-sources/build.gradle \
    android-sources/gradle/wrapper/gradle-wrapper.jar \
    android-sources/gradle/wrapper/gradle-wrapper.properties \
    android-sources/gradlew \
    android-sources/gradlew.bat \
    android-sources/res/drawable-hdpi/icon.png \
    android-sources/res/drawable-ldpi/icon.png \
    android-sources/res/drawable-mdpi/icon.png \ \
    android-sources/res/values/libs.xml \
    android-sources/src/una/unamd/QBroadcastCatcher.java
ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android-sources

HEADERS += \
    Datacore/AbsEntity.h \
    Datacore/BarcodeEntity.h \
    Datacore/DataEntities.h \
    Datacore/EntityQuickHash.h \
    Datacore/PricedBarcodeEntity.h \
    Datacore/PrinterTemplateEntity.h \
    Datacore/ProductEntity.h \
    Datacore/ShortBarcodeEntity.h \
    Datacore/UtilityEntities.h \
    Wrappers/FiscalPrinterWrapper.h \
    dataFormats/dataformatcore.h \
    dataFormats/dataformater.h \
    dataFormats/formats.h \
    dataproviders/BackupingEngine.h \
    dataproviders/DataAsyncLoader.h \
    dataproviders/ModesDescriptions.h \
    dataproviders/TableHandlers.h \
    dataproviders/sqldataprovider.h \
    externalCommunication/AsyncRequestEngine.h \
    externalCommunication/communicationCore.h \
    externalCommunication/tohttp.h \
    externalCommunication/tolocalmemory.h \
    submodules/UNAQtCommons/barcodeHandling/BarcodeObserver.h \
    submodules/UNAQtCommons/consts/DatetimeConstants.h \
    submodules/UNAQtCommons/debug/debugtrace.h \
    submodules/UNAQtCommons/settings/CommonAppSettings.h \
    submodules/UNAQtCommons/widgets/ControlsWidgets/BarcodeControl.h \
    submodules/UNAQtCommons/widgets/ControlsWidgets/ControlTranslator.h \
    submodules/UNAQtCommons/widgets/ControlsWidgets/DateTimeControl.h \
    submodules/UNAQtCommons/widgets/ControlsWidgets/LabelControl.h \
    submodules/UNAQtCommons/widgets/ControlsWidgets/QuantityControl.h \
    submodules/UNAQtCommons/widgets/ControlsWidgets/StringControl.h \
    submodules/UNAQtCommons/widgets/ControlsWidgets/abs_control.h \
    submodules/UNAQtCommons/widgets/Scaling/ScalingUtilities.h \
    submodules/UNAQtCommons/widgets/UtilityElements/AndroidFileDialog.h \
    submodules/UNAQtCommons/widgets/UtilityElements/BigButtonsSpinbox.h \
    submodules/UNAQtCommons/widgets/UtilityElements/ExtendedDialogs.h \
    submodules/UNAQtCommons/widgets/UtilityElements/ExtendedLabels.h \
    submodules/UNAQtCommons/widgets/UtilityElements/MegaIconButton.h \
    submodules/UNAQtCommons/widgets/UtilityElements/SwitchableIMDecorator.h \
    submodules/UNAQtCommons/widgets/parents/abstractNodeInterface.h \
    submodules/UNAQtCommons/widgets/parents/inframedWidget.h \
    submodules/UNAQtCommons/wrappers/DirectConnectionPrinters/AbsPrinterWrapper.h \
    submodules/UNAQtCommons/wrappers/DirectConnectionPrinters/AndroidBluetoothPrinterWrapper.h \
    submodules/UNAQtCommons/wrappers/DirectConnectionPrinters/FTRCOMPrinterWrapper.h \
    submodules/UNAQtCommons/wrappers/DirectConnectionPrinters/NullPrinterWrapper.h \
    submodules/UNAQtCommons/wrappers/DirectConnectionPrinters/PrinterWrapperFactory.h \
    submodules/UNAQtCommons/wrappers/DirectConnectionPrinters/tty/tty.h \
    submodules/UNAQtCommons/wrappers/Platform/Android/QBroadcastCatcher.h \
    submodules/UNAQtCommons/wrappers/Platform/Android/androidservicewrapper.h \
    submodules/UNAQtCommons/wrappers/Sound/AbsSoundWrapper.h \
    submodules/UNAQtCommons/wrappers/Sound/ModernQtSoundWrapper.h \
    submodules/UNAQtCommons/wrappers/Sound/PhononSoundWrapper.h \
    submodules/UNAQtCommons/wrappers/Sound/SoundEffectPlayer.h \
    submodules/UNAQtCommons/wrappers/Sound/WinCEUnpackWrapper.h \
    widgets/ControlsMiniwidgets/ControlManager.h \
    widgets/CoreWidget.h \
    widgets/DatabaseOperationBranch/DatabaseOperationsWidget.h \
    widgets/ExtendedDelegates/ExtraSelectionDelegate.h \
    widgets/InventoryBranch/InventoryWidget.h \
    widgets/InvoiceBranch/InvoiceBranchWidget.h \
    widgets/MultibranchWidgets/BarcodeRedactingWidget.h \
    widgets/MultibranchWidgets/PrintingReceiptScaningWidget.h \
    widgets/MultibranchWidgets/ReceiveWidget.h \
    widgets/MultibranchWidgets/Redactors/Abs_Redactor.h \
    widgets/MultibranchWidgets/Redactors/BarcodeRedactorForm.h \
    widgets/MultibranchWidgets/Redactors/PricedBarcodeRedactorForm.h \
    widgets/MultibranchWidgets/Redactors/RoleBoundRedactor.h \
    widgets/MultibranchWidgets/RestScaningWidget.h \
    widgets/MultibranchWidgets/RoleBoundScaningWidget.h \
    widgets/MultibranchWidgets/ScanedStorageWidget.h \
    widgets/MultibranchWidgets/ScaningCameraWidget.h \
    widgets/MultibranchWidgets/ScaningWidget.h \
    widgets/MultibranchWidgets/SendSettings.h \
    widgets/MultibranchWidgets/SendingDataPickerWidget.h \
    widgets/MultibranchWidgets/UpdateableScaningWidget.h \
    widgets/MultibranchWidgets/innerSettingsForm.h \
    widgets/PriceBranch/PriceBranchRootWidget.h \
    widgets/PriceBranch/PriceBranchScaningWidget.h \
    widgets/PriceBranch/PriceBranchWidget.h \
    widgets/PriceBranch/PriceScaningWidget.h \
    widgets/SalesAccountingBranch/SalesAccounting.h \
    widgets/SearchBranch/SearchWidget.h \
    widgets/SettingsForm.h \
    widgets/SettingsWidgets/DatabaseLoadSettings.h \
    widgets/SettingsWidgets/DatabaseSettings.h \
    widgets/SettingsWidgets/EmailSettings.h \
    widgets/SettingsWidgets/ExtraScaningSettings.h \
    widgets/SettingsWidgets/ExtraSendSettings.h \
    widgets/SettingsWidgets/GeneralSettings.h \
    widgets/SettingsWidgets/LoginSettings.h \
    widgets/SettingsWidgets/PrinterConnectionSettings.h \
    widgets/SettingsWidgets/PrinterSettings.h \
    widgets/SettingsWidgets/PrinterTemplatesSettings.h \
    widgets/SettingsWidgets/QuickSendSettings.h \
    widgets/SettingsWidgets/ScaningSettings.h \
    widgets/SettingsWidgets/SearchDatabaseSettings.h \
    widgets/SettingsWidgets/SerializationSettings.h \
    widgets/SettingsWidgets/SpecialActionsSettings.h \
    widgets/SettingsWidgets/TaxInvoiceSettings.h \
    widgets/SettingsWidgets/ViewSettings.h \
    widgets/SimpleBranch/SimpleBranchWidget.h \
    widgets/SupplyBranch/SuppliesWidget.h \
    widgets/TagPrintingBranch/PriceTagPrintingWidget.h \
    widgets/TagPrintingBranch/PrinterTemplatesList.h \
    widgets/parents/AbstractCameraWidget.h \
    widgets/parents/AbstractScaningWidget.h \
    widgets/parents/BranchRootWidget.h \
    widgets/utils/ElementsStyles.h \
    widgets/utils/EventsAndFilters.h \
    widgets/utils/GlobalAppSettings.h \
    widgets/utils/VirtualBarcodeKeyboard.h \
    widgets/utils/ZebraListItemDelegate.h \
    widgets/utils/client_specific_defaults.h

SOURCES += \
    Datacore/AbsEntity.cpp \
    Datacore/BarcodeEntity.cpp \
    Datacore/DataEntities.cpp \
    Datacore/EntityQuickHash.cpp \
    Datacore/PricedBarcodeEntity.cpp \
    Datacore/PrinterTemplateEntity.cpp \
    Datacore/ProductEntity.cpp \
    Datacore/ShortBarcodeEntity.cpp \
    Datacore/UtilityEntities.cpp \
    Wrappers/FiscalPrinterWrapper.cpp \
    dataFormats/dataformatcore.cpp \
    dataFormats/dataformater.cpp \
    dataFormats/formats.cpp \
    dataproviders/BackupingEngine.cpp \
    dataproviders/DataAsyncLoader.cpp \
    dataproviders/ModesDescriptions.cpp \
    dataproviders/TableHandlers.cpp \
    dataproviders/sqldataprovider.cpp \
    externalCommunication/AsyncRequestEngine.cpp \
    externalCommunication/communicationCore.cpp \
    externalCommunication/tohttp.cpp \
    externalCommunication/tolocalmemory.cpp \
    main.cpp \
    submodules/UNAQtCommons/barcodeHandling/BarcodeObserver.cpp \
    submodules/UNAQtCommons/consts/DatetimeConstants.cpp \
    submodules/UNAQtCommons/debug/debugtrace.cpp \
    submodules/UNAQtCommons/settings/CommonAppSettings.cpp \
    submodules/UNAQtCommons/widgets/ControlsWidgets/BarcodeControl.cpp \
    submodules/UNAQtCommons/widgets/ControlsWidgets/ControlTranslator.cpp \
    submodules/UNAQtCommons/widgets/ControlsWidgets/DateTimeControl.cpp \
    submodules/UNAQtCommons/widgets/ControlsWidgets/LabelControl.cpp \
    submodules/UNAQtCommons/widgets/ControlsWidgets/QuantityControl.cpp \
    submodules/UNAQtCommons/widgets/ControlsWidgets/StringControl.cpp \
    submodules/UNAQtCommons/widgets/ControlsWidgets/abs_control.cpp \
    submodules/UNAQtCommons/widgets/Scaling/ScalingUtilities.cpp \
    submodules/UNAQtCommons/widgets/UtilityElements/AndroidFileDialog.cpp \
    submodules/UNAQtCommons/widgets/UtilityElements/BigButtonsSpinbox.cpp \
    submodules/UNAQtCommons/widgets/UtilityElements/ExtendedDialogs.cpp \
    submodules/UNAQtCommons/widgets/UtilityElements/ExtendedLabels.cpp \
    submodules/UNAQtCommons/widgets/UtilityElements/MegaIconButton.cpp \
    submodules/UNAQtCommons/widgets/UtilityElements/SwitchableIMDecorator.cpp \
    submodules/UNAQtCommons/widgets/parents/abstractNodeInterface.cpp \
    submodules/UNAQtCommons/widgets/parents/inframedWidget.cpp \
    submodules/UNAQtCommons/wrappers/DirectConnectionPrinters/AbsPrinterWrapper.cpp \
    submodules/UNAQtCommons/wrappers/DirectConnectionPrinters/AndroidBluetoothPrinterWrapper.cpp \
    submodules/UNAQtCommons/wrappers/DirectConnectionPrinters/FTRCOMPrinterWrapper.cpp \
    submodules/UNAQtCommons/wrappers/DirectConnectionPrinters/NullPrinterWrapper.cpp \
    submodules/UNAQtCommons/wrappers/DirectConnectionPrinters/PrinterWrapperFactory.cpp \
    submodules/UNAQtCommons/wrappers/DirectConnectionPrinters/tty/tty.cpp \
    submodules/UNAQtCommons/wrappers/Platform/Android/QBroadcastCatcher.cpp \
    submodules/UNAQtCommons/wrappers/Platform/Android/androidservicewrapper.cpp \
    submodules/UNAQtCommons/wrappers/Sound/AbsSoundWrapper.cpp \
    submodules/UNAQtCommons/wrappers/Sound/ModernQtSoundWrapper.cpp \
    submodules/UNAQtCommons/wrappers/Sound/PhononSoundWrapper.cpp \
    submodules/UNAQtCommons/wrappers/Sound/SoundEffectPlayer.cpp \
    submodules/UNAQtCommons/wrappers/Sound/WinCEUnpackWrapper.cpp \
    widgets/ControlsMiniwidgets/ControlManager.cpp \
    widgets/CoreWidget.cpp \
    widgets/DatabaseOperationBranch/DatabaseOperationsWidget.cpp \
    widgets/ExtendedDelegates/ExtraSelectionDelegate.cpp \
    widgets/InventoryBranch/InventoryWidget.cpp \
    widgets/InvoiceBranch/InvoiceBranchWidget.cpp \
    widgets/MultibranchWidgets/BarcodeRedactingWidget.cpp \
    widgets/MultibranchWidgets/PrintingReceiptScaningWidget.cpp \
    widgets/MultibranchWidgets/ReceiveWidget.cpp \
    widgets/MultibranchWidgets/Redactors/Abs_Redactor.cpp \
    widgets/MultibranchWidgets/Redactors/BarcodeRedactorForm.cpp \
    widgets/MultibranchWidgets/Redactors/PricedBarcodeRedactorForm.cpp \
    widgets/MultibranchWidgets/Redactors/RoleBoundRedactor.cpp \
    widgets/MultibranchWidgets/RestScaningWidget.cpp \
    widgets/MultibranchWidgets/RoleBoundScaningWidget.cpp \
    widgets/MultibranchWidgets/ScanedStorageWidget.cpp \
    widgets/MultibranchWidgets/ScaningCameraWidget.cpp \
    widgets/MultibranchWidgets/ScaningWidget.cpp \
    widgets/MultibranchWidgets/SendSettings.cpp \
    widgets/MultibranchWidgets/SendingDataPickerWidget.cpp \
    widgets/MultibranchWidgets/UpdateableScaningWidget.cpp \
    widgets/MultibranchWidgets/innerSettingsForm.cpp \
    widgets/PriceBranch/PriceBranchRootWidget.cpp \
    widgets/PriceBranch/PriceBranchScaningWidget.cpp \
    widgets/PriceBranch/PriceBranchWidget.cpp \
    widgets/PriceBranch/PriceScaningWidget.cpp \
    widgets/SalesAccountingBranch/SalesAccounting.cpp \
    widgets/SearchBranch/SearchWidget.cpp \
    widgets/SettingsForm.cpp \
    widgets/SettingsWidgets/DatabaseLoadSettings.cpp \
    widgets/SettingsWidgets/DatabaseSettings.cpp \
    widgets/SettingsWidgets/EmailSettings.cpp \
    widgets/SettingsWidgets/ExtraScaningSettings.cpp \
    widgets/SettingsWidgets/ExtraSendSettings.cpp \
    widgets/SettingsWidgets/GeneralSettings.cpp \
    widgets/SettingsWidgets/LoginSettings.cpp \
    widgets/SettingsWidgets/PrinterConnectionSettings.cpp \
    widgets/SettingsWidgets/PrinterSettings.cpp \
    widgets/SettingsWidgets/PrinterTemplatesSettings.cpp \
    widgets/SettingsWidgets/QuickSendSettings.cpp \
    widgets/SettingsWidgets/ScaningSettings.cpp \
    widgets/SettingsWidgets/SearchDatabaseSettings.cpp \
    widgets/SettingsWidgets/SerializationSettings.cpp \
    widgets/SettingsWidgets/SpecialActionsSettings.cpp \
    widgets/SettingsWidgets/TaxInvoiceSettings.cpp \
    widgets/SettingsWidgets/ViewSettings.cpp \
    widgets/SimpleBranch/SimpleBranchWidget.cpp \
    widgets/SupplyBranch/SuppliesWidget.cpp \
    widgets/TagPrintingBranch/PriceTagPrintingWidget.cpp \
    widgets/TagPrintingBranch/PrinterTemplatesList.cpp \
    widgets/parents/AbstractCameraWidget.cpp \
    widgets/parents/AbstractScaningWidget.cpp \
    widgets/parents/BranchRootWidget.cpp \
    widgets/utils/ElementsStyles.cpp \
    widgets/utils/EventsAndFilters.cpp \
    widgets/utils/GlobalAppSettings.cpp \
    widgets/utils/VirtualBarcodeKeyboard.cpp \
    widgets/utils/ZebraListItemDelegate.cpp

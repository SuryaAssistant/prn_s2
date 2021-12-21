// SPDX-License-Identifier: GPL-2.0+

#include "selectsupporteddevice.h"

#include <QDebug>
#include <QDesktopServices>
#include <QFile>
#include <QTimer>
#include <QUrl>

#include "devicelistentry.h"
#include "deviceslistmodel.h"
#include "dsomodel.h"
#include "modelregistry.h"
#include "usb/finddevices.h"
#include "usb/uploadFirmware.h"
#include "viewconstants.h"

SelectSupportedDevice::SelectSupportedDevice( QWidget *parent ) : QDialog( parent ), ui( new Ui::SelectSupportedDevice ) {
    ui->setupUi( this );
    ui->buttonBox->button( QDialogButtonBox::Ok )->setEnabled( false );
    qRegisterMetaType< UniqueUSBid >( "UniqueUSBid" );
    connect( ui->buttonBox, &QDialogButtonBox::accepted, [ this ]() {
        if ( ui->cmbDevices->currentIndex() != -1 ) {
            selectedDevice = ui->cmbDevices->currentData( Qt::UserRole ).value< UniqueUSBid >();
        }
        QCoreApplication::instance()->quit();
    } );
    connect( ui->buttonBox, &QDialogButtonBox::helpRequested, []() {
        QString usrManualPath( USR_MANUAL_PATH );
        QFile userManual( usrManualPath );
        if ( userManual.exists() )
            QDesktopServices::openUrl( QUrl( "file://" + usrManualPath ) );
        else
            QDesktopServices::openUrl(
                QUrl( "https://github.com/OpenHantek/OpenHantek6022/blob/main/docs/OpenHantek6022_User_Manual.pdf" ) );
    } );
    connect( ui->btnDemoMode, &QPushButton::clicked, [ this ]() { demoModeClicked = true; } );
}

std::unique_ptr< ScopeDevice > SelectSupportedDevice::showSelectDeviceModal( libusb_context *context, unsigned verboseLevel ) {
    std::unique_ptr< FindDevices > findDevices = std::unique_ptr< FindDevices >( new FindDevices( context, verboseLevel ) );
    std::unique_ptr< DevicesListModel > model =
        std::unique_ptr< DevicesListModel >( new DevicesListModel( findDevices.get(), verboseLevel ) );
    ui->cmbDevices->setModel( model.get() );

    QString messageDeviceReady =
        tr( "<p><br/><b>The device is ready for use.</b></p><p>Please observe the "
            "<a href='https://github.com/OpenHantek/OpenHantek6022/blob/main/docs/OpenHantek6022_User_Manual.pdf'>"
            "user manual</a> for safe operation.</p>" );

    QString messageNoDevices = tr( "<p>OpenHantek6022 is searching for compatible devices ...</p>"
                                   "<p><img align='right' height='200' src='qrc:///switch_6022BL.png'>"
                                   "Don't forget to switch your device into oscilloscope mode if it has multiple modes.</p>" );
#if defined( Q_OS_WIN )
    messageNoDevices += tr( "<p>Please make sure you have installed the windows usb driver correctly</p>" );
#elif defined( Q_OS_LINUX )
    QFile libRules( "/usr/lib/udev/rules.d/60-openhantek.rules" );
    QFile etcRules( "/etc/udev/rules.d/60-openhantek.rules" );
    if ( !libRules.exists() && !etcRules.exists() ) {
        messageNoDevices += tr( "<p>Please make sure you have copied the udev rules file to<br/>"
                                "<b>%1</b> or<br/><b>%2</b><br/>"
                                "for correct USB access permissions.</p>" )
                                .arg( etcRules.fileName(), libRules.fileName() );
    }
#endif
    messageNoDevices +=
        tr( "<p>Visit the build and run instruction "
            "<a href='https://github.com/OpenHantek/OpenHantek6022/blob/main/docs/build.md'>website</a> for help.</p>" );
    messageNoDevices += tr( "<hr/><p>Even without a device you can explore the program's function. "
                            "Just press the <b>Demo Mode</b> button below.</p>" );

    connect( ui->cmbDevices, static_cast< void ( QComboBox::* )( int ) >( &QComboBox::currentIndexChanged ),
             [ this, &messageDeviceReady ]( int index ) {
                 if ( index == -1 ) {
                     ui->buttonBox->button( QDialogButtonBox::Ok )->setEnabled( false );
                     return;
                 }
                 if ( ui->cmbDevices->currentData( Qt::UserRole + 1 ).toBool() ) { // canConnect
                     ui->buttonBox->button( QDialogButtonBox::Ok )->setEnabled( true );
                     ui->labelReadyState->setText( messageDeviceReady );
                 } else {
                     ui->buttonBox->button( QDialogButtonBox::Ok )->setEnabled( false );
                     if ( ui->cmbDevices->currentData( Qt::UserRole + 2 ).toBool() ) { // needFirmware
                         ui->labelReadyState->setText( tr( "<p>Upload in progress ...</p>"
                                                           "<p><b>If the upload takes more than 30 s, please close this window "
                                                           "<br/>and restart the program!</b></p>" ) );
                     } else { // something went wrong, inform user
                         ui->labelReadyState->setText( tr( "<p><br/><b>Connection failed!</b></p>" ) +
                                                       ui->cmbDevices->currentData( Qt::UserRole + 3 ).toString() );
                     }
                 }
             } );

    updateSupportedDevices();

    QTimer timer;
    timer.setInterval( 1000 );
    connect( &timer, &QTimer::timeout, [ this, &model, &findDevices, &messageDeviceReady, &messageNoDevices ]() {
        static int supportedDevices = -1; // max number of devices that can connect or need firmware
        static int readyDevices = -1;
        if ( findDevices->updateDeviceList() ) { // searching...
            model->updateDeviceList();
        }
        supportedDevices = qMax( supportedDevices, model->rowCount( QModelIndex() ) );
        int index = 0;
        int devices = 0;
        for ( index = 0; index < model->rowCount( QModelIndex() ); ++index ) {
            if ( ui->cmbDevices->itemData( index, Qt::UserRole + 1 ).toBool() )
                ++devices; // count devices that can connect
        }
        // printf( "%d, %d, %d devices\n", model->rowCount( QModelIndex() ), supportedDevices, devices );
        if ( 1 == devices && 1 == supportedDevices ) { // only one device ready, start it without user action
            int mIndex = 0;
            for ( mIndex = 0; mIndex < model->rowCount( QModelIndex() ); ++mIndex ) {
                if ( ui->cmbDevices->itemData( mIndex, Qt::UserRole + 1 ).toBool() ) // can connect
                    break;
            }
            ui->cmbDevices->setCurrentIndex( mIndex );
            if ( ui->buttonBox->button( QDialogButtonBox::Ok )->isEnabled() ) { // if scope is ready to run
                ui->buttonBox->button( QDialogButtonBox::Ok )->click();         // start it without user activity
            }
        } else if ( devices && model->rowCount( QModelIndex() ) ) {
            // more than 1 devices ready
            if ( devices != readyDevices ) { // find 1st ready device
                int mIndex = 0;
                for ( mIndex = 0; mIndex < model->rowCount( QModelIndex() ); ++mIndex ) {
                    if ( ui->cmbDevices->itemData( mIndex, Qt::UserRole + 1 ).toBool() ) // can connect
                        break;
                }
                readyDevices = devices;
                ui->labelReadyState->setText( messageDeviceReady );
                ui->cmbDevices->setCurrentIndex( mIndex );
            }
        } else { // no devices found (not yet)
            ui->labelReadyState->setText( messageNoDevices );
        }
    } );
    timer.start();
    QCoreApplication::sendEvent( &timer, new QTimerEvent( timer.timerId() ) ); // immediate timer event

    show();
    QCoreApplication::instance()->exec();
    timer.stop();
    close();
    if ( demoModeClicked )
        return std::unique_ptr< ScopeDevice >( new ScopeDevice() );
    return findDevices->takeDevice( selectedDevice );
}


void SelectSupportedDevice::showLibUSBFailedDialogModel( int error ) {
    ui->buttonBox->button( QDialogButtonBox::Ok )->setEnabled( false );
    ui->labelReadyState->setText( tr( "Can't initalize USB: %1" ).arg( libUsbErrorString( error ) ) );
    show();
    QCoreApplication::instance()->exec();
    close();
}


void SelectSupportedDevice::updateSupportedDevices() {
    QString devices;
    for ( const DSOModel *model : ModelRegistry::get()->models() ) {
        devices.append( model->name ).append( " " );
    }
    ui->labelSupportedDevices->setText( devices );
}

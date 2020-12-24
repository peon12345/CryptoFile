#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <devicemanagement.h>
#include <QSettings>
#include <QMessageBox>

namespace Ui {
class settings;
}

struct Parametrs{
public:
    bool copy = false;
    bool autoDelete = false;
    int algID = 0;
    QString serialUSB = "";
    QString serialHDD = "";
};



class Settings : public QDialog
{
    Q_OBJECT

public:
    QString getSerial(QString deviceType);

     Parametrs& getParams();
     bool copy();
     bool autoDel();


    static inline const QString ORGANIZATION = "CryptoCorp";
    static inline const QString DOMAIN_APPL = "Crypto";
    static inline const QString APPL = "Crypto";

    explicit Settings(QWidget *parent = nullptr);
    ~Settings();

private slots:
    void on_pushButtonSave_clicked();

    void on_pushButtonCancel_clicked();

    void on_pushButtonAddIO_clicked();

    void on_pushButtonAddIO_2_clicked();

private:
    Parametrs parametrs;

    static inline const QString SETTINGS_AUTO_DEL = "settings/auto_delete";
    static inline const QString SETTINGS_ALL_CHECK = "settings/all_check";
    static inline const QString SETTINGS_ALG_ID = "settings/alg_id";

    DeviceManagement device;
    Ui::settings *ui;
};




#endif // SETTINGS_H

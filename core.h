#ifndef CORE_H
#define CORE_H

#include <QObject>
#include <QtCore>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QtSql>

class Core : public QObject
{
    Q_OBJECT
public:
    explicit Core(QObject *parent = 0);
    ~Core();
private:
    void initPort();
    void initDb();
private:
    QSerialPort *port;
    QByteArray bytesReceived;
    QSqlDatabase db;
signals:
    
public slots:
    void read();
    
};

#endif // CORE_H

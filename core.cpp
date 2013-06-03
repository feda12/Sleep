#include "core.h"

Core::Core(QObject *parent) :
    QObject(parent)
{
    initPort();
    initDb();
}

Core::~Core()
{
    delete port;
}

void Core::initPort(){
    port = new QSerialPort("usbmodem1421", this);
    port->open(QIODevice::ReadOnly);
    if(port->error() != QSerialPort::NoError)
    {
        qDebug() << "Error: could not open Arduino";
    }
    port->setBaudRate(QSerialPort::Baud9600);
    port->setFlowControl(QSerialPort::NoFlowControl);
    port->setDataBits(QSerialPort::Data8);
    port->setStopBits(QSerialPort::OneStop);
    port->setParity(QSerialPort::NoParity);
    QObject::connect(port,SIGNAL(readyRead()), this, SLOT(read()));

}

void Core::initDb()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName("localhost");
    db.setDatabaseName("/Users/blecam/Sleep.db");
    if(!db.open())
        qDebug() << "Could not open database";
    QFileInfo fi("~/.Sleep.db");
    QSqlQuery query(db);
    if(!fi.exists())
        query.exec("CREATE TABLE sleep_data (time DATETIME, x double, y double, z double, sum double, diff double, xdiff double, ydiff double, zdiff double);");
    query.exec("SELECT * FROM sleep_data");
}

void Core::read()
{
    QByteArray bytes;
    QList<QString> li;
    QString output;
    int a = port->bytesAvailable();
    bytes.resize(a);
    port->read(bytes.data(), bytes.size());
    //qDebug() << "bytes read:" << bytes.size();

    bytesReceived.append(bytes);

    // only do input if all of it has been received.
    // without this the serial port transports line of messages
    // with only 3 or 4 bytes at a time
    if(bytes.contains('\n'))
    {
        output = QString::fromAscii(bytesReceived);
        output.remove("\n");
        li = output.split("|");
        QSqlQuery query(db);
        QString q = "INSERT INTO sleep_data VALUES (DATETIME('NOW'),";
        foreach(QString s, li){
            q += s;
            if(s != li.last())
                q += ",";
        }
        q+= ");";
        qDebug() << q;
        query.exec(q);
        qDebug() << query.lastError();
        qDebug() << "\n";

        bytesReceived.clear();
    }
}

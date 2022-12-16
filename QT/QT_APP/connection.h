#ifndef CONNECTION_H
#define CONNECTION_H

#include <QtSql>
#include <QSqlDatabase>
#include <QMessageBox>

bool createConnection(){
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    db.setDatabaseName("mydb");
    db.setUserName("alex");
    db.setPassword("25012008");
    db.setHostName("127.0.0.1");
    db.setPort(5432);
    if (!db.open()){
        QMessageBox::warning(0, "DB Error", db.lastError().text());
        return false;
    }
    else{
        QMessageBox::information(0, "Success", "Connection is established");
        return true;
    }

}

bool createTable(){
    QSqlQuery query;
    query.exec("CREATE TABLE users("
               "user_id VARCHAR(255) NOT NULL,"
               "password VARCHAR(255) NOT NULL,"
               "email VARCHAR(255) NOT NULL,"
               "telephone VARCHAR(255) NOT NULL)");
    return (1);
}

#endif // CONNECTION_H

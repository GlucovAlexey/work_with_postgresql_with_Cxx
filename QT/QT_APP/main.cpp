#include <QtGui>
#include <QtSql>
#include <QtTest/QtTest> // Для qWait().

#include "MyWidget.h"

MyDialog::MyDialog(QWidget *parent)
        : QDialog(parent) {

    setWindowFlags(Qt::Window);

    mode = new QComboBox(this);
    QStringList drivers = QSqlDatabase::drivers();
    drivers.removeAll("QMYSQL3");
    drivers.removeAll("QOCI8");
    drivers.removeAll("QODBC3");
    drivers.removeAll("QPSQL7");
    drivers.removeAll("QTDS7");
    mode->addItems(drivers);

    host = new QLineEdit(tr("localhost"), this);
    dbname = new QLineEdit(this);
    user = new QLineEdit(this);
    password = new QLineEdit(this);
    password->setEchoMode(QLineEdit::Password);

    btnStart = new QPushButton(tr("Старт"), this);

    scr = new QTextEdit(this);
    scr->setReadOnly(true);

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(new QLabel(tr("Режим:"), this),
                      0, 0, Qt::AlignRight);
    layout->addWidget(mode, 0, 1, 1, 3);

    layout->addWidget(new QLabel(tr("Хост:"), this),
                      1, 0, Qt::AlignRight);
    layout->addWidget(host, 1, 1);

    layout->addWidget(new QLabel(tr("База данных:"), this),
                      1, 2, Qt::AlignRight);
    layout->addWidget(dbname, 1, 3);

    layout->addWidget(new QLabel(tr("Пользователь:"), this),
                      2, 0, Qt::AlignRight);
    layout->addWidget(user, 2, 1);

    layout->addWidget(new QLabel(tr("Пароль:"), this),
                      2, 2, Qt::AlignRight);
    layout->addWidget(password, 2, 3);

    layout->addWidget(btnStart, 3, 1, 1, 2);
    layout->addWidget(scr, 4, 0, 1, 4);

    layout->setMargin(6);
    layout->setSpacing(5);
    layout->setColumnStretch(1, 1);
    layout->setColumnStretch(3, 1);
    layout->setRowStretch(4, 1);
    setLayout(layout);

    connect(btnStart, SIGNAL(clicked()), this, SLOT(start()));
}

bool MyDialog::start() {
    scr->append(tr("Соединяюсь с базой данных..."));
    QSqlDatabase db = QSqlDatabase::addDatabase(
        mode->currentText() );
    db.setHostName(host->text());
    db.setDatabaseName(dbname->text());
    db.setUserName(user->text());
    db.setPassword(password->text());
    if (db.open()) {
        mode->setEnabled(false);
        host->setEnabled(false);
        dbname->setEnabled(false);
        user->setEnabled(false);
        password->setEnabled(false);
        btnStart->setEnabled(false);
        scr->append(tr("Соединение установлено!"));
    }else{
        scr->append(tr("Не могу соединиться: "));
        scr->append(db.lastError().text());
        return false;
    }

    QSqlQuery sql = QSqlQuery();
    //sql.exec(tr("SET NAMES 'cp1251'"));
    QStringList dbtables = db.tables(QSql::Tables);
    if (dbtables.contains( tr("employee"),
                           Qt::CaseInsensitive)) {
        scr->append( tr(
                "Таблица \"employee\" уже существует."));
        sql.exec(tr("DROP TABLE employee"));
        if ( sql.lastError().type() == QSqlError::NoError ) {
            scr->append( tr(
                "Удалили таблицу \"employee\" "));
        }else{
            scr->append( tr(
                "Не могу удалить таблицу \"employee\":"));
            scr->append(sql.lastError().text());
            return false;
        }
    }

    sql.exec( tr(
        "create table employee ( "
        "  id integer PRIMARY KEY, "
        "  name char(30) not null, "
        "  born date null, "
        "  salary numeric(12,2), "
        "  married boolean NULL ) " ) );
    if ( sql.lastError().type() == QSqlError::NoError ) {
        scr->append( tr(
            "Создали таблицу \"employee\"."));
    }else{
        scr->append( tr(
                "Не могу создать таблицу \"employee\":"));
        scr->append(sql.lastError().text());
        return false;
    }

    if (sql.prepare( tr(
            "INSERT INTO employee "
            "  VALUES (?, ?, ?, ?, ?)") ) ) {
        int arr_id[] = {123, 345, 501};
        QString arr_name[] = {tr("Винни-Пух"),
                              tr("Ослик Иа"),
                              tr("Поросёнок")};
        QDate arr_born[] = {QDate(1971, 12, 31),
                            QDate(1965, 2, 23),
                            QDate(1982, 6, 14)};
        float arr_salary[] = {1234.56f, 2345.67f, 871};
        int arr_married[] = {1, 0, 0};

        for (unsigned int i=0; i < 3; i++) {
            sql.bindValue(0, arr_id[i]);
            sql.bindValue(1, arr_name[i]);
            sql.bindValue(2, arr_born[i]);
            sql.bindValue(3, arr_salary[i]);
            sql.bindValue(4, arr_married[i]);
            sql.exec();
            if ( sql.lastError().type() == QSqlError::NoError ) {
                scr->append( tr(
                    "Вставили новую запись."));
            }else{
                scr->append( tr(
                    "Не могу вставить новую запись:"));
                scr->append(sql.lastError().text());
                return false;
            }
        }
    }else{
        scr->append( tr(
                "Не могу подготовить запрос:"));
        scr->append(sql.lastError().text());
        return false;
    }

    sql.exec( tr("SELECT * FROM employee ") );
    if ( sql.isActive() ) {
        QSqlRecord rec = sql.record();
        scr->append( tr(
            "В таблице \"employee\" %1 столбцов: ")
            .arg(rec.count() ) );

        QString fields;
        for(int j=0; j<rec.count(); j++)
            fields += rec.fieldName(j) + ", ";

        scr->append(fields);

        scr->append( tr(
            "В таблице \"employee\" %1 записей: ")
            .arg(sql.size() ) );

        while ( sql.next() ) {
            int id = sql.value(0).toInt();
            QString name = sql.value(1).toString();
            QDate born = sql.value(2).toDate();
            double salary = sql.value(3).toDouble();
            bool married = sql.value(4).toBool();
            scr->append( tr(
                "%1\t %2\t %3\t %4\t %5")
                .arg(id)
                .arg(name)
                .arg(born.toString(tr("dd/MM/yyyy")))
                .arg(salary)
                .arg(married) );
        }
    }else{
        scr->append( tr(
                "Не могу получить данные:"));
        scr->append(sql.lastError().text());
        return false;
    }

    scr->append( tr(
        "При закрытии окна соединение с БД будет завершено."));
    return true;
}

void MyDialog::closeEvent(QCloseEvent *event) {
    QSqlDatabase db = QSqlDatabase::database();
    if (db.isOpen()) {
        db.close();
        scr->append("--------------------------");
        scr->append(tr("Соединение с базой данных закрыто!"));
        QTest::qWait(1000);    // Ждать 1 сек.
    }
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MyDialog *mainWin = new MyDialog();
    mainWin->show();
    return app.exec();
}

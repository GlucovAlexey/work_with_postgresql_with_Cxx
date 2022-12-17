#ifndef MYWIDGET_H
#define MYWIDGET_H

//#include <QtGui>
#include <QtWidgets>

class MyDialog : public QDialog{
    Q_OBJECT
public:
    MyDialog(QWidget *parent = 0);
protected:
    virtual void closeEvent(QCloseEvent *event);
private slots:
    bool start();
private:
    QComboBox *mode;   // Режим (драйвер).
    QLineEdit *host;   // Хост.
    QLineEdit *dbname; // Имя БД.
    QLineEdit *user;   // Пользователь.
    QLineEdit *password; // Пароль.
    QTextEdit *scr;      // Для вывода сообщений.
    QPushButton *btnStart; // Кнопка 'Старт'.
};

#endif // MYWIDGET_H

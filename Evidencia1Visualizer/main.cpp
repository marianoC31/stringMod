#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>
#include <QDebug>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qDebug() << "Directorio de trabajo actual:" << QDir::currentPath();

    QFile archivoEstilo("../../style.qss");
    if (archivoEstilo.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream stream(&archivoEstilo);
        a.setStyleSheet(stream.readAll());
        qDebug() << "QSS cargado correctamente";
    } else {
        qDebug() << "No se pudo abrir el QSS:" << archivoEstilo.errorString();
    }

    MainWindow w;
    w.show();
    return QApplication::exec();
}
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include "../kmp.hpp"
#include <Qtimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
private slots:
    void actualizarSeleccion();
    void iniciarKMP();
    void mostrarSiguienteEvento();
    void pausarKMP();
    void reiniciarKMP();
private:
    vector<Evento> eventos;
    int eventoActual= 0;
    Ui::MainWindow *ui;

    void mostrarEvento(const Evento &evento);

    QTimer *timer;
    QString leerArchivo(const QString &ruta);
};
#endif // MAINWINDOW_H

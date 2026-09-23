#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QTimer>
#include "../kmp.hpp"
#include "../lcs.hpp"

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
    void cambiarParte();
    void actualizarSeleccion();
    void iniciarAlgoritmo();
    void mostrarSiguienteEvento();
    void pausar();
    void reiniciar();

private:
    vector<Evento> eventosKMP;
    vector<EventoLCS> eventosLCS;
    int eventoActual = 0;
    Ui::MainWindow *ui;

    void mostrarEventoKMP(const Evento &evento);
    void mostrarEventoLCS(const EventoLCS &evento);
    void iniciarKMP();
    void iniciarLCS();

    QTimer *timer;
    QString leerArchivo(const QString &ruta);
};
#endif // MAINWINDOW_H

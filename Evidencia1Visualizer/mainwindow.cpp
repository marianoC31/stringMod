#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextCursor>
#include <QTextCharFormat>
#include <QFile>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    timer = new QTimer(this);
    connect(timer,
            &QTimer::timeout,
            this,
            &MainWindow::mostrarSiguienteEvento);
    ui->comboParte->addItem("Parte 1 - KMP");
    ui->comboParte->addItem("Parte 2");
    ui->comboParte->addItem("Parte 3");

    ui->comboTransmission->addItem("transmission1.txt");
    ui->comboTransmission->addItem("transmission2.txt");

    ui->comboMCode->addItem("mcode1.txt");
    ui->comboMCode->addItem("mcode2.txt");
    ui->comboMCode->addItem("mcode3.txt");

    connect(ui->comboTransmission,
            &QComboBox::currentIndexChanged,
            this,
            &MainWindow::actualizarSeleccion);
    connect(ui->comboMCode,
            &QComboBox::currentIndexChanged,
            this,
            &MainWindow::actualizarSeleccion);
    connect(ui->btnIniciar,
            &QPushButton::clicked,
            this,
            &MainWindow::iniciarKMP);
    connect(ui->btnPausar,
            &QPushButton::clicked,
            this,
            &MainWindow::pausarKMP);
    connect(ui->btnReiniciar,
            &QPushButton::clicked,
            this,
            &MainWindow::reiniciarKMP);
    actualizarSeleccion();
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::leerArchivo(const QString &ruta)
{
    QFile archivo(ruta);

    if (!archivo.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return "No se pudo abrir el archivo";
    }

    QTextStream entrada(&archivo);

    QString contenido;
    QString linea;

    while (!entrada.atEnd()) {
        linea = entrada.readLine();
        contenido += linea;
    }

    return contenido;
}
void MainWindow::actualizarSeleccion()
{
    QString nombreTransmission =
        ui->comboTransmission->currentText();
    QString nombreMCode =
        ui->comboMCode->currentText();
    QString transmission =
        leerArchivo(nombreTransmission);
    QString mcode =
        leerArchivo(nombreMCode);

    ui->transmissionView->setPlainText(transmission);
    ui->mcodeView->setPlainText(mcode);
    ui->lblEstado->setText(
        "Estado: Listo"
        );
}
void MainWindow::iniciarKMP()
{
    timer->stop();
    QString nombreTransmission =
        ui->comboTransmission->currentText();
    QString nombreMCode =
        ui->comboMCode->currentText();
    QString transmission =
        leerArchivo(nombreTransmission);
    QString mcode =
        leerArchivo(nombreMCode);
    eventos.clear();
    eventoActual = 0;
    int posicion = kmpBuscarPrimera(
        transmission.toStdString(),
        mcode.toStdString(),
        &eventos,
        nombreTransmission.toStdString(),
        nombreMCode.toStdString()
        );

    if (posicion == -1) {
        ui->lblEstado->setText(
            "Estado: No encontrado"
            );
    } else {
        ui->lblEstado->setText(
            "Estado: Encontrado en posición " +
            QString::number(posicion + 1)
            );
    }
    if(!eventos.empty()){
        eventoActual = 0;
        timer->start(300);
    }
}
void MainWindow::mostrarSiguienteEvento()
{
    if(eventoActual >= eventos.size()){
        timer->stop();
        ui->lblEstado->setText("Estado: Animación Terminadinski");
        return;
    }
    const Evento &evento = eventos[eventoActual];

    QString estado =
        "Evento " + QString::number(eventoActual+1)+
                     "/" + QString::number(eventos.size())+
                     "\nTipo: " + QString::fromStdString(evento.tipo)+
                     "\nPosición texto: " + QString::number(evento.posTexto) +
                     "\nPosición patrón: " + QString::number(evento.posPatron);
    ui->lblEstado->setText(estado);
    mostrarEvento(evento);
    eventoActual++;
}
void MainWindow::mostrarEvento(const Evento &evento)
{
    QFont fuente("Courier New");
    fuente.setStyleHint(QFont::Monospace);

    ui->transmissionView->setFont(fuente);
    ui->mcodeView->setFont(fuente);

    QString mcode =
        leerArchivo(QString::fromStdString(evento.archivoPatron));

    int inicioPatron =
        evento.posTexto - evento.posPatron;

    if (inicioPatron < 0) {
        inicioPatron = 0;
    }

    QString mcodeDesplazado =
        QString(inicioPatron, ' ') + mcode;

    ui->mcodeView->setPlainText(mcodeDesplazado);

    ui->transmissionView->setExtraSelections({});
    ui->mcodeView->setExtraSelections({});

    QTextCharFormat formato;

    if (evento.tipo == "comparando") {
        formato.setBackground(Qt::yellow);
        formato.setForeground(Qt::black);
    }
    else if (evento.tipo == "match_parcial") {
        formato.setBackground(Qt::green);
        formato.setForeground(Qt::black);
    }
    else if (evento.tipo == "mismatch") {
        formato.setBackground(Qt::red);
        formato.setForeground(Qt::white);
    }
    else if (evento.tipo == "match_total") {
        formato.setBackground(Qt::blue);
        formato.setForeground(Qt::white);
    }
    else if (evento.tipo == "no encontradi") {
        formato.setBackground(Qt::red);
        formato.setForeground(Qt::white);
    }

    QTextEdit::ExtraSelection seleccionTexto;
    QTextEdit::ExtraSelection seleccionPatron;

    if (evento.posTexto >= 0) {

        QTextCursor cursorTexto(
            ui->transmissionView->document()
            );

        cursorTexto.setPosition(evento.posTexto);

        cursorTexto.movePosition(
            QTextCursor::NextCharacter,
            QTextCursor::KeepAnchor
            );

        seleccionTexto.cursor = cursorTexto;
        seleccionTexto.format = formato;
    }

    if (evento.posPatron >= 0) {

        int posicionVisual =
            inicioPatron + evento.posPatron;

        QTextCursor cursorPatron(
            ui->mcodeView->document()
            );

        cursorPatron.setPosition(posicionVisual);

        cursorPatron.movePosition(
            QTextCursor::NextCharacter,
            QTextCursor::KeepAnchor
            );

        seleccionPatron.cursor = cursorPatron;
        seleccionPatron.format = formato;
    }

    ui->transmissionView->setExtraSelections(
        {seleccionTexto}
        );

    ui->mcodeView->setExtraSelections(
        {seleccionPatron}
        );
}
void MainWindow::pausarKMP()
{
    if(timer->isActive()){
        timer->stop();
        ui->lblEstado->setText("Estado: Pausado");
    } else if(!eventos.empty()&&eventoActual<eventos.size()){
        timer->start(300);
        ui->lblEstado->setText("Estado: Reanudando...");
    }
}

void MainWindow::reiniciarKMP()
{
    timer->stop();
    eventoActual = 0;

    if(!eventos.empty()){
        mostrarEvento(eventos[0]);
        ui->lblEstado->setText("Estado: Reiniciando");
    }else{
        ui->lblEstado->setText("Estado: Listo");
    }
}

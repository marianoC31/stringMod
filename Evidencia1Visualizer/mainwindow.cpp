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
    ui->comboParte->addItem("Parte 2 - Manacher");
    ui->comboParte->addItem("Parte 3 - LCS");

    ui->comboTransmission->addItem("transmission1.txt");
    ui->comboTransmission->addItem("transmission2.txt");

    ui->comboMCode->addItem("mcode1.txt");
    ui->comboMCode->addItem("mcode2.txt");
    ui->comboMCode->addItem("mcode3.txt");

    connect(ui->comboParte,
            &QComboBox::currentIndexChanged,
            this,
            &MainWindow::cambiarParte);
    connect(ui->comboTransmission,
            &QComboBox::currentIndexChanged,
            this,
            &MainWindow::actualizarSeleccion);
    connect(ui->comboMCode,
            &QComboBox::currentIndexChanged,
            this,
            &MainWindow::actualizarSeleccion);
    connect(ui->sliderVelocidad,
            &QSlider::valueChanged,
            this,
            [this](int valor){
                timer->setInterval(valor);
            });
    connect(ui->btnIniciar,
            &QPushButton::clicked,
            this,
            &MainWindow::iniciarAlgoritmo);
    connect(ui->btnPausar,
            &QPushButton::clicked,
            this,
            &MainWindow::pausar);
    connect(ui->btnReiniciar,
            &QPushButton::clicked,
            this,
            &MainWindow::reiniciar);

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

void MainWindow::cambiarParte()
{
    timer->stop();
    eventosKMP.clear();
    eventosLCS.clear();
    eventosManacher.clear();
    eventoActual = 0;
    ui->transmissionView->setExtraSelections({});
    ui->mcodeView->setExtraSelections({});

    QString parte = ui->comboParte->currentText();

    if (parte.contains("Parte 3 - LCS")) {
        ui->label->setText("Transmission 1:");
        ui->label_2->setText("Transmission 2:");
        ui->comboTransmission->setEnabled(false);
        ui->comboMCode->setEnabled(false);

        QString t1 = leerArchivo("transmission1.txt");
        QString t2 = leerArchivo("transmission2.txt");

        ui->transmissionView->setPlainText(t1);
        ui->mcodeView->setPlainText(t2);
        ui->lblEstado->setText("Estado: Listo para Parte 3 (LCS)\nPresione 'Iniciar' para buscar la subcadena común más larga.");
    } else if (parte.contains("Parte 1 - KMP")) {
        ui->label->setText("Transmission:");
        ui->label_2->setText("MCode:");
        ui->comboTransmission->setEnabled(true);
        ui->comboMCode->setEnabled(true);
        actualizarSeleccion();
    } else if(parte.contains("Parte 2 - Manacher")){
        ui->label->setText("Transmission:");
        ui->label_2->setText("Palíndromo más largo:");
        ui->comboTransmission->setEnabled(true);
        ui->comboMCode->setEnabled(false);
        actualizarSeleccion();
    }
}

void MainWindow::actualizarSeleccion()
{
    QString parte = ui->comboParte->currentText();
    if (parte.contains("Parte 3 - LCS")) return;
    if(parte.contains("Parte 2 - Manacher")){
        timer->stop();
        ui->transmissionView->setExtraSelections({});
        ui->transmissionView->setPlainText(leerArchivo(ui->comboTransmission->currentText()));
        ui->mcodeView->clear();
        ui->lblEstado->setText("Estado: Listo para Parte 2 (Manacher)\nPresione 'Iniciar' para buscar el palíndromo más largo.");
        return;
    }
    QString nombreTransmission = ui->comboTransmission->currentText();
    QString nombreMCode = ui->comboMCode->currentText();
    QString transmission = leerArchivo(nombreTransmission);
    QString mcode = leerArchivo(nombreMCode);

    ui->transmissionView->setPlainText(transmission);
    ui->mcodeView->setPlainText(mcode);
    ui->lblEstado->setText("Estado: Listo");
}

void MainWindow::iniciarAlgoritmo()
{
    QString parte = ui->comboParte->currentText();
    if (parte.contains("Parte 1 - KMP")) {
        iniciarKMP();
    } else if (parte.contains("Parte 3 - LCS")) {
        iniciarLCS();
    } else if(parte.contains("Parte 2 - Manacher")){
        iniciarManacher();
    } else {
        ui->lblEstado->setText("Estado: Seleccione Parte 1 o Parte 3");
    }
}

void MainWindow::iniciarKMP()
{
    timer->stop();
    QString nombreTransmission = ui->comboTransmission->currentText();
    QString nombreMCode = ui->comboMCode->currentText();
    QString transmission = leerArchivo(nombreTransmission);
    QString mcode = leerArchivo(nombreMCode);

    eventosKMP.clear();
    eventoActual = 0;

    int posicion = kmpBuscarPrimera(
        transmission.toStdString(),
        mcode.toStdString(),
        &eventosKMP,
        nombreTransmission.toStdString(),
        nombreMCode.toStdString()
    );

    if (posicion == -1) {
        ui->lblEstado->setText("Estado: No encontrado");
    } else {
        ui->lblEstado->setText("Estado: Encontrado en posición " + QString::number(posicion + 1));
    }

    if (!eventosKMP.empty()) {
        eventoActual = 0;
        timer->start(ui->sliderVelocidad->value());
    }
}

void MainWindow::iniciarLCS()
{
    timer->stop();
    QString t1 = leerArchivo("transmission1.txt");
    QString t2 = leerArchivo("transmission2.txt");

    eventosLCS.clear();
    eventoActual = 0;

    ResultadoLCS res = longestCommonSubstring(
        t1.toStdString(),
        t2.toStdString(),
        &eventosLCS,
        "transmission1.txt",
        "transmission2.txt"
    );

    if (res.longitud == 0) {
        ui->lblEstado->setText("Estado: No se encontró subcadena común");
    } else {
        QString info = QString("Estado: Subcadena común más larga: \"%1\" (Longitud: %2)\n"
                               "Transmission 1: [%3, %4]\n"
                               "Transmission 2: [%5, %6]")
                           .arg(QString::fromStdString(res.subcadena))
                           .arg(res.longitud)
                           .arg(res.inicioTexto1)
                           .arg(res.finTexto1)
                           .arg(res.inicioTexto2)
                           .arg(res.finTexto2);
        ui->lblEstado->setText(info);
    }

    if (!eventosLCS.empty()) {
        eventoActual = 0;
        timer->start(ui->sliderVelocidad->value());
    }
}

void MainWindow::mostrarSiguienteEvento()
{
    QString parte = ui->comboParte->currentText();
    if(parte.contains("Parte 2 - Manacher")){
        if(eventoActual>=(int)eventosManacher.size()){
            timer->stop();
            ui->lblEstado->setText(resumenManacher+"\n✓ Animación completada");
            return;
        }
        const Evento &evento = eventosManacher[eventoActual];
        ui->lblEstado->setText(QString("Evento %1/%2\nTipo: %3\nCentro: %4\nRadio: %5")
                                   .arg(eventoActual + 1)
                                   .arg(eventosManacher.size())
                                   .arg(QString::fromStdString(evento.tipo))
                                   .arg(evento.posTexto)
                                   .arg(evento.posPatron));
        mostrarEventoManacher(evento);
        eventoActual++;
        return;
    }
    if (parte.contains("Parte 3 - LCS")) {
        if (eventoActual >= eventosLCS.size()) {
            timer->stop();
            ui->lblEstado->setText(ui->lblEstado->text() + "\n✓ Animación completada");
            return;
        }

        const EventoLCS &evento = eventosLCS[eventoActual];
        QString estado = QString("Evento %1/%2\nTipo: %3\nPosición Texto 1: %4\nPosición Texto 2: %5\nLongitud: %6")
                             .arg(eventoActual + 1)
                             .arg(eventosLCS.size())
                             .arg(QString::fromStdString(evento.tipo))
                             .arg(evento.posTexto)
                             .arg(evento.posPatron)
                             .arg(evento.longitud);

        ui->lblEstado->setText(estado);
        mostrarEventoLCS(evento);
        eventoActual++;
    } else {
        if (eventoActual >= eventosKMP.size()) {
            timer->stop();
            ui->lblEstado->setText("Estado: Animación Terminada");
            return;
        }

        const Evento &evento = eventosKMP[eventoActual];
        QString estado = "Evento " + QString::number(eventoActual + 1) +
                         "/" + QString::number(eventosKMP.size()) +
                         "\nTipo: " + QString::fromStdString(evento.tipo) +
                         "\nPosición texto: " + QString::number(evento.posTexto) +
                         "\nPosición patrón: " + QString::number(evento.posPatron);

        ui->lblEstado->setText(estado);
        mostrarEventoKMP(evento);
        eventoActual++;
    }
}

void MainWindow::mostrarEventoKMP(const Evento &evento)
{
    QFont fuente("Courier New");
    fuente.setStyleHint(QFont::Monospace);

    ui->transmissionView->setFont(fuente);
    ui->mcodeView->setFont(fuente);

    QString mcode = leerArchivo(QString::fromStdString(evento.archivoPatron));

    int inicioPatron = evento.posTexto - evento.posPatron;
    if (inicioPatron < 0) {
        inicioPatron = 0;
    }

    QString mcodeDesplazado = QString(inicioPatron, ' ') + mcode;
    ui->mcodeView->setPlainText(mcodeDesplazado);

    ui->transmissionView->setExtraSelections({});
    ui->mcodeView->setExtraSelections({});

    QTextCharFormat formato;
    if (evento.tipo == "comparando") {
        formato.setBackground(Qt::yellow);
        formato.setForeground(Qt::black);
    } else if (evento.tipo == "match_parcial") {
        formato.setBackground(Qt::green);
        formato.setForeground(Qt::black);
    } else if (evento.tipo == "mismatch") {
        formato.setBackground(Qt::red);
        formato.setForeground(Qt::white);
    } else if (evento.tipo == "match_total") {
        formato.setBackground(Qt::blue);
        formato.setForeground(Qt::white);
    } else if (evento.tipo == "no encontrado" || evento.tipo == "no encontradi") {
        formato.setBackground(Qt::red);
        formato.setForeground(Qt::white);
    }

    QTextEdit::ExtraSelection seleccionTexto;
    QTextEdit::ExtraSelection seleccionPatron;

    if (evento.posTexto >= 0) {
        QTextCursor cursorTexto(ui->transmissionView->document());
        cursorTexto.setPosition(evento.posTexto);
        cursorTexto.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
        seleccionTexto.cursor = cursorTexto;
        seleccionTexto.format = formato;
    }

    if (evento.posPatron >= 0) {
        int posicionVisual = inicioPatron + evento.posPatron;
        QTextCursor cursorPatron(ui->mcodeView->document());
        cursorPatron.setPosition(posicionVisual);
        cursorPatron.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
        seleccionPatron.cursor = cursorPatron;
        seleccionPatron.format = formato;
    }

    ui->transmissionView->setExtraSelections({seleccionTexto});
    ui->mcodeView->setExtraSelections({seleccionPatron});
}

void MainWindow::mostrarEventoLCS(const EventoLCS &evento)
{
    QFont fuente("Courier New");
    fuente.setStyleHint(QFont::Monospace);

    ui->transmissionView->setFont(fuente);
    ui->mcodeView->setFont(fuente);

    QString t1 = leerArchivo(QString::fromStdString(evento.archivoTexto));
    QString t2 = leerArchivo(QString::fromStdString(evento.archivoPatron));

    ui->transmissionView->setPlainText(t1);
    ui->mcodeView->setPlainText(t2);

    ui->transmissionView->setExtraSelections({});
    ui->mcodeView->setExtraSelections({});

    QTextCharFormat formato;
    if (evento.tipo == "nuevo_maximo") {
        formato.setBackground(QColor(255, 165, 0)); // Naranja
        formato.setForeground(Qt::black);
    } else if (evento.tipo == "match_parcial") {
        formato.setBackground(Qt::green);
        formato.setForeground(Qt::black);
    } else if (evento.tipo == "match_total") {
        formato.setBackground(Qt::blue);
        formato.setForeground(Qt::white);
    } else {
        formato.setBackground(Qt::yellow);
        formato.setForeground(Qt::black);
    }

    QList<QTextEdit::ExtraSelection> selTextoList;
    QList<QTextEdit::ExtraSelection> selPatronList;

    if (evento.posTexto >= 0 && evento.longitud > 0) {
        QTextEdit::ExtraSelection sel;
        QTextCursor cursor(ui->transmissionView->document());
        cursor.setPosition(evento.posTexto);
        cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, evento.longitud);
        sel.cursor = cursor;
        sel.format = formato;
        selTextoList.append(sel);
    }

    if (evento.posPatron >= 0 && evento.longitud > 0) {
        QTextEdit::ExtraSelection sel;
        QTextCursor cursor(ui->mcodeView->document());
        cursor.setPosition(evento.posPatron);
        cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, evento.longitud);
        sel.cursor = cursor;
        sel.format = formato;
        selPatronList.append(sel);
    }

    ui->transmissionView->setExtraSelections(selTextoList);
    ui->mcodeView->setExtraSelections(selPatronList);
}

void MainWindow::iniciarManacher()
{
    timer->stop();
    QString nombre = ui->comboTransmission->currentText();
    QString transmission = leerArchivo(nombre);

    eventosManacher.clear();
    eventoActual = 0;

    ui->transmissionView->setPlainText(transmission);
    ui->mcodeView->clear();

    ResultadoManacher res = manacher(transmission.toStdString(),
                                     &eventosManacher,
                                     nombre.toStdString());
    if(res.longitud==0){
        resumenManacher = "Estado: No se encontró palíndromo";
    }else{
        resumenManacher = QString("Estado: Palíndromo más largo: \%1\"(longitd: %2)\nPosición: [%3, %4]")
                              .arg(QString::fromStdString(res.subcadena))
                              .arg(res.longitud)
                              .arg(res.inicio+1)
                              .arg(res.fin+1);
    }
    ui->lblEstado->setText(resumenManacher);

    if(!eventosManacher.empty()){
        timer->start(ui->sliderVelocidad->value());

    }
}
void MainWindow::mostrarEventoManacher(const Evento &evento)
{
    QFont fuente("Courier New");
    fuente.setStyleHint(QFont::Monospace);
    ui->transmissionView->setFont(fuente);
    ui->mcodeView->setFont(fuente);

    int inicio = (evento.posTexto - evento.posPatron)/2;
    int longitud = evento.posPatron;

    QString texto = ui->transmissionView->toPlainText();
    ui->mcodeView->setPlainText(QString(inicio,' ')+texto.mid(inicio,longitud));

    QTextCharFormat formato;
    if(evento.tipo=="nuevoCentro"){
        formato.setBackground(Qt::green);  formato.setForeground(Qt::black);
    } else if (evento.tipo == "palindromo_max") {
        formato.setBackground(Qt::blue);   formato.setForeground(Qt::white);
    } else { // expandiendo
        formato.setBackground(Qt::yellow); formato.setForeground(Qt::black);
    }

    QList<QTextEdit::ExtraSelection> selTexto, selPal;
    if(longitud>0){
        auto crear = [&](QTextEdit *view){
            QTextEdit::ExtraSelection sel;
            QTextCursor c(view->document());
            c.setPosition((inicio));
            c.movePosition(QTextCursor::NextCharacter,QTextCursor::KeepAnchor, longitud);
            sel.cursor = c;
            sel.format = formato;
            return sel;
        };
        selTexto.append(crear(ui->transmissionView));
        selPal.append(crear(ui->mcodeView));

        }
    ui->transmissionView->setExtraSelections(selTexto);
    ui->mcodeView->setExtraSelections(selPal);

}
int MainWindow::totalEventos() const
{
    QString parte = ui->comboParte->currentText();
    if(parte.contains("Parte 2 - Manacher")) return eventosManacher.size();
    if(parte.contains("Parte 3 - LCS")) return eventosLCS.size();
    return eventosKMP.size();
}
void MainWindow::pausar()
{
    int total = totalEventos();
    if (timer->isActive()) {
        timer->stop();
        ui->lblEstado->setText("Estado: Pausado");
    } else if (total > 0 && eventoActual < total) {
        timer->start(ui->sliderVelocidad->value());
        ui->lblEstado->setText("Estado: Reanudando...");
    }
}

void MainWindow::reiniciar()
{
    timer->stop();
    eventoActual = 0;
    QString parte = ui->comboParte->currentText();
    int total = totalEventos();

    if (total > 0) {
        if (parte.contains("Parte 2 - Manacher")) mostrarEventoManacher(eventosManacher[0]);
        else if(parte.contains("Parte 3 -> LCS")) mostrarEventoLCS(eventosLCS[0]);
        else mostrarEventoKMP(eventosKMP[0]);
        ui->lblEstado->setText("Estado: Reiniciando animación");
    } else {
        ui->lblEstado->setText("Estado: Listo");
    }
}

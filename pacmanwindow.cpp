#include <iostream>
#include "pacmanwindow.hpp"

using namespace std;

PacmanWindow::PacmanWindow(QWidget *pParent, Qt::WindowFlags flags):QFrame(pParent, flags)
{
    // Taille des cases en pixels
    int largeurCase, hauteurCase;

    this->setStyleSheet("background-color: rgb(80, 80, 80);");


    if (pixmapPacman[0].load("./data/pacman1.bmp")==false)
    {
        cout<<"Impossible d'ouvrir pacman1.png"<<endl;
        exit(-1);
    }
        if (pixmapPacman[1].load("./data/pacman2.bmp")==false)
    {
        cout<<"Impossible d'ouvrir pacman2.png"<<endl;
        exit(-1);
    }
        if (pixmapPacman[2].load("./data/pacman2.bmp")==false)
    {
        cout<<"Impossible d'ouvrir pacman1.png"<<endl;
        exit(-1);
    }
    imagePacman = 0;

    if (pixmapFantome.load("./data/fantome.bmp")==false)
    {
        cout<<"Impossible d'ouvrir fantome.bmp"<<endl;
        exit(-1);
    }

    if (pixmapMur.load("./data/mur.bmp")==false)
    {
        cout<<"Impossible d'ouvrir mur.bmp"<<endl;
        exit(-1);
    }

    jeu.init();

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &PacmanWindow::handleTimer);
    timer->start(150);


    largeurCase = pixmapMur.width();
    hauteurCase = pixmapMur.height();

        // gestion boutton

    PacmanButton *btn1 = new PacmanButton("Ajout de fantome",this);
    btn1->setGeometry(10,2.5,150,hauteurCase-5);
    btn1->setStyleSheet("background-color: white");
        
    PacmanButton *btn2 = new PacmanButton("Retrait de fantome",this);
    btn2->setGeometry(170,2.5,150,hauteurCase-5);
    btn2->setStyleSheet("background-color: white");

    connect(btn1, &QPushButton::clicked, this, &PacmanWindow::ajoutFantome);

    connect(btn2, &QPushButton::clicked, this, &PacmanWindow::retraitFantome);


    resize(jeu.getNbCasesX()*largeurCase, jeu.getNbCasesY()*(hauteurCase+2));
}

void PacmanWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    int x, y;

    // Taille des cases en pixels
    int largeurCase, hauteurCase;

    largeurCase = pixmapMur.width();
    hauteurCase = pixmapMur.height();

    // Dessine les cases
    for (y=0;y<jeu.getNbCasesY();y++)
        for (x=0;x<jeu.getNbCasesX();x++)
			if (jeu.getCase(x,y)==MUR)
                painter.drawPixmap(x*largeurCase, (y+1)*hauteurCase, pixmapMur);

    // Dessine les fantomes
    const list<Fantome> &fantomes = jeu.getFantomes();
    list<Fantome>::const_iterator itFantome;
    for (itFantome=fantomes.begin(); itFantome!=fantomes.end(); itFantome++)
        painter.drawPixmap(itFantome->getPosX()*largeurCase, (itFantome->getPosY()+1)*hauteurCase, pixmapFantome);

    // Dessine Pacman en fonction de la direction:
    int rot;
    if (jeu.getDirPacman()==DROITE)
        rot=0;
    if(jeu.getDirPacman() == BAS)
        rot=90;
    else if(jeu.getDirPacman() == GAUCHE)
        rot=180;
    else if(jeu.getDirPacman() == HAUT)
        rot=270;
    QMatrix rm;
    rm.rotate(rot);
    // rotation de l'image du Pacman
    QPixmap p = pixmapPacman[imagePacman].transformed(rm);

	painter.drawPixmap(jeu.getPacmanX()*largeurCase, (jeu.getPacmanY()+1)*hauteurCase, p);
    // prochaine image du pacman
	imagePacman++;
	imagePacman%=3;



}


void PacmanWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key()==Qt::Key_Left)
        jeu.setDirPacman(GAUCHE);
    else if (event->key()==Qt::Key_Right)
        jeu.setDirPacman(DROITE);
    else if (event->key()==Qt::Key_Up)
        jeu.setDirPacman(HAUT);
    else if (event->key()==Qt::Key_Down)
        jeu.setDirPacman(BAS);
    update();
}

void PacmanWindow::handleTimer()
{
    jeu.evolue();
    jeu.colisionPacman();
    update();
}

void PacmanWindow::ajoutFantome() {

    jeu.AjoutFantome();
}

void PacmanWindow::retraitFantome() {

    jeu.RetraitFantome();
}

PacmanButton::PacmanButton(const QString &text, QWidget *parent) : QPushButton(text,parent)
{
}


void PacmanButton::keyPressEvent(QKeyEvent *e)
{
if (parent()!=nullptr)
QCoreApplication::sendEvent(parent(), e);
}

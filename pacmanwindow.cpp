#include <iostream>
#include "pacmanwindow.hpp"

using namespace std;

PacmanWindow::PacmanWindow(QWidget *pParent, Qt::WindowFlags flags):QFrame(pParent, flags)
{
    // Taille des cases en pixels
    int largeurCase, hauteurCase;

    this->setStyleSheet("background-color: rgb(80, 80, 80);");

    initImages();
    

    etatJeu = 1;

    jeu.init();

    timerJeu = new QTimer(this);
    connect(timerJeu, &QTimer::timeout, this, &PacmanWindow::handleTimerJeu);
    timerJeu->start(20);

    timerAffichage = new QTimer(this);
    connect(timerAffichage, &QTimer::timeout, this, &PacmanWindow::handleTimerAffichage);
    timerAffichage->start(40);


    largeurCase = pixmapMur.width();
    hauteurCase = pixmapMur.height();

        // gestion boutton

    PacmanButton *btn1 = new PacmanButton("Ajout de fantome",this);
    btn1->setGeometry(10,2.5,150,hauteurCase-5);
    btn1->setStyleSheet("background-color: white");

    PacmanButton *btn2 = new PacmanButton("Retrait de fantome",this);
    btn2->setGeometry(170,2.5,150,hauteurCase-5);
    btn2->setStyleSheet("background-color: white");

    PacmanButton *btn3 = new PacmanButton("Fin",this);
    btn3->setGeometry(475,2.5,150,hauteurCase-5);
    btn3->setStyleSheet("background-color: white");

    connect(btn1, &QPushButton::clicked, this, &PacmanWindow::ajoutFantome);

    connect(btn2, &QPushButton::clicked, this, &PacmanWindow::retraitFantome);

    connect(btn3, &QPushButton::clicked, this, &PacmanWindow::finDeJeu);


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

    if (etatJeu==1)
    {


        // Dessine les cases
        for (y=0;y<jeu.getNbCasesY();y++)
            for (x=0;x<jeu.getNbCasesX();x++)
                if (jeu.getCase(x,y)==MUR)
                    painter.drawPixmap(x*largeurCase, (y+1)*hauteurCase, pixmapMur);
                else if (jeu.getCase(x,y)==VIDE)
                    painter.drawPixmap(x*largeurCase, (y+1)*hauteurCase, pixmapVide);

        // Dessine les fantomes
        const list<Fantome> &fantomes = jeu.getFantomes();
        list<Fantome>::const_iterator itFantome;
        for (itFantome=fantomes.begin(); itFantome!=fantomes.end(); itFantome++)
        {
            QPixmap pixmapFanome;
            switch (itFantome->getComportement())
            {
                {
                case TRAQUEUR:
                    pixmapFanome= pixmapFantomeRouge;
                    break;
                case FUYARD:
                    pixmapFanome= pixmapFantomeFuyard;
                    break;
                case OBSERVATEUR:
                    pixmapFanome= pixmapFantomeOrange;
                    break;
                default:
                    pixmapFanome= pixmapFantomeCyan;
                    break;
                }
            }
            painter.drawPixmap(itFantome->getPosX()*largeurCase, (itFantome->getPosY()+1)*hauteurCase, pixmapFanome);
        }
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
        imagePacman%=4;
        }

    else if (etatJeu == 0)//affichage fin du jeu
    {
        QPixmap p = pixmapPacman[1];
        for (int i=0;i<jeu.getNbCasesX();i++)
        {
            painter.drawPixmap(i*largeurCase, (jeu.getNbCasesY()/4)*hauteurCase, p);
            painter.drawPixmap(i*largeurCase, (jeu.getNbCasesY()/4)*hauteurCase*4, p);
        }
        /*PacmanButton *button = findChild<PacmanButton*>("Fin");
        button->deleteLater();*/

        //painter.fillRect(rect(), Qt::white);

        painter.setPen(Qt::black);
        painter.setFont(QFont("Arial", 50));
        painter.drawText(rect(), Qt::AlignCenter, "Fin du jeu");

        QTimer::singleShot(3000, this, &PacmanWindow::close);

    }




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

void PacmanWindow::handleTimerJeu()
{
    jeu.evolue();

    if (jeu.getFin()==true)
        finDeJeu();

}

void PacmanWindow::handleTimerAffichage()
{
    update();
}

void PacmanWindow::finDeJeu(){


    etatJeu = 0;
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

void PacmanWindow::initImages()
{
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
        if (pixmapPacman[2].load("./data/pacman3.bmp")==false)
    {
        cout<<"Impossible d'ouvrir pacman1.png"<<endl;
        exit(-1);
    }
        if (pixmapPacman[3].load("./data/pacman2.bmp")==false)
    {
        cout<<"Impossible d'ouvrir pacman1.png"<<endl;
        exit(-1);
    }
    imagePacman = 0;

    //Images des fantomes

    if (pixmapFantomeCyan.load("./data/fantomes/cyan.png")==false)
    {
        cout<<"Impossible d'ouvrir cyan.png"<<endl;
        exit(-1);
    }
    if (pixmapFantomeRouge.load("./data/fantomes/rouge.png")==false)
    {
        cout<<"Impossible d'ouvrir rouge.png"<<endl;
        exit(-1);
    }
    if (pixmapFantomeOrange.load("./data/fantomes/orange.png")==false)
    {
        cout<<"Impossible d'ouvrir orange.png"<<endl;
        exit(-1);
    }
    if(pixmapFantomeFuyard.load("./data/fantomes/fuyard.png")==false)
    {
        cout<<"Impossible d'ouvrir fuyard.png"<<endl;
        exit(-1);
    }


    if (pixmapMur.load("./data/greystone.png")==false)
    {
        cout<<"Impossible d'ouvrir mur.bmp"<<endl;
        exit(-1);
    }
    if (pixmapVide.load("./data/background.bmp")==false)
    {
        cout<<"Impossible d'ouvrir mur.bmp"<<endl;
        exit(-1);
    }
}




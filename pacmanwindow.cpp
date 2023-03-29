#include <iostream>
#include "pacmanwindow.hpp"

using namespace std;

PacmanWindow::PacmanWindow(QWidget *pParent, Qt::WindowFlags flags):QFrame(pParent, flags)
{
    // Taille des cases en pixels
    int largeurCase, hauteurCase;

    this->setStyleSheet("background-color: rgb(80, 80, 80);");

    initImages();
    /////////////////// Charger une police depuis le fichier/////////////////////
    // Charger la police depuis le fichier
    QString fontFile = QApplication::applicationDirPath() + "/PAC-FONT.TTF";
    int fontId = QFontDatabase::addApplicationFont(fontFile);
    // Récupérer le nom de la police
    PacmanFont = QFontDatabase::applicationFontFamilies(fontId).at(0);
    
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

    PacmanButton *btn3 = new PacmanButton("Pause",this);
    btn3->setGeometry(330,2.5,150,hauteurCase-5);
    btn3->setStyleSheet("background-color: white");

    PacmanButton *btn4 = new PacmanButton("Fin",this);
    btn4->setGeometry(500,2.5,150,hauteurCase-5);
    btn4->setStyleSheet("background-color: white");

    connect(btn1, &QPushButton::clicked, this, &PacmanWindow::ajoutFantome);

    connect(btn2, &QPushButton::clicked, this, &PacmanWindow::retraitFantome);

    connect(btn3, &QPushButton::clicked, this, &PacmanWindow::Pause);

    connect(btn4, &QPushButton::clicked, this, &PacmanWindow::finDeJeu);


    resize(jeu.getNbCasesX()*largeurCase, jeu.getNbCasesY()*(hauteurCase+2));
}
///////////////////////////////////////////AFFICHAGE///////////////////////////////////////////
 
void PacmanWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    int x, y;

        // Taille des cases en pixels
        int largeurCase, hauteurCase;

        largeurCase = pixmapMur.width();
        hauteurCase = pixmapMur.height();

    if (etatJeu==PLAY)
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
    else if (etatJeu == FIN)//affichage fin du jeu
    {
        QPixmap p = pixmapPacman[1];
        for (int i=0;i<jeu.getNbCasesX();i++)
        {
            painter.drawPixmap(i*largeurCase, (jeu.getNbCasesY()/4)*hauteurCase, p);
            painter.drawPixmap(i*largeurCase, (jeu.getNbCasesY()/4)*hauteurCase*4, p);
        }
        painter.setPen(Qt::black);
        painter.setFont(QFont(PacmanFont, 50));
        painter.drawText(rect(), Qt::AlignCenter, "fin du jeu");

        QTimer::singleShot(3000, this, &PacmanWindow::close);

    }
    else if(etatJeu==PAUSE)
    {   

        // Calculer les coordonnées du centre de l'écran
        int centerX = jeu.getNbCasesX()/2*largeurCase;
        int centerY = jeu.getNbCasesY()/2*hauteurCase;

        // Définir les dimensions du rectangle
        int lrect = 500;
        int hrect = 100;

        // Créer un QRect centré sur la fenêtre
        QRect rectangle(centerX - lrect / 2, centerY - hrect / 2, lrect, hrect);

        painter.setFont(QFont(PacmanFont, 50));//définit la police et la taille du texte
        painter.fillRect(rectangle, Qt::white);
        painter.drawText(rectangle, Qt::AlignCenter, tr("pause"));

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


    etatJeu = FIN;
}

void PacmanWindow::Pause(){

    if (timerJeu->isActive())
        {
            timerJeu->stop();
            etatJeu=PAUSE;
        }
    else
        {
            timerJeu->start(20);
            etatJeu=PLAY;
        }
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




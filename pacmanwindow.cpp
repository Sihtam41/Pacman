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

    // Déclaration des boutons :

    btnAjoutFantome = new PacmanButton("Ajout de fantome",this);
    btnAjoutFantome->setGeometry(10,2.5,150,hauteurCase-5);
    btnAjoutFantome->setStyleSheet("background-color: white");

    btnRetraitFantome = new PacmanButton("Retrait de fantome",this);
    btnRetraitFantome->setGeometry(170,2.5,150,hauteurCase-5);
    btnRetraitFantome->setStyleSheet("background-color: white");

    btnPause = new PacmanButton("Pause",this);
    btnPause->setGeometry(330,2.5,150,hauteurCase-5);
    btnPause->setStyleSheet("background-color: white");

    btnFin = new PacmanButton("Fin",this);
    btnFin->setGeometry(500,2.5,150,hauteurCase-5);
    btnFin->setStyleSheet("background-color: white");

    connect(btnAjoutFantome, &QPushButton::clicked, this, &PacmanWindow::ajoutFantome);

    connect(btnRetraitFantome, &QPushButton::clicked, this, &PacmanWindow::retraitFantome);

    connect(btnPause, &QPushButton::clicked, this, &PacmanWindow::Pause);

    connect(btnFin, &QPushButton::clicked, this, &PacmanWindow::finDeJeu);


    resize(jeu.getNbCasesX()*largeurCase, jeu.getNbCasesY()*(hauteurCase+2));
}
///////////////////////////////////////////AFFICHAGE///////////////////////////////////////////
 
void PacmanWindow::paintEvent(QPaintEvent *)
{
    //QPainter painter(this);
    QImage image(size(), QImage::Format_ARGB32_Premultiplied);
    //image.fill(Qt::white);

    // Dessiner quelque chose sur l'image
    QPainter painter(&image);
    

   

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
        /*QPixmap p = pixmapPacman[1];
        for (int i=0;i<jeu.getNbCasesX();i++)
        {
            painter.drawPixmap(i*largeurCase, (jeu.getNbCasesY()/4)*hauteurCase, p);
            painter.drawPixmap(i*largeurCase, (jeu.getNbCasesY()/4)*hauteurCase*4, p);
        }*/

        //On cache les boutons
        btnAjoutFantome->hide();
        btnRetraitFantome->hide();
        btnPause->hide();
        btnFin->hide();
        // on change le fond et on écrit le texte final
        this->setStyleSheet("background-color: rgb(153, 115, 0);");//change la couleur du fond
        painter.setFont(QFont(PacmanFont, 60));
        painter.drawText(rect(), Qt::AlignCenter, tr("100000000000000009\n\nFIN DU JEU\n\n100000000000000009"));// 1,0,9 correspondent a des images

        QTimer::singleShot(3000, this, &PacmanWindow::close);

    }
    else if(etatJeu==PAUSE)
    {   
        painter.drawPixmap(0, 0, pixmapJeu);
        // Calculer les coordonnées du centre de l'écran
        int centerX = jeu.getNbCasesX()*largeurCase/2;
        int centerY = jeu.getNbCasesY()*(hauteurCase+1)/2;

        // Définir les dimensions du rectangle
        int lrect = 500;
        int hrect = 100;

        // Créer un QRect centré sur la fenêtre
        QRect rectangle(centerX - lrect / 2, centerY - hrect / 2, lrect, hrect);

        painter.setFont(QFont(PacmanFont, 50));//définit la police et la taille du texte
        painter.fillRect(rectangle, Qt::gray);
        painter.drawText(rectangle, Qt::AlignCenter, tr("PAUSE"));

    }

    // Créer un QPixmap et dessiner l'image sur le pixmap
    QPixmap pixmap(size());
    pixmap.fill(Qt::transparent);
    QPainter pixmapPainter(&pixmap);
    pixmapPainter.drawImage(0, 0, image);

    // Enregistrer le pixmap dans un fichier
    //pixmap.save("image.png");

    // Afficher l'image dans le widget
    QPainter widgetPainter(this);
    widgetPainter.drawPixmap(0, 0, pixmap);

    if (etatJeu==PLAY)//Si le jeu est sur play alors on enregistre l'image de jeu
        pixmapJeu = pixmap;


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
    else if (event->key()==Qt::Key_Escape || event->key()==Qt::Key_P )
        Pause();
    else if (event->key()==Qt::Key_F2)
        screenShot();
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

void PacmanWindow::screenShot() {//prend un screen du jeu et pas de la fenettre courante
    cout<<"screenshot"<<endl;
    

    QString path = QCoreApplication::applicationDirPath() +"/screen";

    // Vérifier si le dossier existe déjà
    QDir dir(path);
    if (!dir.exists()) {
        // Créer le dossier
        cout<<"creation du dossier"<<endl;
        dir.mkpath(".");
    }
    //Le dossier est présent alors on enregistre l'image


    // Obtenir la date et l'heure actuelles en chaîne de caractères
    QString currentDateTimeString = QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss");

    // Créer le lien pour enregistrer l'image avec la date et l'heure dans le nom de fichier
    QString pathImage = "screen/image-" + currentDateTimeString + ".png";

    pixmapJeu.save(pathImage);//Sauvegarde de l'image de jeu

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




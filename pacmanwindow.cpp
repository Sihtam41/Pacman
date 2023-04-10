#include <iostream>
#include "pacmanwindow.hpp"
#include "jeu.hpp"

using namespace std;

PacmanWindow::PacmanWindow(QWidget *pParent, Qt::WindowFlags flags):QFrame(pParent, flags)
{
    initImages();
    initFont();
    initBtn();
    initTimer();
    initMenu();

    setWindowTitle("Pacman");//Titre de la fenêtre

    QIcon myIcon("./data/icon.png"); // Créer un objet QIcon à partir de l'image
    setWindowIcon(myIcon); // Définir l'icône de la fenêtre

}

///////////////////////////////////////////AFFICHAGE///////////////////////////////////////////

void PacmanWindow::paintEvent(QPaintEvent *)
{
    // Créer une image de la taille de la fenêtre
    QImage image(size(), QImage::Format_ARGB32_Premultiplied);
    // Dessiner quelque chose sur l'image
    QPainter painter(&image);

    if (etatJeu==PLAY)
    {
        afficheJeu(&painter);//affiche du jeu
    }
    else if (etatJeu == FIN)//affichage fin du jeu
    {
        affichageFin(&painter);
    }
    else if(etatJeu==PAUSE)
    {
        affichagePause(&painter);

    }
    else if(etatJeu==MENU)
    {
        painter.drawPixmap(0,0,pixmapMenu);
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
    if (etatJeu!=FIN)
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

    QTimer::singleShot(3000, this, initMenu);
    etatJeu = FIN;
    timerJeu->stop();
    animation=false;
    update();
}

void PacmanWindow::Pause(){

    if (timerJeu->isActive())
        {
            timerJeu->stop();
            etatJeu=PAUSE;
            btnAjoutFantome->hide();
            btnRetraitFantome->hide();

        }
    else
        {
            timerJeu->start(20);
            etatJeu=PLAY;
            btnAjoutFantome->show();
            btnRetraitFantome->show();

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

void PacmanWindow::lancerTimerJeu()
{
    timerJeu->start(timerJeuInterval);
    animation=true;
}

PacmanButton::PacmanButton(const QString &text, QWidget *parent) : QPushButton(text,parent)
{
}


void PacmanButton::keyPressEvent(QKeyEvent *e)
{
    if (parent()!=nullptr)
        QCoreApplication::sendEvent(parent(), e);
}
/////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////AFFICHAGE/////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
bool PacmanWindow::afficheJeu(QPainter* painter)
{
    // Taille des cases en pixels
    int largeurCase, hauteurCase;

    largeurCase = pixmapMur.width();
    hauteurCase = pixmapMur.height();

    // Dessine les cases
    for (int y=0;y<jeu.getNbCasesY();y++)
        for (int x=0;x<jeu.getNbCasesX();x++)
            if (jeu.getCase(x,y)==MUR)
                painter->drawPixmap(x*largeurCase, (y+1)*hauteurCase, pixmapMur);
            else if (jeu.getCase(x,y)==VIDE)
                painter->drawPixmap(x*largeurCase, (y+1)*hauteurCase, pixmapVide);

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
        painter->drawPixmap(itFantome->getPosX()*largeurCase, (itFantome->getPosY()+1)*hauteurCase, pixmapFanome);
    }

    //dessine les pacballs
    list<PacBalls> &yep = jeu.getPacBalls();
    list<PacBalls>::iterator itPacballs;
    for (itPacballs=yep.begin(); itPacballs!=yep.end(); itPacballs++)
        painter.drawPixmap(itPacballs->getPosX()*largeurCase, (itPacballs->getPosY()+1)*hauteurCase, pixmapPacball);

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

    painter->drawPixmap(jeu.getPacmanX()*largeurCase, (jeu.getPacmanY()+1)*hauteurCase, p);
    // prochaine image du pacman
    if (animation)//si l'animation est activée
    {
        imagePacman++;
        imagePacman%=4;
    }


    return true;

}

bool PacmanWindow::affichageFin(QPainter* painter)
{

    //On cache les boutons
    if (btnAjoutFantome!= nullptr)
    {
        btnAjoutFantome->hide();
        btnRetraitFantome->hide();
        btnPause->hide();
        btnFin->hide();
    }

    // on change le fond et on écrit le texte final
    this->setStyleSheet("background-color: rgb(153, 115, 0);");//change la couleur du fond
    painter->setFont(QFont(PacmanFontFile, 50));
    painter->drawText(rect(), Qt::AlignCenter, tr("100000000000000009\n\nFIN DU JEU\n\n100000000000000009"));// 1,0,9 correspondent a des images

    return true;

}

bool PacmanWindow::affichagePause(QPainter* painter)
{
    painter->drawPixmap(0, 0, pixmapJeu);//affiche l'image de jeu

    // Taille des cases en pixels
    int largeurCase, hauteurCase;

    largeurCase = pixmapMur.width();
    hauteurCase = pixmapMur.height();
    // Calculer les coordonnées du centre de l'écra
    int centerX = jeu.getNbCasesX()*largeurCase/2;
    int centerY = jeu.getNbCasesY()*(hauteurCase+1)/2;

    // Définir les dimensions du rectangle
    int lrect = 500;
    int hrect = 100;

    // Créer un QRect centré sur la fenêtre
    QRect rectangle(centerX - lrect / 2, centerY - hrect / 2, lrect, hrect);

    painter->setFont(QFont(PacmanFontFile, 50));//définit la police et la taille du texte
    painter->fillRect(rectangle, Qt::gray);
    painter->drawText(rectangle, Qt::AlignCenter, tr("PAUSE"));
}




/////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////INITIALISATION///////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

bool PacmanWindow::initJeu()
{
    etatJeu=PLAY;//Le jeu est sur play

    //affichage des boutons
    btnAjoutFantome->show();
    btnRetraitFantome->show();
    btnPause->show();
    btnFin->show();
    btnQuitter->hide();
    btnLancerJeu->hide();

    this->setStyleSheet("background-color: rgb(80, 80, 80);");//Couleur de fond

    jeu.init();//Initialisation du jeu



    // Taille des cases en pixels
    int largeurCase = pixmapMur.width();
    int hauteurCase = pixmapMur.height();

    resize(jeu.getNbCasesX()*largeurCase, jeu.getNbCasesY()*(hauteurCase+2));//redimensionne la fenetre

     QTimer::singleShot(2000, this, lancerTimerJeu); // lance le timer Jeu  après 2s

    //affichage du score
    QFont font("Times", 9, QFont::Bold);

    AffichageScore = new QLabel(this);
    AffichageScore->setText("score :");
    AffichageScore->setGeometry(360, 2.5, 150, hauteurCase-5);
    AffichageScore->setStyleSheet("color : white");
    AffichageScore->setFont(font);

    valeurScore = new QLabel(this);
    valeurScore->setText("0");
    valeurScore->setGeometry(430, 2.5, 150, hauteurCase-5);
    valeurScore->setStyleSheet("color : white");
    valeurScore->setFont(font);

    return true;
}

bool PacmanWindow::initMenu()
{
    etatJeu = MENU;

    this->move(100, 100);
    this->setStyleSheet("background-color: rgb(0, 0, 0);");//Couleur de fond
    resize(976,800);

    btnPause->hide();
    btnFin->hide();
    btnAjoutFantome->hide();
    btnRetraitFantome->hide();
    btnLancerJeu->show();
    btnQuitter->show();



    return true;
}

bool PacmanWindow::initBtn()
{
    // Déclaration des boutons :

    //Boutons du Menu:
    btnLancerJeu = new PacmanButton("", this);
    btnLancerJeu->setGeometry(318,401,330,110);
    connect(btnLancerJeu, &QPushButton::clicked, this, &PacmanWindow::initJeu);
    btnLancerJeu->setStyleSheet("background-color: rgba(0,0,0,0); border: none;");
    btnLancerJeu->hide();

    btnQuitter = new PacmanButton("", this);
    btnQuitter->setGeometry(318,601,330,110);
    connect(btnQuitter, &QPushButton::clicked, this, &PacmanWindow::close);
    btnQuitter->setStyleSheet("background-color: rgba(0,0,0,0); border: none;");
    btnQuitter->hide();

    //Boutons du Jeu:
    btnAjoutFantome = new PacmanButton("Ajout de fantome",this);
    btnAjoutFantome->setGeometry(10,2.5,150,32-5);
    btnAjoutFantome->setStyleSheet("background-color: white");
    btnAjoutFantome->hide();
    connect(btnAjoutFantome, &QPushButton::clicked, this, &PacmanWindow::ajoutFantome);

    btnRetraitFantome = new PacmanButton("Retrait de fantome",this);
    btnRetraitFantome->setGeometry(170,2.5,150,32-5);
    btnRetraitFantome->setStyleSheet("background-color: white");
    connect(btnRetraitFantome, &QPushButton::clicked, this, &PacmanWindow::retraitFantome);
    btnRetraitFantome->hide();

    btnPause = new PacmanButton("Pause",this);
    btnPause->setGeometry(330,2.5,150,32-5);
    btnPause->setStyleSheet("background-color: white");
    connect(btnPause, &QPushButton::clicked, this, &PacmanWindow::Pause);
    btnPause->hide();

    btnFin = new PacmanButton("Fin",this);
    btnFin->setGeometry(500,2.5,150,32-5);
    btnFin->setStyleSheet("background-color: white");
    connect(btnFin, &QPushButton::clicked, this, &PacmanWindow::finDeJeu);
    btnFin->hide();

    return true;

}
bool PacmanWindow::initTimer()
{
    timerJeu = new QTimer(this);
    connect(timerJeu, &QTimer::timeout, this, &PacmanWindow::handleTimerJeu);

    timerAffichage = new QTimer(this);
    connect(timerAffichage, &QTimer::timeout, this, &PacmanWindow::handleTimerAffichage);
    timerAffichage->start(timerAffichageInterval);//start du timerAffichage (ne s'arrête jamais)
}

bool PacmanWindow::initFont()
{
    int id = QFontDatabase::addApplicationFont("./PAC-FONT.TTF");//Chargement de la police
    PacmanFontFile = QFontDatabase::applicationFontFamilies(id).at(0);//Récupération du nom de la police
    return true;
}

bool PacmanWindow::initImages()
{
    if (pixmapPacman[0].load("./data/pacman1.bmp")==false)
    {
        cout<<"Impossible d'ouvrir pacman1.bmp"<<endl;
        exit(-1);
    }
        if (pixmapPacman[1].load("./data/pacman2.bmp")==false)
    {
        cout<<"Impossible d'ouvrir pacman2.bmp"<<endl;
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
    //image pacball
    if (pixmapPacball.load("./data/pacBall.bmp")==false)
    {
        cout<<"Impossible d'ouvrir pacBall.bmp"<<endl;
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
    if (pixmapMenu.load("./data/PacmanMenu.png")==false)
    {
        cout<<"Impossible d'ouvrir PacmanMenu.png"<<endl;
        exit(-1);
    }
    return true;
}

void PacmanWindow::ActualiserScore()
{
    valeurScore->setText(QString::number(jeu.getScore()));

}




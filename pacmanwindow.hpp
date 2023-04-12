#ifndef PACMANWINDOW_HPP
#define PACMANWINDOW_HPP

#include <QtCore/QtCore>
#include <QtWidgets/QtWidgets>
#include "jeu.hpp"

typedef enum {FIN, PAUSE, PLAY, MENU,MENU_LEVELS} EtatJeu;

class PacmanButton;

class PacmanWindow : public QFrame
{
  protected:
    Jeu jeu;
    QPixmap pixmapMur, pixmapVide;
    QPixmap pixmapPacman [4];
    QPixmap pixmapFantomeRouge, pixmapFantomeCyan, pixmapFantomeOrange, pixmapFantomeFuyard;
    QPixmap pixmapJeu;
    QPixmap pixmapPacball, pixmapBall;
    QPixmap pixmapMenu;
    QPixmap pixmapMenuLevels;
    int imagePacman;
    EtatJeu etatJeu;
    int levelSelected;

    //Boutons
    PacmanButton *btnPause, *btnFin, *btnAjoutFantome, *btnRetraitFantome;
    PacmanButton *btnLancerJeu, *btnQuitter, *btnLevel1, *btnLevel2, *btnLevel3,*btnLevel4;

    QTimer *timerAffichage, *timerJeu;//timer
    int timerAffichageInterval=40, timerJeuInterval=20;//interval des timers
    bool animation=false;//variable qui définit si les animations sont activées ou non

    QLabel *AffichageScore, *valeurScore;

    QString PacmanFontFile;// chemin vers la police d'écriture

  public:
    PacmanWindow(QWidget *pParent=0, Qt::WindowFlags flags=Qt::WindowFlags());
    //méthodes de gestion des timers:
    void handleTimerJeu();//Méthode qui gère ce qui se passe à chaque tick du timerJeu
    void handleTimerAffichage();
    void lancerTimerJeu();

    void ajoutFantome();
    void retraitFantome();

    void finDeJeu();//Méthode qui gère la fin du jeu
    void Pause();//Met sur pause ou sur play le jeu

    // méthode d'initialisation:
    bool initJeu();// initialise le jeu
    bool initBtn();// initialise les boutons et les caches
    bool initImages();// initialise les images
    bool initTimer();// initialise les timers
    bool initFont();// initialise la police
    bool initMenu();// initialise le menu
    bool initLabel();// initialise les labels
    void initMenuLevels();// initialise le menu de choix de niveaux

    // méthodes de gestion des boutons qui permettent de choisir le niveau
    void levelSelected1();
    void levelSelected2();
    void levelSelected3();
    void levelSelected4();
    void NiveauSuivant();

    // méthode d'affichage:
    bool afficheJeu(QPainter*);
    bool affichagePause(QPainter*);
    bool affichageFin(QPainter*);
    bool affichageMenu(QPainter*);

    void ActualiserScore();

    void screenShot();

  protected:
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *);

};


class PacmanButton : public QPushButton
{
    public:
        PacmanButton(const QString &text, QWidget *parent = nullptr);
        void keyPressEvent(QKeyEvent *);
};


#endif

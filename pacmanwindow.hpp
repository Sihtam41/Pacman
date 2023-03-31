#ifndef PACMANWINDOW_HPP
#define PACMANWINDOW_HPP

#include <QtCore/QtCore>
#include <QtWidgets/QtWidgets>
#include "jeu.hpp"

typedef enum {FIN, PAUSE, PLAY, MENU} EtatJeu;

class PacmanButton;

class PacmanWindow : public QFrame
{
  protected:
    Jeu jeu;
    QPixmap  pixmapMur, pixmapVide;
    QPixmap pixmapPacman [4];
    QPixmap pixmapFantomeRouge, pixmapFantomeCyan, pixmapFantomeOrange, pixmapFantomeFuyard;
    QPixmap pixmapJeu;
    QPixmap pixmapMenu;
    int imagePacman;
    EtatJeu etatJeu=MENU;

    PacmanButton *btnPause, *btnFin, *btnAjoutFantome, *btnRetraitFantome;
    PacmanButton *btnLancerJeu, *btnQuitter;

    QTimer *timerAffichage, *timerJeu;

    QString PacmanFont;

  public:
    PacmanWindow(QWidget *pParent=0, Qt::WindowFlags flags=Qt::WindowFlags());
    void handleTimerJeu();
    void handleTimerAffichage();
    void ajoutFantome();
    void retraitFantome();
    void finDeJeu();
    void Pause();//Met sur pause ou sur play le jeu
    bool init();
    bool lancerJeu();
    void initImages();
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
#ifndef PACMANWINDOW_HPP
#define PACMANWINDOW_HPP

#include <QtCore/QtCore>
#include <QtWidgets/QtWidgets>
#include "jeu.hpp"

typedef enum {FIN, PAUSE, PLAY} EtatJeu;

class PacmanButton;

class PacmanWindow : public QFrame
{
  protected:
    Jeu jeu;
    QPixmap  pixmapMur, pixmapVide;
    QPixmap pixmapPacman [4];
    QPixmap pixmapFantomeRouge, pixmapFantomeCyan, pixmapFantomeOrange, pixmapFantomeFuyard;
    QPixmap pixmapJeu;
    int imagePacman;
    EtatJeu etatJeu=PLAY;

    PacmanButton *btnPause, *btnFin, *btnAjoutFantome, *btnRetraitFantome;

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
    void onGameOver();
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
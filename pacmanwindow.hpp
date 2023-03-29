#ifndef PACMANWINDOW_HPP
#define PACMANWINDOW_HPP

#include <QtCore/QtCore>
#include <QtWidgets/QtWidgets>
#include "jeu.hpp"

typedef enum {FIN, PAUSE, PLAY} EtatJeu;

class PacmanWindow : public QFrame
{
  protected:
    Jeu jeu;
    QPixmap  pixmapMur, pixmapVide;
    QPixmap pixmapPacman [4];
    QPixmap pixmapFantomeRouge, pixmapFantomeCyan, pixmapFantomeOrange, pixmapFantomeFuyard;
    int imagePacman;
    EtatJeu etatJeu=PLAY;//1 si le jeu est en fonction et 0 si la fin du jeu

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
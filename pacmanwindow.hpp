#ifndef PACMANWINDOW_HPP
#define PACMANWINDOW_HPP

#include <QtCore/QtCore>
#include <QtWidgets/QtWidgets>
#include "jeu.hpp"

class PacmanWindow : public QFrame
{
  protected:
    Jeu jeu;
    QPixmap  pixmapMur, pixmapVide;
    QPixmap pixmapPacman [4];
    QPixmap pixmapFantomeRouge, pixmapFantomeCyan, pixmapFantomeOrange, pixmapFantomeFuyard;
    int imagePacman;
    int etatJeu;//1 si le jeu est en fonction et 0 si la fin du jeu

    QTimer *timerAffichage, *timerJeu;

  public:
    PacmanWindow(QWidget *pParent=0, Qt::WindowFlags flags=Qt::WindowFlags());
    void handleTimerJeu();
    void handleTimerAffichage();
    void ajoutFantome();
    void retraitFantome();
    void finDeJeu();
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
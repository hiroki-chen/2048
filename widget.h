#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QFile>
#include "game.h"

#define LBLSTYLESHEET "QLabel { color:#000000; background:#90EE90; border-radius:%1px; }"
#define BTNSTYLESHEET "QPushButton {color:#FFFFFF; background:#87CEFF; border-radius:%1px; } QPushButton:pressed{ color:blue; background:blue; border-radius:%1px; }"

class Widget : public QWidget {
    Q_OBJECT

public:
    Widget(QWidget* parent = nullptr);
    ~Widget();

private:
    QMenuBar* menuBar;
    QMenu* optionMenu;
    QMenu* windowMenu;

    QAction* minimizeAction;
    QAction* maximizeAction;
    QAction* restartAction;
    QAction* closeAction;


    QPushButton* restartBtn;
    QLabel* scoreLbl;
    QLabel* highestScoreLbl;
    GameWidget* gameWidget;
    qreal xDiff, yDiff;
    int highestScore;

protected:
    void resizeEvent(QResizeEvent*);

public slots:
    void onScoreInc(int);
    void onGameOver();
    void onWin();
};

#endif

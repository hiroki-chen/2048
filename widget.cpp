#include "widget.h"

Widget::Widget(QWidget *parent): QWidget(parent) {
    highestScore = 0;
    QFile file("score.file");
    if (file.open(QIODevice::ReadOnly)) {
        file.read((char *)&highestScore, sizeof(highestScore));
        file.close();
    }

    gameWidget = new GameWidget(this);
    gameWidget->setGeometry(2, 200, 400, 400);
    connect(gameWidget, SIGNAL(scoreInc(int)), this, SLOT(onScoreInc(int)));
    connect(gameWidget, SIGNAL(GameOver()), this, SLOT(onGameOver()));
    connect(gameWidget, SIGNAL(win()), this, SLOT(onWin()));

    menuBar = new QMenuBar(this);
    menuBar->setGeometry(0, 0, width(), 40);
    optionMenu = new QMenu("选项", this);
    windowMenu = new QMenu("窗口", this);
    restartAction = new QAction("重新开始", this);
    restartAction->setShortcut(QKeySequence("ctrl+alt+R"));
    closeAction = new QAction("结束游戏", this);
    closeAction->setShortcut(QKeySequence("ctrl+alt+Q"));
    minimizeAction = new QAction("窗口最小化", this);
    minimizeAction->setShortcut(QKeySequence("ctrl+M"));
    maximizeAction = new QAction("窗口最大化", this);
    maximizeAction->setShortcut(QKeySequence("ctrl+alt+M"));

    connect(closeAction, SIGNAL(triggered()), this, SLOT(close()));
    connect(restartAction, SIGNAL(triggered()), gameWidget, SLOT(restart()));
    connect(minimizeAction, SIGNAL(triggered()), this, SLOT(showMinimized()));
    connect(maximizeAction, SIGNAL(triggered()), this, SLOT(showMaximized()));
    optionMenu->addAction(restartAction);
    optionMenu->addAction(closeAction);
    windowMenu->addAction(minimizeAction);
    windowMenu->addAction(maximizeAction);
    menuBar->addMenu(windowMenu);
    menuBar->addMenu(optionMenu);

    QFont font;
    font.setFamily("helvetica");
    font.setBold(true);
    font.setPixelSize(25);

    restartBtn = new QPushButton("重新开始", this);
    restartBtn->setGeometry(100, 120, 200, 50);
    restartBtn->setFont(font);
    restartBtn->setStyleSheet(QString(BTNSTYLESHEET).arg(3).arg(15));
    connect(restartBtn, SIGNAL(clicked()), gameWidget, SLOT(restart()));

    highestScoreLbl = new QLabel(QString("最高分:\n%1").arg(highestScore),this);
    highestScoreLbl->setGeometry(209, 20, 180, 70);
    highestScoreLbl->setFont(font);
    highestScoreLbl->setAlignment(Qt::AlignCenter);
    highestScoreLbl->setStyleSheet(QString(LBLSTYLESHEET).arg(5).arg(20));

    scoreLbl = new QLabel("分数:\n0", this);
    scoreLbl->setGeometry(15, 20, 180, 70);
    scoreLbl->setFont(font);
    scoreLbl->setAlignment(Qt::AlignCenter);
    scoreLbl->setStyleSheet(QString(LBLSTYLESHEET).arg(5).arg(20));

    this->setFixedSize(480,760);
}

Widget::~Widget() {
    delete restartBtn;
    delete scoreLbl;
    delete highestScoreLbl;
    delete gameWidget;
}

void Widget::onScoreInc(int score) {
    scoreLbl->setText(QString("分数:\n%1").arg(score));
    if (score > highestScore) {
        highestScore = score;
        highestScoreLbl->setText(QString("最高分:\n%1").arg(highestScore));

        QFile file("score.file");
        file.open(QIODevice::WriteOnly);
        file.write((char*)&highestScore, sizeof(highestScore));
        file.close();
    }
}

void Widget::onGameOver() { QMessageBox::warning(this, "GameOver", "你输了！"); }
void Widget::onWin() { QMessageBox::information(this, "Congratulation", "你赢了!"); }

void Widget::resizeEvent(QResizeEvent*) {
    xDiff = width() / 404.0;
    yDiff = height() / 606.0;
    QFont font;
    font.setFamily("helvetica");
    font.setBold(true);
    font.setPixelSize(25 * yDiff);
    restartBtn->setFont(font);
    highestScoreLbl->setFont(font);
    scoreLbl->setFont(font);
    restartBtn->setStyleSheet(QString(BTNSTYLESHEET).arg(3 * xDiff).arg(15 * xDiff));
    highestScoreLbl->setStyleSheet(QString(LBLSTYLESHEET).arg(5 * xDiff).arg(20 * xDiff));
    scoreLbl->setStyleSheet(QString(LBLSTYLESHEET).arg(5 * xDiff).arg(20 * xDiff));
    gameWidget->setGeometry(2 * xDiff, 200 * yDiff, 400 * xDiff, 400 * yDiff);
    restartBtn->setGeometry(15 * xDiff, 20 * yDiff, 180 * xDiff, 70 * yDiff);
    highestScoreLbl->setGeometry(209 * xDiff, 20 * yDiff, 180 * xDiff, 70 * yDiff);
    scoreLbl->setGeometry(15 * xDiff, 110 * yDiff, 374 * xDiff, 70 * yDiff);
}

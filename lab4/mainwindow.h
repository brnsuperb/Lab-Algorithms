#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QFile>
#include <QGraphicsScene>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QGraphicsScene *sc;
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void draw1();//По списку
    void draw2();//По матрице
    class Pair//класс "Ребро"
    {
    public:
        int start;//стартовая вершина
        QString end;//конечная вершина
    };
    QVector<int> variety1;//Множество станков
    QVector<QString> variety2;//Множество деталей
    QVector<Pair> list;//список ребер
    bool matrix[20][20];//Матрица смежности
    bool edge1(int vertex);
    bool edge2(int vertex, int size);
private:
    Ui::MainWindow *ui;

public slots:
    void work();
};

#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    class Pair{//Ребро
    public:
        int start;//Начало
        int end;  //Конец
    };
    class Vertex//
    {
    public:
        int v;      //Вершина
        int degree; //Степень веришины
        QVector<int> colors;//Цвета, которые содержит вершина
    };

    class Edge//Ребро
    {
    public:
        int start;//Стартовая вершина
        int end;    //Конечная вершина
        int color;  //Цвет ребра
    };

    //Vert massiv[20];
    QGraphicsScene *sc;
    QVector<Pair> list;//Исходный список ребер
    QVector<Edge> newList;//Список ребер с их цветами
    bool matrix[20][20];//матрица смежности
    Vertex deg[20];     //Массив вершин
    Vertex degree[20];  //Массив вершин
    QVector<Pair> listing;
    QVector<Edge> newListing;
    void draw1();
    void draw2();
    void find1();
    void find2();
public slots:
    void work();
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

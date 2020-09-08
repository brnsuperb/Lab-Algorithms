#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextStream>
#define PI 3.14
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::work()
{
    if (ui->radioButton->isChecked())
        draw1();
    if (ui->radioButton_2->isChecked())
        draw2();
}

bool MainWindow::edge1(int vertex)
{
    bool flag = true;
    for (int i = 0;i < list.size();i++)
    {
        if (list[i].start == vertex)
        {
            flag = false;
            break;
        }
    }
    return flag;
}

bool MainWindow::edge2(int vertex, int size)
{
    bool flag = true;
    for (int i = 0;i < size;i++)
    {
        if (matrix[i][vertex])
        {
            flag = false;
            break;
        }
    }
    return flag;
}

void MainWindow::draw1()//Считывание с файла со списком и визуализация
{
    variety1.clear();
    variety2.clear();
    QFile file;
    QTextStream inp(&file);
    file.setFileName("/home/superb/lab4/list.txt");
    file.open(QIODevice::ReadOnly);
    int count = 0;//Количество вершин
    QString s = inp.readLine();
    count = s.toInt();
    //Заполняем множества станков и деталей
    for (int i = 0;i < count;i++) variety1.push_back(i + 1);
    s = inp.readLine();
    QStringList lst = s.split(" ");
    for (int i = 0;i < lst.size();i++) variety2.push_back(lst[i]);

    while(!inp.atEnd())
    {
        s = inp.readLine();
        QStringList lst1 = s.split(" ");
        Pair A;
        A.start = lst1[0].toInt();
        A.end = lst1[1];
        list.push_back(A);
    }
    file.close();

    sc = new QGraphicsScene();
    QPixmap qp(ui->graphicsView->size());
    QPainter pn(&qp);
    pn.fillRect(QRect(0,0,ui->graphicsView->width(),ui->graphicsView->height()),QBrush(Qt::white));
    QPen pen;
    pen.setWidth(2);
    pn.setPen(pen);
    int h1, w1, h2, w2;
    QPoint point1[variety1.size()];//Массив точек-координат станков
    QPoint point2[variety2.size()];//Массив точек-координат деталей
    h1 = ui->graphicsView->height() / 4;
    w1 = ui->graphicsView->width() / (variety1.size() + 1);
    w2 = ui->graphicsView->width() / (variety2.size() + 1);
    for (int i = 0;i < variety1.size();i++)
    {
        point1[i].setX((i + 1)  * w1);
        point1[i].setY(h1);
    }
    for (int i = 0;i < variety2.size();i++)
    {
        point2[i].setX((i + 1) * w2);
        point2[i].setY(2 * h1);
    }
    for (int i = 0;i < list.size();i++)
    {
        Pair A = list[i];
        int index = 0;
        for (int j = 0;j < variety2.size();j++)
        {
            if (A.end == variety2[j])
            {
                index = j;
                break;
            }
        }
        pn.drawLine(point1[A.start - 1],point2[index]);
    }
    pn.setBrush(QBrush(Qt::gray, Qt::SolidPattern));
    for (int i = 0;i < variety1.size();i++)
    {
        pn.drawEllipse(point1[i],20,20);
        pn.drawText(point1[i],QString::number(i + 1));
    }

    for (int i = 0;i < variety2.size();i++)
    {
        pn.drawEllipse(point2[i],20,20);
        pn.drawText(point2[i],variety2[i]);
    }
    QString answer = "";
    for (int i = 0;i < variety1.size();i++)
    {
        if (!edge1(variety1[i]))
            answer += QString::number(variety1[i]) + " ";
    }
    ui->label->setText(answer);
    sc->addPixmap(qp);
    ui->graphicsView->setScene(sc);
}

void MainWindow::draw2()//Считывание с файла с матрицей и визуализация
{
    for (int i = 0;i < 20;i++)
        for (int j = 0;j < 20;j++)
            matrix[i][j] = false;
    list.clear();
    QFile file;
    QTextStream inp(&file);
    file.setFileName("/home/superb/lab4/matrix.txt");
    file.open(QIODevice::ReadOnly);
    int rows = 0;//Количество деталей
    int columns = 0;//Количество станков
    QString s = inp.readLine();
    QStringList ls = s.split(" ");
    columns = ls[1].toInt();
    rows = ls[0].toInt();
    s = inp.readLine();
    ls = s.split(" ");
    for (int i = 0;i < rows;i++)
    {
        s = inp.readLine();
        QStringList lst = s.split(" ");
        for (int j = 0;j < lst.size();j++)
        {
            if (lst[j].toInt() == 1)
                matrix[i][j] = true;
        }
    }

    sc = new QGraphicsScene();
    QPixmap qp(ui->graphicsView->size());
    QPainter pn(&qp);
    pn.fillRect(QRect(0,0,ui->graphicsView->width(),ui->graphicsView->height()),QBrush(Qt::white));
    QPen pen;
    pen.setWidth(2);
    pn.setPen(pen);
    QPoint point1[columns];
    QPoint point2[rows];
    int h1 = ui->graphicsView->height() / 4;
    int w1 = ui->graphicsView->width() / (columns + 1);
    int w2 = ui->graphicsView->width() / (rows + 1);
    for (int i = 0;i < columns;i++)
    {
        point1[i].setX((i + 1)  * w1);
        point1[i].setY(h1);
    }
    for (int i = 0;i < rows;i++)
    {
        point2[i].setX((i + 1) * w2);
        point2[i].setY(2 * h1);
    }

    for (int i = 0;i < rows;i++)
    {
        for (int j = 0;j < columns;j++)
        {
            if (matrix[i][j])
            {
                pn.drawLine(point1[j],point2[i]);
            }
        }
    }
    pn.setBrush(QBrush(Qt::gray, Qt::SolidPattern));
    for (int i = 0;i < columns;i++)
    {
        pn.drawEllipse(point1[i],20,20);
        pn.drawText(point1[i],QString::number(i + 1));
    }

    for (int i = 0;i < ls.size();i++)
    {
        pn.drawEllipse(point2[i],20,20);
        pn.drawText(point2[i],ls[i]);
    }
    QString answer = "";
    for (int i = 0;i < columns;i++)
    {
        if (!edge2(i,rows))
            answer += QString::number(i + 1) + " ";
    }
    ui->label->setText(answer);
    sc->addPixmap(qp);
    ui->graphicsView->setScene(sc);
}

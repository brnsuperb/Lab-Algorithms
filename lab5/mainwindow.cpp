#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsScene>
#include <QVector>
#include <QFile>
#include <QTextStream>
#include <QPixmap>
#include <math.h>
#define PI 3.14
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    for (int i = 0;i < 20;i++)
    {
        for (int j = 0;j < 20;j++)
        {
            matrix[i][j] = false;
        }
    }
}

void MainWindow::work()
{
    if (ui->radioButton->isChecked())
        draw1();
    if (ui->radioButton_2->isChecked())
        draw2();
}

void MainWindow::draw1()
{
    list.clear();
    QFile file;
    QTextStream inp(&file);
    file.setFileName("/home/superb/lab5/matrix.txt");
    file.open(QIODevice::ReadOnly);
    int index = 0;

    while(!inp.atEnd())
    {
        QString s = inp.readLine();
        QStringList lst = s.split(" ");
        for (int i = 0;i < lst.size();i++)
        {
            if (lst[i].toInt() == 0)
                matrix[index][i] = false;
            else
                matrix[index][i] = true;
        }
        index++;
    }
    file.close();
    sc = new QGraphicsScene();
    QPixmap qp(ui->graphicsView->size());
    QPainter pn(&qp);
    pn.fillRect(QRect(0,0,ui->graphicsView->width(),ui->graphicsView->height()),QBrush(Qt::white));
    QPen pen;
    pen.setWidth(2);
    pn.setPen(pen);
    QPoint point[index];
    int radius = 150;
    int corner = 0;
    int k = 360/index;
    int xcentr = ui->graphicsView->width() / 2;
    int ycentr = ui->graphicsView->height() / 2;
    for (int i = 0;i < index;i++)
    {
        int x = radius * cos(corner * PI / 180);
        int y = radius * sin(corner * PI / 180);
        point[i].setX(xcentr + x + 20);
        point[i].setY(ycentr + y + 20);
        corner += k;
    }

    for (int i = 0; i < index;i++)
    {
        for (int j = 0;j < index;j++)
        {
            if (!matrix[i][j])
                continue;
            else
            {
                if (i == j)
                {
                    corner = k * i;
                    int x = (radius + 25) * cos(corner * PI / 180);
                    int y = (radius + 25) * sin(corner * PI / 180);
                    pn.drawEllipse(xcentr + x,ycentr + y,30,30);
                }
                else
                    pn.drawLine(point[i],point[j]);
            }
        }
    }

    pn.setBrush(QBrush(Qt::gray, Qt::SolidPattern));
    corner = 0;
    for (int i = 0;i < index;i++)
    {
         int x = radius * cos(corner * PI / 180);
         int y = radius * sin(corner * PI / 180);
         point[i].setX(xcentr + x + 20);
         point[i].setY(ycentr + y + 20);
         pn.drawEllipse(xcentr + x,ycentr + y,40,40);
         pn.drawText(xcentr + x + 15,ycentr + y + 25,QString::number(i + 1));
         corner+=k;
    }

    for (int i = 0;i < index;i++)
    {
        degree[i].v = i + 1;
        degree[i].degree = 0;
    }
    //Нахождение степеней вершин
    for (int i = 0;i < index;i++)
    {
        for (int j = 0;j < index;j++)
        {
            if (matrix[i][j])
                degree[i].degree++;
        }
    }


    int maxDegree = degree[0].degree;
    for (int i = 1;i < index;i++)
    {
        if (degree[i].degree > maxDegree)
            maxDegree = degree[i].degree;
    }
    ui->label->setText("Макс.степень " + QString::number(maxDegree));
    for (int i = 0;i < index;i++)
    {
        for (int j = 0;j < index;j++)
        {
            if (matrix[i][j])
            {
                Pair A;
                A.start = i + 1;
                A.end = j + 1;
                listing.push_back(A);
            }
        }
    }

    for (int i = 0;i < listing.size();i++)
    {
        Pair A = listing[i];
        for (int j = i;j < listing.size();j++)
        {
            if (i == j) continue;
            if ((listing[j].start == A.end) && (listing[j].end == A.start))
            {
                listing.remove(j);
                break;
            }
        }
    }
    //Нахождение правильной раскраски
    find1();
    //Раскраска ребер
    pn.setBrush(QBrush(Qt::yellow, Qt::SolidPattern));
    double K = 1 /.5;
    for (int i = 0;i < newListing.size();i++)
    {
        Edge A = newListing[i];
        double x = (point[A.start - 1].x() + (point[A.end - 1].x() * K))/(1 + K);
        double y = (point[A.start - 1].y() + (point[A.end - 1].y() * K))/(1 + K);
        QPoint p;
        p.setX(x);
        p.setY(y);
        pn.drawEllipse(p,15,15);
        pn.drawText(p, QString::number(A.color));
    }
    sc->addPixmap(qp);
    ui->graphicsView->setScene(sc);
}

void MainWindow::draw2()
{
    //list.clear();
    QFile file;
    QTextStream inp(&file);
    file.setFileName("/home/superb/lab5/list.txt");
    file.open(QIODevice::ReadOnly);
    QString s = inp.readLine();
    int index = s.toInt();

    while(!inp.atEnd())
    {
        QString s = inp.readLine();
        QStringList lst = s.split(" ");
        Pair A;
        A.start = lst[0].toInt();
        A.end = lst[1].toInt();
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
    QPoint point[index];
    int radius = 150;
    int corner = 0;
    int k = 360/index;
    int xcentr = ui->graphicsView->width() / 2;
    int ycentr = ui->graphicsView->height() / 2;
    for (int i = 0;i < index;i++)
    {
        int x = radius * cos(corner * PI / 180);
        int y = radius * sin(corner * PI / 180);
        point[i].setX(xcentr + x + 20);
        point[i].setY(ycentr + y + 20);
        corner += k;
    }

    for (int i = 0; i < list.size();i++)
    {
        Pair B = list[i];
        if (B.start == B.end)
        {
            corner = k * (i + 1);
            int x = (radius + 25) * cos(corner * PI / 180);
            int y = (radius + 25) * sin(corner * PI / 180);
            pn.drawEllipse(xcentr + x,ycentr + y,30,30);
        }
        else
            pn.drawLine(point[B.start - 1],point[B.end - 1]);
    }

    pn.setBrush(QBrush(Qt::green, Qt::SolidPattern));
    corner = 0;
    for (int i = 0;i < index;i++)
    {
         int x = radius * cos(corner * PI / 180);
         int y = radius * sin(corner * PI / 180);
         point[i].setX(xcentr + x + 20);
         point[i].setY(ycentr + y + 20);
         pn.drawEllipse(xcentr + x,ycentr + y,40,40);
         pn.drawText(xcentr + x + 15,ycentr + y + 25,QString::number(i + 1));
         corner+=k;
    }
    ui->label->setText("work");
    for (int i = 0;i < index;i++)
    {
        deg[i].v = i + 1;
        deg[i].degree = 0;
    }

    for (int i = 0;i < list.size();i++)
    {
        Pair A = list[i];
        if (A.start == A.end)
            deg[A.start - 1].degree++;
        else
        {
            deg[A.start - 1].degree++;
            deg[A.end - 1].degree++;
        }
    }
    for (int n = 1;n < index;n++)
    {
        for (int i = 0;i < index - n;i++)
        {
            if (deg[i].degree < deg[i + 1].degree)
            {
                Vertex A = deg[i];
                deg[i] = deg[i + 1];
                deg[i + 1] = A;
            }
        }
    }

    find2();
    pn.setBrush(QBrush(Qt::yellow, Qt::SolidPattern));
    double K = 1 /.5;
    for (int i = 0;i < newList.size();i++)
    {
        Edge A = newList[i];
        double x = (point[A.start - 1].x() + (point[A.end - 1].x() * K))/(1 + K);
        double y = (point[A.start - 1].y() + (point[A.end - 1].y() * K))/(1 + K);
        QPoint p;
        p.setX(x);
        p.setY(y);
        pn.drawEllipse(p,13,13);
        pn.drawText(p, QString::number(A.color));
    }
    sc->addPixmap(qp);
    ui->graphicsView->setScene(sc);
}

void MainWindow::find1()
{
    newListing.clear();
    for (int i = 0;i < listing.size();i++)
    {
        int currentColor1 = 1;
        int currentColor2 = 1;
        Pair currentEdge = listing[i];
        int step = 0;
        while (step != degree[currentEdge.start - 1].degree)
        {
            for (int j = 0;j < degree[currentEdge.start - 1].colors.size();j++)
            {
                if (degree[currentEdge.start - 1].colors[j] == currentColor1)
                {
                    currentColor1++;
                    break;
                }
            }
            step++;
        }
        step = 0;
        while(step != degree[currentEdge.end - 1].degree)
        {
            for (int j = 0;j < degree[currentEdge.end - 1].colors.size();j++)
            {
                if (degree[currentEdge.end - 1].colors[j] == currentColor2)
                {
                    currentColor2++;
                    break;
                }
            }
            step++;
        }
        if (currentColor1 >= currentColor2)
        {
            for (int j = 0;j < degree[currentEdge.end - 1].colors.size();j++)
            {
                if (degree[currentEdge.end - 1].colors[j] == currentColor1)
                    currentColor1++;
            }
            Edge B;
            B.start = currentEdge.start;
            B.end = currentEdge.end;
            B.color = currentColor1;
            degree[currentEdge.start - 1].colors.push_back(currentColor1);
            degree[currentEdge.end - 1].colors.push_back(currentColor1);
            newListing.push_back(B);
        }
        else
        {
            for (int j = 0;j < degree[currentEdge.start - 1].colors.size();j++)
            {
                if (degree[currentEdge.start - 1].colors[j] == currentColor2)
                    currentColor2++;
            }
            Edge B;
            B.start = currentEdge.start;
            B.end = currentEdge.end;
            B.color = currentColor2;
            degree[currentEdge.start - 1].colors.push_back(currentColor2);
            degree[currentEdge.end - 1].colors.push_back(currentColor2);
            newListing.push_back(B);
        }
    }
}

void MainWindow::find2()
{
    newList.clear();
    for (int i = 0;i < list.size();i++)
    {
        int currentColor1 = 1;
        int currentColor2 = 1;
        Pair currentEdge = list[i];
        int step = 0;
        while(step != deg[currentEdge.start - 1].degree)
        {
            for (int j = 0;j < deg[currentEdge.start - 1].colors.size();j++)
            {
                if (deg[currentEdge.start - 1].colors[j] == currentColor1)
                {
                    currentColor1++;
                    break;
                }
            }
            step++;
        }
        step = 0;
        while(step != deg[currentEdge.end - 1].degree)
        {
            for (int j = 0;j < deg[currentEdge.end - 1].colors.size();j++)
            {
                if (deg[currentEdge.end - 1].colors[j] == currentColor2)
                {
                    currentColor2++;
                    break;
                }
            }
            step++;
        }
        if (currentColor1 > currentColor2)
        {
            for (int j = 0;j < deg[currentEdge.end - 1].colors.size();j++)
            {
                if (deg[currentEdge.end - 1].colors[j] == currentColor1)
                    currentColor1++;
            }
            Edge B;
            B.start = currentEdge.start;
            B.end = currentEdge.end;
            B.color = currentColor1;
            deg[currentEdge.start - 1].colors.push_back(currentColor1);
            deg[currentEdge.end - 1].colors.push_back(currentColor1);
            newList.push_back(B);
        }
        else
        {
            step = 0;
            while(step != deg[currentEdge.start - 1].degree)
            {
                for (int j = 0;j < deg[currentEdge.start - 1].colors.size();j++)
                {
                    if (deg[currentEdge.start - 1].colors[j] == currentColor2)
                    {
                        currentColor2++;
                        break;
                    }
                }
                step++;
            }
            step = 0;
            while(step != deg[currentEdge.end - 1].degree)
            {
                for (int j = 0;j < deg[currentEdge.end - 1].colors.size();j++)
                {
                    if (deg[currentEdge.end - 1].colors[j] == currentColor1)
                    {
                        currentColor1++;
                        break;
                    }
                }
                step++;
            }
            for (int j = 0;j < deg[currentEdge.start - 1].colors.size();j++)
            {
                if (deg[currentEdge.start - 1].colors[j] == currentColor2)
                    currentColor2++;
            }
            Edge B;
            B.start = currentEdge.start;
            B.end = currentEdge.end;
            B.color = currentColor2;
            deg[currentEdge.start - 1].colors.push_back(currentColor2);
            deg[currentEdge.end - 1].colors.push_back(currentColor2);
            newList.push_back(B);
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

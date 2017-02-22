#include "mainwindow.h"
#include <QApplication>
#include <QtGui>
#include <QMessageBox>
#include <QDebug>


bool validate(QString imgName, bool biggerThan1000) {
    imgName = imgName.left(imgName.lastIndexOf(".TIF"));
    int n = imgName.toInt();

    if(biggerThan1000 && n >= 1000) {
        return true;
    }
    if(!biggerThan1000 && n <= 999) {
        return true;
    }
    return false;
}

void createPDF(QString pdfName, QString path, bool biggerThan1000)
{
    QPdfWriter writer(pdfName);
    writer.setPageSize(QPagedPaintDevice::A4);

    QPainter painter(&writer);

    QDir dir(path);
    QStringList files = dir.entryList();

    QString imgName;
    QImage img;
    QPixmap pixmap;
    bool firstPage = true;
    for(int i = 0; i < files.count(); i++) {
        imgName = files.at(i);
        if(imgName.contains("TIF") && validate(imgName, biggerThan1000)) {

            if(!firstPage)
                writer.newPage();
            else
                firstPage = false;

            img.load(path + "\\" + imgName);
            pixmap = QPixmap::fromImage(img);
            painter.drawPixmap(QRect(0,0,img.width(),img.height()), pixmap);
            qDebug() << "img- w: " <<img.width() << "h: " <<img.height();
            qDebug() << "pdf- w: " <<writer.logicalDpiX()*8.3 << "h: " <<writer.logicalDpiY()*11.7;
        }
    }
    painter.end();
}

QString getSpecificPath(QString index)
{
    if(index.length() > 3)
        index = index.right(index.length() - 1);
    if(index.at(0) == "0")
        index = index.right(index.length() - 1);
    if(index.length() == 1)
        index = "0" + index;


    QString middleNumber = index;
    QString lastNumber = middleNumber.right(1);

    middleNumber = middleNumber.mid(0, middleNumber.length() - 1);

    if(middleNumber.length() == 1)
        middleNumber = "0" + middleNumber;

    return "\\" + lastNumber + "\\" + middleNumber;
}

bool isBiggerThan1000(QString index) {
    if(index.length() >= 4) {
        return true;
    } else {
        return false;
    }
}

QString generatePDFName(QString pdfName) {
    if(pdfName.length() > 50)
        pdfName = pdfName.left(50);
    return pdfName + ".pdf";
}

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);

    const QString basePath = "C:\\Users\\fdaros\\Desktop\\Normas digitalizadas\\DOC_CD\\DATA";
    QString specificPath = "";
    QString fullPath = "";

    QFile inputFile("Index.dx");
    QTextStream in(&inputFile);
    inputFile.open(QIODevice::ReadOnly);
    QString line = in.readLine(); line = in.readLine(); line = in.readLine(); line = in.readLine();

//    while(!in.atEnd()) {
        line = in.readLine();
        QStringList splited = line.split(";");

        QString pdfName = generatePDFName("normasPDF\\" + splited.at(21) + " " + splited.at(22));
        QString index = splited.at(1);

        specificPath = getSpecificPath(index);
        fullPath = basePath + specificPath;

        createPDF(pdfName, fullPath, isBiggerThan1000(index));
//    }
    QMessageBox::information(NULL,"Hi!","Image has been written to the pdf file!");
    return 0;
}







#include "thread.h"
#include <QThread>
#include <QDebug>
#include <QPdfWriter>
#include <QPainter>
#include <QDir>
#include <QProcess>

Thread::Thread(int id, QStringList names, QStringList paths, QStringList indexes, int nThreads)
{
    t_Id = id;
    this->nameList = names;
    this->pathList = paths;
    this->indexList = indexes;
    this->stride = nThreads;
}

void Thread::run()
{
//    for(int i = t_Id; i < stridee; i += 6) {
//        createPDF(nameList.at(i), pathList.at(i), isBiggerThan1000(indexList.at(i)));
//    }
    compressPDF();
}

void Thread::createPDF(QString pdfName, QString path, bool biggerThan1000)
{
    pdfName = "normasPDF\\" + pdfName + ".pdf";
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
            painter.drawImage(QRect(0,0, writer.logicalDpiX()*8.3, writer.logicalDpiY()*11.7), img);
        }
    }
    painter.end();
}

bool Thread::isBiggerThan1000(QString index)
{
    if(index.length() >= 4) {
        return true;
    } else {
        return false;
    }
}

bool Thread::validate(QString imgName, bool biggerThan1000) {
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

void Thread::compressPDF()
{
    for(int i = t_Id; i < nameList.count(); i += stride) {
        QProcess process;
        process.start("cmd.exe");
        process.waitForStarted();
        process.write("cd normasPDF\n");
        QString fileName = nameList.at(i);
        fileName = fileName + ".pdf";
        QString compress = "convert.exe -depth 1 -density 200 -compress lzw \"" + fileName +"\" compressed/\""+ fileName + "\"\n"; //density 200
        QByteArray baCompress = compress.toLatin1();
        process.write(compress.toLatin1().data());
        process.closeWriteChannel();
        process.waitForFinished();
    }
}


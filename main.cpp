#include "mainwindow.h"
#include <QApplication>
#include <QtGui>
#include <QMessageBox>
#include <QDebug>
#include <thread.h>

QStringList nameList;
QStringList pathList;
QStringList indexList;

QString getSpecificPath(QString index);
bool isBiggerThan1000(QString index);
QString generatePDFName(QString sigla, QString name, QString index);
bool validate(QString imgName, bool biggerThan1000);
void createPDF(QString pdfName, QString path, bool biggerThan1000);
void compressPDF();
QString unaccent(QString s);
void findMissingFiles(QString path, QString compressedPath);

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);

//    const QString basePath = "C:\\Users\\fdaros\\Desktop\\Normas digitalizadas\\DOC_CD\\DATA";
//    QString specificPath = "";
//    QString fullPath = "";

//    QFile inputFile("Index.dx");
//    QTextStream in(&inputFile);
//    inputFile.open(QIODevice::ReadOnly);
//    QString line = in.readLine(); line = in.readLine(); line = in.readLine(); line = in.readLine();

//    while(!in.atEnd()) {
//        line = in.readLine();
//    line = "679=0;679;0;0;0;0;0;0;1;0;0;1;1;20040314170215;1;20040314170215;1;13;0;0;0;NBR 8148;PAPÉIS E PAPELÕES CELULÓSICOS NOVOS E ENVELHECIDOS, PARA FINS ELÉTRICOS - MEDIDA DO GRAU DE POLIMERIZAÇÃO VISCOSIMÉTRICO MÉDIO - MÉTODO DE ENSAIO;PAPEL. PAPELÃO. ISOLANTE ELÉTRICO;01/2000;CB-03 - COMITÊ BRASILEIRO DE ELETRICIDADE;--;IMPRESSO E DIGITAL;NÃO;--;--;--;08.044;09520090422CXB13_0008;00001;CD 00001";
//        QStringList splited = line.split(";");
//        QString index = splited.at(1);

//        QString pdfName = generatePDFName(splited.at(21), splited.at(22), index);

//        specificPath = getSpecificPath(index);
//        fullPath = basePath + specificPath;
//        pathList.append(fullPath);
//        indexList.append(index);
//        createPDF(pdfName, fullPath, isBiggerThan1000(index));
//    }
//    findMissingFiles("C:\\Users\\fdaros\\Desktop\\i2pdf\\Image2PDF\\normasPDF", "C:\\Users\\fdaros\\Desktop\\i2pdf\\Image2PDF\\normasPDF\\compressed");

//    //    QVector<Thread*> threads;
//    //    int stride = 2;
//    //    for(int i = 0; i < stride; i++) {
//    //        Thread* t = new Thread(i, nameList, pathList, indexList, stride);
//    //        threads.append(t);
//    //        threads.last()->start();
//    //    }

//    //    for(int i = 0; threads.count(); i ++) {
//    //        while(threads.at(i)->isRunning());
//    //    }


//    qDebug() <<"\nCount: " <<nameList.count();
//    compressPDF();

//    //    QMessageBox::information(NULL,"Hi!","Image has been written to the pdf file!");
    return 0;
}

void findMissingFiles(QString path, QString compressedPath)
{
    QDir dir(path);
    QDir dirCompressed(compressedPath);

    QStringList pdfs = dir.entryList();
    QStringList compressedPdfs = dirCompressed.entryList();

    qDebug() << pdfs.count();
    for(int i = 0; i < pdfs.count(); i++) {
        if(!compressedPdfs.contains(pdfs.at(i)))
            qDebug() << pdfs.at(i);
    }
}

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
        painter.drawPixmap(QRect(0,0, writer.logicalDpiX()*8.3, writer.logicalDpiY()*11.7), pixmap);
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

QString generatePDFName(QString sigla, QString name, QString index)
{
    //    if(pdfName == "-- --")
    //        pdfName = index;

    name = unaccent(name).replace("  ", " ");
    QString pdfName = sigla + " " + name;


    if(pdfName.contains("/"));
        pdfName.replace("/", " ");

    if(pdfName.contains("-- "))
        pdfName.replace("-- ", "");

    if(pdfName == "")
        pdfName = index;

    if(pdfName.length() > 35)
        pdfName = pdfName.left(35);

    if(nameList.contains(pdfName)) {
        pdfName = pdfName + "(" + index + ")";
    }

    if(!nameList.contains(pdfName))
        nameList.append(pdfName);

    return "normasPDF\\" + pdfName + ".pdf";
}

void compressPDF()
{
    for(int i = 0; i < nameList.count(); i ++) {
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

QString unaccent(QString s)
{
    QString output(s.normalized(QString::NormalizationForm_D));
    return output.replace(QRegExp("[^a-zA-Z\\s]"), "");
}









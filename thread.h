#ifndef MYTHREAD_H
#define MYTHREAD_H
#include <QThread>

class Thread : public QThread
{
public:
    Thread(int id, QStringList names, QStringList paths, QStringList indexes, int stride);
    void run();

private:
    int t_Id;
    QStringList nameList;
    QStringList pathList;
    QStringList indexList;
    int stride;

    void createPDF(QString pdfName, QString path, bool biggerThan1000);
    bool isBiggerThan1000(QString index);
    bool validate(QString imgName, bool biggerThan1000);
    void compressPDF();
};

#endif // MYTHREAD_H

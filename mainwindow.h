#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <vector>
#include <QRegExpValidator>
#include <QRegExp>
#include <QIcon>
#include <vector>
#include <QMessageBox>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    unsigned char calcChecksum(unsigned char* arr, const int & numOfBytes);
    void qStrToUchar(QString str, unsigned char* arr, int *numOfBytes);
    void printUCharArr(unsigned char * arr, int numOfBytes);
    QString reverseHexQStr(const QString &hex);
    QString reverseBinQStr(QString bin);
    void binQStrToUCharArr(const QString& bin, unsigned char* arr, int* arrSize);
    void reverseUCharArr(unsigned char* arr, int arrSize);
    QString uCharArrToQStr(unsigned char* arr, int arrSize);
    QString hexQStrToBinQStr(const QString& hex);
    QString binQStrToHexQStr(const QString& bin);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();


    void on_radioButton_9_clicked();

    void on_radioButton_10_clicked();

    void on_radioButton_7_clicked();

    void on_radioButton_8_clicked();

    void on_pushButton_7_clicked();

private:
    Ui::MainWindow *ui;
    QRegExpValidator* hexValidator;
    QRegExpValidator* floatUnitValidator;
    double getFactorOfTen(QString f);
    QString stripFactorOfTen(QString& f);
    double computeUnitValue(QString v);
    void vDivTabError(QString s);
    double computeVDiv(double vin, double r1, double r2, double vout);
    void findRTotal(bool option);
    void findParallelRs(unsigned int numOfR);
    void initParallelRTab();
    void parRTabError(QString s);
    double calcParRtotal(double r1, double r2);
    double calcParRtotal(unsigned int count, double r1);
    QString setFactorOfTen(double n);
    QString getFOfTenUnit(int fOfTen);
    std::vector<double> calcParallelR(unsigned int numOfR, double rtotal, double rref);
    double calcParR(double rtotal, double r1);
    std::vector<double> calParallelR_A(double rtotal, double rref);
    void showParRResults(double rref, QString resultStr);
};

#endif // MAINWINDOW_H

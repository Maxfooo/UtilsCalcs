#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Utils & Calcs");
    //this->setWindowIcon(QIcon(":/images/faceforicon_2_bJ5_2.ico"));
    this->setWindowIcon(QIcon(":/images/bigmaxletters_3_AsP_2.ico"));
    hexValidator = new QRegExpValidator(QRegExp("^[0-9a-fA-F]{32}$"),this);
    floatUnitValidator = new QRegExpValidator(QRegExp("^[0-9]+[\\.]?[0-9]*[numkMG]?$"),this);
    ui->lineEdit->setValidator(hexValidator);
    ui->lineEdit_3->setValidator(hexValidator);
    ui->lineEdit_7->setValidator(hexValidator);

    ui->lineEdit_10->setValidator(floatUnitValidator);
    ui->lineEdit_11->setValidator(floatUnitValidator);
    ui->lineEdit_12->setValidator(floatUnitValidator);
    ui->lineEdit_13->setValidator(floatUnitValidator);
    ui->lineEdit_14->setValidator(floatUnitValidator);
    ui->lineEdit_15->setValidator(floatUnitValidator);
    ui->lineEdit_16->setValidator(floatUnitValidator);
    ui->lineEdit_18->setValidator(floatUnitValidator);

    initParallelRTab();
}

MainWindow::~MainWindow()
{
    delete ui;
}

/***********************************/
//
//  HEX CALCULATIONS TAB
//
/***********************************/


void MainWindow::on_pushButton_clicked() // Calculate Checksum/2's compliment
{
    unsigned char values[16];
    int numOfChars;
    unsigned char checksum = 0;
    QString temp = ui->lineEdit->text();

    if (ui->radioButton_5->isChecked())
    {
        qStrToUchar(temp, values, &numOfChars);
    }
    else
    {
        qStrToUchar(binQStrToHexQStr(temp), values, &numOfChars);
    }

    checksum = calcChecksum(values, numOfChars);
    ui->lineEdit_2->setText("0x" + QString::number(checksum, 16));
    ui->lineEdit_9->setText("0b" + QString::number(checksum, 2));
}

unsigned char MainWindow::calcChecksum(unsigned char* arr, const int & numOfBytes)
{
    unsigned char checksum = 0;
    for (int i=0;i<numOfBytes;i++)
    {
        checksum += arr[i];
    }
    checksum = 0xFF - checksum + 1; // calc two's compliment
    return checksum;
}

void MainWindow::qStrToUchar(QString str, unsigned char* arr, int * numOfBytes)
{
    int strSize = str.length();
    if (strSize < 1)
    {
        return;
    }
    QString temp = "";
    int temp1 = 0;
    int tempNumOfBytes = 0;
    bool bStatus = false;

    if (strSize % 2 != 0)
    {
        str.prepend('0');
        strSize++;
    }

    for (int i=0; i < strSize; i++)
    {
        if (temp1 < 2)
        {
            temp.append(str[i]);
            temp1++;
        }
        else
        {
            arr[tempNumOfBytes++] = temp.toUInt(&bStatus, 16);
            temp = str[i];
            temp1 = 1;
        }
    }
    arr[tempNumOfBytes++] = temp.toUInt(&bStatus, 16);
    *numOfBytes = tempNumOfBytes;
}

void MainWindow::printUCharArr(unsigned char * arr, int numOfBytes)
{
    for (int i=0; i < numOfBytes;i++)
    {
        qDebug() << QString::number(arr[i]);
    }
}

void MainWindow::on_pushButton_2_clicked() // To hex conversion button
{
    bool status = false;
    QString temp = ui->lineEdit_3->text();
    int base = ui->checkBox->isChecked() ? 2 : 10;
    ui->lineEdit_4->setText("0x" + QString::number(temp.toUInt(&status, base), 16));
}

void MainWindow::on_pushButton_3_clicked() // To decimal conversion button
{
    bool status = false;
    QString temp = ui->lineEdit_3->text();
    int base = ui->checkBox->isChecked() ? 2 : 16;
    ui->lineEdit_5->setText(QString::number(temp.toUInt(&status, base)));
}

void MainWindow::on_pushButton_4_clicked() // To binary conversion button
{
    bool status = false;
    QString temp = ui->lineEdit_3->text();
    if (ui->checkBox->isChecked())
    {
        ui->lineEdit_6->setText(temp);
        return;
    }
    QString temp1 = "";
    int base = ui->radioButton->isChecked() ? 16 : 10;
    ui->lineEdit_6->setText("0b" + temp1.setNum(temp.toUInt(&status, base), 2));
}

void MainWindow::on_pushButton_5_clicked() // Reverse Byte(s) button
{
    QString temp = ui->lineEdit_7->text();
    // radioButton_3 == hex
    // radioButton_4 == Binary
    QString temp1 = ui->radioButton_3->isChecked() ? reverseHexQStr(temp) : reverseBinQStr(temp);
    ui->lineEdit_8->setText(temp1);
}

QString MainWindow::reverseBinQStr(QString bin)
{
    QString temp = "";
    int temp1 = bin.length()-1;
    for (int i = temp1; i >=  0; i--)
    {
        temp.append(bin[i]);
    }
    return temp;
}

QString MainWindow::reverseHexQStr(const QString &hex)
{
    QString reverseHex = "";
    // turn each hex digit into a binary string starting from the back of the hex string
    for (int i=hex.length()-1; i >= 0; i--)
    {
        // reverse the binary string
        // convert it back to a hex digit
        // append it to a temp string var
        reverseHex.append(binQStrToHexQStr(reverseBinQStr(hexQStrToBinQStr(hex[i]))));
    }

    // return string
    return reverseHex;
}

QString MainWindow::hexQStrToBinQStr(const QString& hex)
{
    bool status = false;
    unsigned int temp1 = hex.toUInt(&status, 16);
    QString temp = QString::number(temp1, 2);

    // need to preserve 4 bits per nibble (which a hex digit is)
    if (temp.length() < 4)
    {
        for (int i = 0; i < 4-temp.length(); i++)
        {
            temp.prepend("0");
        }
    }
    return temp;
}

QString MainWindow::binQStrToHexQStr(const QString& bin)
{
    bool status = false;
    unsigned int temp = bin.toUInt(&status, 2);
    return QString::number(temp, 16);
}

void MainWindow::binQStrToUCharArr(const QString& bin, unsigned char* arr, int* arrSize)
{
    if (bin.length() > *arrSize)
    {
        return;
    }
    int i = 0;
    for (i; i < bin.length(); i++)
    {
        arr[i] = bin[i].digitValue();
    }
    *arrSize = i;
}

void MainWindow::reverseUCharArr(unsigned char* arr, int arrSize)
{
    unsigned char temp;
    for (int i = 0; i < arrSize/2; i++)
    {
        temp = arr[i];
        arr[i] = arr[arrSize-1-i];
        arr[arrSize-1-i] = temp;
        qDebug() << QString::number(arr[i]);
    }
}

QString MainWindow::uCharArrToQStr(unsigned char* arr, int arrSize)
{
    QString temp = "";
    for (int i = 0; i < arrSize; i++)
    {
        temp.append(QString::number(arr[i], 16));
    }
    return temp;
}

/***********************************/
//
//  VOLTAGE DIVIDER TAB
//
/***********************************/

void MainWindow::on_pushButton_6_clicked()
{
    QLineEdit* resultLE;
    bool status = false;
    double vin = 0;
    double r1 = 0;
    double r2 = 0;
    double vout = 0;

    QString temp = ui->lineEdit_10->text(); // Vin
    if (temp.isEmpty())
    {
        vin = -1;
        resultLE = ui->lineEdit_10;
    }
    else
    {
        vin = computeUnitValue(temp);
        if (vin == -1) { vDivTabError("Vin"); return;}
    }

    temp = ui->lineEdit_11->text();
    if (temp.isEmpty())
    {
        if (vin == -1) { vDivTabError("R1"); return;}
        r1 = -1;
        resultLE = ui->lineEdit_11;
    }
    else
    {
        r1 = computeUnitValue(temp);
        if (r1 == -1) { vDivTabError("R1"); return;}
    }

    temp = ui->lineEdit_12->text();
    if (temp.isEmpty())
    {
        if (r1 == -1) { vDivTabError("R2"); return;}
        r2 = -1;
        resultLE = ui->lineEdit_12;
    }
    else
    {
        r2 = computeUnitValue(temp);
        if (r2 == -1) { vDivTabError("R2"); return;}
    }

    temp = ui->lineEdit_13->text();
    if (temp.isEmpty())
    {
        if (r2 == -1) { vDivTabError("Vout"); return;}
        vout = -1;
        resultLE = ui->lineEdit_13;
    }
    else
    {
        vout = computeUnitValue(temp);
        if (vout == -1) { vDivTabError("Vout"); return;}
    }

    resultLE->setText(QString::number(computeVDiv(vin, r1, r2, vout)));
}

void MainWindow::vDivTabError(QString s)
{
    ui->label_17->setText(s);
}


/***********************************/
//
//  PARALLEL RESISTORS TAB
//
/***********************************/

void MainWindow::initParallelRTab()
{
    this->on_radioButton_9_clicked();
    this->on_radioButton_7_clicked();
}

void MainWindow::on_radioButton_9_clicked() // Find total R
{
    ui->lineEdit_18->setEnabled(false);
    ui->groupBox_7->setEnabled(true);
    ui->radioButton_11->setEnabled(false);
    ui->radioButton_12->setEnabled(false);
    ui->lineEdit_15->setEnabled(true);
    ui->lineEdit_16->setEnabled(true);
    if (ui->radioButton_7->isChecked())
    {
        this->on_radioButton_7_clicked();
    }
    else
    {
        this->on_radioButton_8_clicked();
    }
}

void MainWindow::on_radioButton_10_clicked() // Find Parallel R's
{
    ui->lineEdit_18->setEnabled(true);
    ui->lineEdit_17->setEnabled(false);
    ui->groupBox_7->setEnabled(false);
    ui->radioButton_11->setEnabled(true);
    ui->radioButton_12->setEnabled(true);
    ui->lineEdit_15->setEnabled(false);
    ui->lineEdit_16->setEnabled(false);
}

void MainWindow::on_radioButton_7_clicked() // 2-3 Resistors
{
    ui->lineEdit_15->setEnabled(true);
    ui->lineEdit_16->setEnabled(true);
    ui->lineEdit_17->setEnabled(false);
}

void MainWindow::on_radioButton_8_clicked() // n Resistors
{
    ui->lineEdit_17->setEnabled(true);
    ui->lineEdit_15->setEnabled(false);
    ui->lineEdit_16->setEnabled(false);
}

void MainWindow::on_pushButton_7_clicked() // Calculate Parallel Tab
{
    unsigned int numOfR = ui->radioButton_11->isChecked() ? 2 : 3;
    if (ui->radioButton_9->isChecked())
    {
        findRTotal(ui->radioButton_7->isChecked());
    }
    else
    {
        findParallelRs(numOfR);
    }
}

void MainWindow::findRTotal(bool option)
{
    bool status = false;
    double r1 = 0;
    double r2 = 0;
    double r3 = 0;
    unsigned int numOfR = 0;

    QString temp = ui->lineEdit_14->text(); // r1


    if (option) // 2-3 Arbitrarily valued resistors
    {
        if (temp.isEmpty()) // r1
        {
            r1 = -1;
        }
        else
        {
            r1 = computeUnitValue(temp);
            if (r1 == -1) { parRTabError("R1"); return;}
        }

        temp = ui->lineEdit_15->text();
        if (temp.isEmpty())
        {
            if (r1 == -1) { parRTabError("R2"); return;}
            r2 = -1;
        }
        else
        {
            r2 = computeUnitValue(temp);
            if (r2 == -1) { parRTabError("R2"); return;}
        }

        temp = ui->lineEdit_16->text();
        if (temp.isEmpty())
        {
            if (r2 == -1) { parRTabError("R3"); return;}
            r3 = -1;
        }
        else
        {
            r3 = computeUnitValue(temp);
            if (r3 == -1) { parRTabError("R3"); return;}
        }

        if (r1 == -1)
        {
            ui->lineEdit_18->setText(setFactorOfTen(calcParRtotal(r2, r3)));
        }
        else if (r2 == -1)
        {
            ui->lineEdit_18->setText(setFactorOfTen(calcParRtotal(r1, r3)));
        }
        else if (r3 == -1)
        {
            ui->lineEdit_18->setText(setFactorOfTen(calcParRtotal(r1, r2)));
        }
        else
        {
            ui->lineEdit_18->setText(setFactorOfTen(calcParRtotal(r1, calcParRtotal(r2, r3))));
        }
    }
    else
    {
        if (temp.isEmpty()) // r1
        {
            parRTabError("R1");
            return;
        }
        else
        {
            r1 = computeUnitValue(temp);
            if (r1 == -1) { parRTabError("R1"); return;}
        }

        temp = ui->lineEdit_17->text(); // # of resistors
        if (temp.isEmpty())
        {
            parRTabError("# of Resistors");
            return;
        }
        else
        {
            numOfR = temp.toUInt(&status);
            if (!status) { parRTabError("# of Resistors"); return;}
            if (numOfR < 1) { parRTabError("# of Resistors"); return;}
        }
        ui->lineEdit_18->setText(setFactorOfTen(calcParRtotal(numOfR, r1)));
    }
}

void MainWindow::findParallelRs(unsigned int numOfR)
{
    double r1;
    double rtotal;
    std::vector<double> results;
    QString resultStr = "";

    QString temp = ui->lineEdit_14->text();
    if (temp.isEmpty()) // r1
    {
        r1 = 0;
    }
    else
    {
        r1 = computeUnitValue(temp);
        if (r1 == -1) { parRTabError("R1"); return;}
    }

    temp = ui->lineEdit_18->text();
    if (temp.isEmpty()) // rtotal
    {
        parRTabError("Rtotal");
        return;
    }
    else
    {
        rtotal = computeUnitValue(temp);
        if (rtotal == -1) { parRTabError("Rtotal"); return;}
        if (r1 > 0 && r1 < rtotal) { parRTabError("R1 too small.");} return;
        r1 = r1 == 0 ? 2*rtotal : r1;
    }
    qDebug() << "Hi";
    results = calcParallelR(numOfR, rtotal, r1);

    int newLineStart = 0;
    for (int i=0; i < results.size(); i+=numOfR)
    {
        if (numOfR == 2)
        {
            resultStr += QString("(%1, %2) ").arg(results.at(i)).arg(results.at(i+1));
        }
        else
        {
            resultStr += QString("(%1, %2, %3) ").arg(results.at(i)).arg(results.at(i+1)).arg(results.at(i+2));
        }

        if (newLineStart == 4)
        {
            resultStr += "\n";
            newLineStart = 0;
        }
        newLineStart++;
    }

    showParRResults(r1, resultStr);

}

void MainWindow::parRTabError(QString s)
{
    ui->label_19->setText(s);
}

/***********************************/
//
//  GENERAL UTILITIES
//
/***********************************/

double MainWindow::getFactorOfTen(QString f)
{
    if (f == "n") { return (1/1000000000);}
    if (f == "u") { return (1/1000000);}
    if (f == "m") { return (1/1000);}
    if (f == "k") { return (1000);}
    if (f == "M") { return (1000000);}
    if (f == "G") { return (1000000000);}
    return 1;

}

QString MainWindow::setFactorOfTen(double n)
{
    if (n >= 1)
    {
        if (n / 1000 >= 1)
        {
            if (n / 1000000 >= 1)
            {
                if (n / 1000000000 >= 1)
                {
                    return (QString::number(n/1000000000) + "G");
                }
                return (QString::number(n/1000000) + "M");
            }
            return (QString::number(n/1000) + "k");
        }
        return QString::number(n);
    }
    else
    {
        if (n * 1000 < 1)
        {
            if (n * 1000000 < 1)
            {
                if (n * 1000000000 < 1)
                {
                    return (QString::number(n*1000000000) + "n");
                }
                return (QString::number(n*1000000) + "u");
            }
            return (QString::number(n*1000) + "m");
        }
        return (QString::number(n));
    }
}

//QString MainWindow::setFactorOfTen(double n)
//{
//    int i = 1;
//    QString result = "";
//    if (n >= 1)
//    {
//        for (i = 1; i < 4; i++)
//        {
//            if (n / (std::pow(1000, i)) < 1)
//            {
//                i--;
//                result = QString::number((n / (std::pow(1000, i)));
//                break;
//            }
//        }
//        return (result + getFOfTenUnit(i*3));
//    }
//    else
//    {
//        for (i = -1; i > -4; i--)
//        {
//            if (n * std::pow(1000, i) > 1)
//            {
//                i++;
//                result = QString::number( (n*std::pow(1000, i)));
//                break;
//            }
//        }
//        return (result + getFOfTenUnit(i*3));
//    }
//}

QString MainWindow::getFOfTenUnit(int fOfTen)
{
    switch (fOfTen)
    {
    case -9: return "n";

    case -6: return "u";

    case -3: return "m";

    case 0: return "";

    case 3: return "k";

    case 6: return "M";

    case 9: return "G";

    default: return "";
    }
}

QString MainWindow::stripFactorOfTen(QString& f)
{
    if (f.contains("n"))
    {
        f.replace("n", "");
        return "n";
    }
    else if (f.contains("u"))
    {
        f.replace("u", "");
        return "u";
    }
    else if (f.contains("m"))
    {
        f.replace("m", "");
        return "m";
    }
    else if (f.contains("k"))
    {
        f.replace("k", "");
        return "k";
    }
    else if (f.contains("M"))
    {
        f.replace("M", "");
        return "M";
    }
    else if (f.contains("G"))
    {
        f.replace("G", "");
        return "G";
    }
    return "";
}

double MainWindow::computeUnitValue(QString v)
{
    bool status = false;
    double fOfTen = getFactorOfTen(stripFactorOfTen(v));
    double temp = v.toDouble(&status);
    if (!status) { return -1;}
    return (temp * fOfTen);
}

double MainWindow::computeVDiv(double vin, double r1, double r2, double vout)
{
    // if *var* < 0; compute that *var*

    if (vin < 0)
    {
        return ( (r1+r2)/r2 * vout );
    }
    else if (r1 < 0)
    {
        return ( r2 * (vin/vout - 1) );
    }
    else if (r2 < 0)
    {
        return ( r1 / (vin/vout - 1) );
    }
    else
    {
        return ( vin * r2/(r1+r2) );
    }
}

double MainWindow::calcParRtotal(double r1, double r2)
{
    return ( 1 / (1/r1 + 1/r2) );
}

double MainWindow::calcParRtotal(unsigned int count, double r1)
{
    double rtotal = r1;
    if (count <= 1)
    {
        return r1;
    }
    for (int i = 0; i < count-1; i++)
    {
        rtotal = calcParRtotal(rtotal, r1);
    }
    return rtotal;
}

std::vector<double> MainWindow::calcParallelR(unsigned int numOfR, double rtotal, double rref)
{
    std::vector<double> results;
    double rref_2 = 0;

    // compute 30 (900) results
    // Set 1) 10 with rref == [2*rtotal,] 3*rtotal, ..., 12*rtotal [,13*rtotal]
    // Set 2) 10 with rref == 20*rtotal, 30*rtotal, ..., 100*rtotal
    // Set 3) 10 with rref == 200*rtotal, 300*rtotal, ..., 1000*rtotal
    // Variability with Set 1) if numOfR is 3, must start with 3*rtotal
    // If numOfR is 3, there should be 900 results, where each rref has above for
    // r2 and r3, where r2 becomes rref_2

    if (numOfR == 2)
    {
        results = calParallelR_A(rtotal, rref);
    }
    else
    {

    }

    return results;
}

std::vector<double> MainWindow::calParallelR_A(double rtotal, double rref)
{
    std::vector<double> results;
    int j = 1;
    qDebug() << "Hello";
    for (int i = 1; i < 31; i++)
    {

        if (i > 20)
        {
            j = 100;
        }
        else if (i > 10)
        {
            j = 10;
        }
        rref*i*j;
        results.push_back(calcParR(rtotal, rref));
    }
    return results;
}

double MainWindow::calcParR(double rtotal, double r1)
{
    return ( 1 / (1/rtotal - 1/r1));
}

void MainWindow::showParRResults(double rref, QString resultStr)
{
    QMessageBox msgBox;
    msgBox.setText(QString("Reference Resistor (R1): %1\n\n%2").arg(QString::number(rref)).arg(resultStr));
    msgBox.exec();
}

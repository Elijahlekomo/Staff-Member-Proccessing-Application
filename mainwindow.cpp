#include "mainwindow.h"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QRegularExpression>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), filePath("")
{

    centralWidget = new QWidget(this);
    layout = new QVBoxLayout(centralWidget);

    textEdit = new QTextEdit(this);
    layout->addWidget(textEdit);

    openFileButton = new QPushButton("Open File", this);
    connect(openFileButton, &QPushButton::clicked, this, &MainWindow::on_openFileButton_clicked);
    layout->addWidget(openFileButton);

    checkFileButton = new QPushButton("Check File", this);
    connect(checkFileButton, &QPushButton::clicked, this, &MainWindow::on_checkFileButton_clicked);
    layout->addWidget(checkFileButton);

    setCentralWidget(centralWidget);
}

MainWindow::~MainWindow()
{

}

void MainWindow::on_openFileButton_clicked()
{
    filePath = QFileDialog::getOpenFileName(this, "Open Text File", "", "Text Files (*.txt);;All Files (*)");
    if (!filePath.isEmpty()) {
        textEdit->append("File loaded: " + filePath);
    }
}

void MainWindow::on_checkFileButton_clicked()
{
    if (!filePath.isEmpty()) {
        checkFileForProblems(filePath);
    } else {
        QMessageBox::warning(this, "Error", "Please load a file first.");
    }
}

void MainWindow::checkFileForProblems(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Cannot open file for reading.");
        return;
    }

    QTextStream in(&file);
    int lineNumber = 1;

    QRegularExpression numberInWord("\\w*\\d+\\w*");
    QRegularExpression capitalInWord("\\w+[A-Z]+\\w*");
    QRegularExpression specialCharInWord("[\\w]*[\\^#@!&*(){}\\[\\];:'\"\\/?><,|=+~`]+[\\w]*");

    textEdit->append("Checking file for problems...\n");

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList words = line.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);

        for (int wordNumber = 0; wordNumber < words.size(); ++wordNumber) {
            const QString &word = words[wordNumber];

            if (numberInWord.match(word).hasMatch() ||
                capitalInWord.match(word).hasMatch() ||
                specialCharInWord.match(word).hasMatch()) {

                textEdit->append(QString("Problematic word: \"%1\" found at Line %2, Word %3")
                                     .arg(word)
                                     .arg(lineNumber)
                                     .arg(wordNumber + 1));
            }
        }
        lineNumber++;
    }

    textEdit->append("Check completed.\n");
    file.close();
}

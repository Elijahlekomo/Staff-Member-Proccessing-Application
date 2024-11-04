#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_openFileButton_clicked();
    void on_checkFileButton_clicked();

private:
    QString filePath;
    QTextEdit *textEdit;
    QPushButton *openFileButton;
    QPushButton *checkFileButton;
    QWidget *centralWidget;
    QVBoxLayout *layout;

    void checkFileForProblems(const QString &filePath);
};

#endif

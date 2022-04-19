#pragma once

#include "shared/thread/thread_utils.h"
#include "database/mssql_driver.h"

#include <QMainWindow>
#include <QtSql>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool init();
    void deinit();

    void saveGeometry();
    void loadGeometry();

private slots:

    void on_btnCopyData_clicked();
    void on_btnSelect_clicked();

private:
    Ui::MainWindow *ui;

    db::mssql::Driver::Ptr _connect;
    db::mssql::Transaction::Ptr _transact1;
    db::mssql::Transaction::Ptr _transact2;

    trd::ThreadIdList _threadIds;
};

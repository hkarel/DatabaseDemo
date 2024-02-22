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
    void on_btnConnect_clicked(bool);
    void on_btnDisconnect_clicked(bool);

    void on_btnBeginTransact1_clicked(bool);
    void on_btnCommitTransact1_clicked(bool);
    void on_btnRollbackTransact1_clicked(bool);

    void on_btnBeginTransact2_clicked(bool);
    void on_btnCommitTransact2_clicked(bool);
    void on_btnRollbackTransact2_clicked(bool);

    void on_btnSelect1_clicked(bool);
    void on_btnSelect2_clicked(bool);
    void on_btnSelect3_clicked(bool);
    void on_btnInsAndSelect4_clicked(bool);

    void on_btnUpdate1_clicked(bool);
    void on_btnUpdate2_clicked(bool);
    void on_btnUpdate3_clicked(bool);
    void on_btnUpdate4_clicked(bool);

    void on_btnInsert1_clicked(bool);
    void on_btnInsert2_clicked(bool);
    void on_btnInsert3_clicked(bool);
    void on_btnInsert4_clicked(bool);

    void on_btnDeleteAll_clicked(bool);
    void on_btnAbortQuery_clicked(bool);

private:
    Ui::MainWindow *ui;

    db::mssql::Driver::Ptr _connect;
    db::mssql::Transaction::Ptr _transact1;
    db::mssql::Transaction::Ptr _transact2;

    //QSqlQuery _selectQuery;
    QSqlQueryModel _queryModel;

    trd::ThreadIdList _threadIds;
};

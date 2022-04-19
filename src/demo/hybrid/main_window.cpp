#include "main_window.h"
#include "ui_main_window.h"

#include "shared/steady_timer.h"
#include "shared/logger/logger.h"
#include "shared/logger/format.h"
#include "shared/logger/config.h"
#include "shared/config/appl_conf.h"
#include "shared/config/logger_conf.h"
#include "shared/qt/quuidex.h"
#include "shared/qt/logger_operators.h"

#include "../enum_test.h"
#include "database/sql_func.h"
#include "database/mssql_pool.h"
#include "database/postgres_pool.h"

#include <unistd.h>
#include <thread>

using namespace db::mssql;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //view->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::init()
{
    return true;
}

void MainWindow::deinit()
{
    _transact1.reset();
    _transact2.reset();

    _threadIds.lock([](std::vector<pid_t>& tids) {
        for (pid_t tid : tids)
            mspool().abortOperation(tid);
    });

    while (!_threadIds.empty())
        usleep(100*1000);
}

void MainWindow::saveGeometry()
{
    QPoint p = pos();
    QVector<int> v {p.x(), p.y(), width(), height()};
    config::state().setValue("windows.main_window.geometry", v);
}

void MainWindow::loadGeometry()
{
    QVector<int> v {0, 0, 800, 600};
    config::state().getValue("windows.main_window.geometry", v);
    move(v[0], v[1]);
    resize(v[2], v[3]);
}

void MainWindow::on_btnCopyData_clicked()
{
    db::postgres::Driver::Ptr dbconDst = pgpool().connect();
    QSqlQuery qDst {dbconDst->createResult()};

    qint64 maxTimeline = 0;

    if (!qDst.prepare("select max(timeline) as timeline  from motor_table"))
        return;

    if (!qDst.exec())
        return;

    if (!qDst.first())
        return;

    sql::assignValue(maxTimeline, qDst.record(), "timeline");

    db::mssql::Driver::Ptr dbconSrc = mspool().connect();
    QSqlQuery qSrc {dbconSrc->createResult()};

    qSrc.setForwardOnly(false);

    QString querySrc = "select timeline, name, state from motor_table where timeline > :TIMELINE";

    if (!qSrc.prepare(querySrc))
        return;

    sql::bindValue(qSrc, ":TIMELINE", maxTimeline);

    if (!qSrc.exec())
        return;

    QString queryDst = "insert into motor_table(timeline, name) VALUES (:TIMELINE, :NAME)";

    if (!qDst.prepare(queryDst))
        return;

    while (qSrc.next())
    {
        QSqlRecord r = qSrc.record();

        qint64 timeline;
        QString name;

        sql::assignValue(timeline, r, "timeline");
        sql::assignValue(name, r, "name");

        sql::bindValue(qDst, ":TIMELINE", timeline);
        sql::bindValue(qDst, ":NAME", name);

        if (!qDst.exec())
        {
            log_error << "Error on copy record";
        }
    }
}

void MainWindow::on_btnSelectData_clicked()
{
    db::mssql::Driver::Ptr dbconSrc = mspool().connect();
    QSqlQuery qSrc {dbconSrc->createResult()};

    qSrc.setForwardOnly(false);

    QString querySrc = "select timeline, name, state from motor_table";

    if (!qSrc.prepare(querySrc))
        return;

    if (!qSrc.exec())
        return;

    while (qSrc.next())
    {
        QSqlRecord r = qSrc.record();

        qint64 timeline;
        QString name;
        bool state;

        sql::assignValue(timeline, r, "timeline");
        sql::assignValue(name, r, "name");
        sql::assignValue(state, r, "state");
    }
}

void MainWindow::on_btnInsertData_clicked()
{
    db::mssql::Driver::Ptr dbconSrc = mspool().connect();
    QSqlQuery qSrc {dbconSrc->createResult()};

    qSrc.setForwardOnly(false);

    QString querySrc =
        " INSERT INTO [motor_table]          "
        "   ( timeline, name, state )  "
        " VALUES                             "
        "   (:TIMELINE, :NAME, :STATE) ";

    if (!qSrc.prepare(querySrc))
        return;

    qint64 timeline = QDateTime::currentMSecsSinceEpoch();
    QString name = "name";
    bool state = true;

    sql::bindValue(qSrc, ":TIMELINE", timeline);
    sql::bindValue(qSrc, ":NAME", name);
    sql::bindValue(qSrc, ":STATE", state);

    if (!qSrc.exec())
        return;
}

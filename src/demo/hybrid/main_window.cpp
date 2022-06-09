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

    _threadIds.lock([](const std::vector<pid_t>& tids) {
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

void MainWindow::on_btnInsertData1_clicked()
{
    db::mssql::Driver::Ptr dbconSrc = mspool().connect();
    QSqlQuery qSrc {dbconSrc->createResult()};

    qSrc.setForwardOnly(false);

    // Если параметр не указывать, то в БД значение этого столбца будет NULL
    QString querySrc =
        " insert into motor_table "
        "   ( timeline, state )   "
        " values                  "
        "   (:TIMELINE, :STATE)   ";

    if (!qSrc.prepare(querySrc))
        return;

    qint64 timeline = QDateTime::currentMSecsSinceEpoch();
    bool state = true;

    sql::bindValue(qSrc, ":TIMELINE", timeline);
    sql::bindValue(qSrc, ":STATE", state);

    if (!qSrc.exec())
        return;
}

void MainWindow::on_btnInsertData2_clicked()
{
    db::mssql::Driver::Ptr dbconSrc = mspool().connect();
    QSqlQuery qSrc {dbconSrc->createResult()};

    qSrc.setForwardOnly(false);

    QString querySrc =
        " insert into [motor_table]    "
        "   ( timeline, name, state )  "
        " values                       "
        "   (:TIMELINE, :NAME, :STATE) ";

    if (!qSrc.prepare(querySrc))
        return;

    qint64 timeline = QDateTime::currentMSecsSinceEpoch();
    bool state = true;

    sql::bindValue(qSrc, ":TIMELINE", timeline);
    sql::bindValue(qSrc, ":NAME", QVariant());
    sql::bindValue(qSrc, ":STATE", state);

    if (!qSrc.exec())
        return;
}

void MainWindow::on_btnInsertData3_clicked()
{
    db::mssql::Driver::Ptr dbconSrc = mspool().connect();
    QSqlQuery qSrc {dbconSrc->createResult()};

    qSrc.setForwardOnly(false);

    QString querySrc =
        " insert into [motor_table]    "
        "   ( timeline, name, state )  "
        " values                       "
        "   (:TIMELINE, :NAME, :STATE) ";

    if (!qSrc.prepare(querySrc))
        return;

    qint64 timeline = QDateTime::currentMSecsSinceEpoch();
    bool state = true;

    sql::bindValue(qSrc, ":TIMELINE", timeline);
    sql::bindValue(qSrc, ":STATE", state);

    if (!qSrc.exec())
        return;
}

void MainWindow::on_btnInsertData4_clicked()
{
    db::mssql::Driver::Ptr dbconSrc = mspool().connect();
    QSqlQuery qSrc {dbconSrc->createResult()};

    qSrc.setForwardOnly(false);

    QString querySrc =
        " insert into [motor_table]    "
        "   ( timeline, name, state )  "
        " values                       "
        "   (:TIMELINE, :NAME, :STATE) ";

    if (!qSrc.prepare(querySrc))
        return;

    qint64 timeline = QDateTime::currentMSecsSinceEpoch();
    bool state = true;

    sql::bindValue(qSrc, ":TIMELINE", timeline);
    sql::bindValue(qSrc, ":NAME", QVariant().toString());
    sql::bindValue(qSrc, ":STATE", state);

    if (!qSrc.exec())
        return;
}

void MainWindow::on_btnInsertData5_clicked()
{
    db::mssql::Driver::Ptr dbconSrc = mspool().connect();
    QSqlQuery qSrc {dbconSrc->createResult()};

    qSrc.setForwardOnly(false);

    QString querySrc =
        " insert into [motor_table]    "
        "   ( timeline, name, state )  "
        " values                       "
        "   (:TIMELINE, :NAME, :STATE) ";

    if (!qSrc.prepare(querySrc))
        return;

    qint64 timeline = QDateTime::currentMSecsSinceEpoch();
    bool state = true;

    sql::bindValue(qSrc, ":TIMELINE", timeline);
    sql::bindValue(qSrc, ":NAME", QVariant(QVariant::String));
    sql::bindValue(qSrc, ":STATE", state);

    if (!qSrc.exec())
        return;
}

void MainWindow::on_btnMoveData1_clicked()
{
    db::postgres::Driver::Ptr dbconSrc = pgpool().connect();
    QSqlQuery qSrc {dbconSrc->createResult()};

    db::mssql::Driver::Ptr dbconDst = mspool().connect();
    QSqlQuery qDst {dbconDst->createResult()};

    qSrc.setForwardOnly(false);

    QString querySrc = "select rec_id, dtime_zakl from engine_on_line_tmp";

    if (!qSrc.prepare(querySrc))
        return;

    if (!qSrc.exec())
        return;

    QString queryDst =
        " insert into engine_on_line_tmp2                                   "
        "   ( rec_id, num_dv, ods_id4, odsb_dv, odsb_sm, dtime_zakl )      "
        " values                                                           "
        "   ( :rec_id, :num_dv, :ods_id4, :odsb_dv, :odsb_sm, :dtime_zakl) ";

    if (!qDst.prepare(queryDst))
        return;

    qint32 i = 0;
    while (qSrc.next())
    {
        QSqlRecord r = qSrc.record();

        qint64 rec_id = 1;
        QString num_dv = "num_dv";
        qint64 ods_id4 = 2;
        QString odsb_dv = "odsb_dv";
        QString odsb_sm = "odsb_sm";
        QDateTime dtime_zakl;

        sql::assignValue(rec_id, r, "rec_id");
        sql::assignValue(dtime_zakl, r, "dtime_zakl");

        sql::bindValue(qDst, ":rec_id", QDateTime::currentSecsSinceEpoch() + i);
        sql::bindValue(qDst, ":num_dv", num_dv);
        sql::bindValue(qDst, ":ods_id4", ods_id4);
        sql::bindValue(qDst, ":odsb_dv", odsb_dv);
        sql::bindValue(qDst, ":odsb_sm", odsb_sm);
        sql::bindValue(qDst, ":dtime_zakl", dtime_zakl);

        qDst.exec();

        i++;
    }
}

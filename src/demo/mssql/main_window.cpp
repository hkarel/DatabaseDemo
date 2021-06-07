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

#include <unistd.h>
#include <thread>

using namespace db::mssql;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->tableView->setModel(&_queryModel);
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

void MainWindow::on_btnConnect_clicked(bool)
{
    log_debug << "Call db::mssql::pool().connect()";
    _connect = db::mssql::pool().connect();
}

void MainWindow::on_btnDisconnect_clicked(bool)
{
    if (_connect)
    {
        _connect->close();
        _connect.reset();
    }
}

void MainWindow::on_btnBeginTransact1_clicked(bool)
{
    _transact1.reset();

    db::mssql::Driver::Ptr dbcon = mspool().connect();
    _transact1 = dbcon->createTransact();
    _transact1->begin();
}

void MainWindow::on_btnCommitTransact1_clicked(bool)
{
    if (_transact1)
        _transact1->commit();
}

void MainWindow::on_btnRollbackTransact1_clicked(bool)
{
    if (_transact1)
        _transact1->rollback();
}

void MainWindow::on_btnBeginTransact2_clicked(bool)
{
    _transact2.reset();

    db::mssql::Driver::Ptr dbcon = mspool().connect();
    _transact2 = dbcon->createTransact();
    _transact2->begin();
}

void MainWindow::on_btnCommitTransact2_clicked(bool)
{
    if (_transact2)
        _transact2->commit();
}

void MainWindow::on_btnRollbackTransact2_clicked(bool)
{
    if (_transact2)
        _transact2->rollback();
}

void MainWindow::on_btnSelect1_clicked(bool)
{
    _queryModel.setQuery(QSqlQuery());

    db::mssql::Driver::Ptr dbcon = mspool().connect();
    QSqlQuery q {dbcon->createResult()};

    q.setForwardOnly(false);

    if (!q.prepare("SELECT * FROM TABLE1"))
        return;

    if (q.exec())
    {
        log_info << "--- Select-query 1 exec success ---";
    }

    log_info << "Select-query 1 records count: "
             << db::mssql::resultSize(q, mspool().connect());

    _queryModel.setQuery(q);

    log_info << "NumRowsAffected: " << q.numRowsAffected();
}

void MainWindow::on_btnSelect2_clicked(bool)
{
    _queryModel.setQuery(QSqlQuery());
    //_queryModel.query().clear();

    db::mssql::Driver::Ptr dbcon = mspool().connect();
    QSqlQuery q {dbcon->createResult()};

    q.setForwardOnly(false);

    QString query =
        " SELECT TOP %1       "
        "  [F_GUID]           "
        " ,[F_BIGINT]         "
        " ,[F_BINARY]         "
        " ,[F_BIT]            "
        " ,[F_CHAR]           "
        " ,[F_DATE]           "
        " ,[F_DATETIME]       "
        " ,[F_DATETIME2]      "
        " ,[F_DATETIMEOFFSET] "
        " ,[F_DECIMAL]        "
        " ,[F_FLOAT]          "
        " ,[F_INT]            "
        " ,[F_NCHAR]          "
        " ,[F_NUMERIC]        "
        " ,[F_NVARCHAR_N]     "
        " ,[F_NVARCHAR_MAX]   "
        " ,[F_REAL]           "
        " ,[F_SMALLDATETIME]  "
        " ,[F_SMALLINT]       "
        " ,[F_TIME]           "
        " ,[F_TINYINT]        "
        " ,[F_VARBINARY_N]    "
        " ,[F_VARBINARY_MAX]  "
        " ,[F_VARCHAR_N]      "
        " ,[F_VARCHAR_MAX]    "
        " FROM                "
        "   TABLE1 ";
    query = query.arg(10);

    if (sql::exec(q, query))
    {
        log_info << "--- Select-query 2 exec success ---";
    }

    log_info << "Select-query 2 records count: "
             << db::mssql::resultSize(q, mspool().connect());

    _queryModel.setQuery(q);

    log_info << "NumRowsAffected: " << q.numRowsAffected();
}

void MainWindow::on_btnSelect3_clicked(bool)
{
    _queryModel.setQuery(QSqlQuery());

    db::mssql::Driver::Ptr dbcon = mspool().connect();
    QSqlQuery q {dbcon->createResult()};

    q.setForwardOnly(false);

    QString query =
        " SELECT TOP %1 "
        "  [F_GUID]           "
        " ,[F_BIGINT]         "
        " ,[F_BINARY]         "
        " ,[F_BIT]            "
        " ,[F_CHAR]           "
        " ,[F_DATE]           "
        " ,[F_DATETIME]       "
        " ,[F_DATETIME2]      "
        " ,[F_DATETIMEOFFSET] "
        " ,[F_DECIMAL]        "
        " ,[F_FLOAT]          "
        " ,[F_INT]            "
        " ,[F_NCHAR]          "
        " ,[F_NUMERIC]        "
        " ,[F_NVARCHAR_N]     "
        " ,[F_NVARCHAR_MAX]   "
        " ,[F_REAL]           "
        " ,[F_SMALLDATETIME]  "
        " ,[F_SMALLINT]       "
        " ,[F_TIME]           "
        " ,[F_TINYINT]        "
        " ,[F_VARBINARY_N]    "
        " ,[F_VARBINARY_MAX]  "
        " ,[F_VARCHAR_N]      "
        " ,[F_VARCHAR_MAX]    "
        " FROM                "
        "   TABLE1            ";
    query = query.arg(5);

    if (sql::exec(q, query))
    {
        log_info << "--- Select-query 3 exec success ---";
    }

    while (q.next())
    {
        QSqlRecord r = q.record();

        QUuidEx    f_guid;
        qint64     f_bigint;
        QByteArray f_binary;
        bool       f_bit;
        char       f_char;
        QDate      f_date;
        QDateTime  f_datetime;
        QDateTime  f_datetime2;
        QDateTime  f_datetimeoffset;
        double     f_decimal;
        float      f_float;
        qint32     f_int;
        char       f_nchar;
        double     f_numeric;
        QString    f_nvarchar_n;
        QString    f_nvarchar_max;
        double     f_real;
        QDateTime  f_smalldatetime;
        qint16     f_smallint;
        QTime      f_time;
        qint8      f_tinyint;
        QByteArray f_varbinary_n;
        QByteArray f_varbinary_max;
        QString    f_varchar_n;
        QString    f_varchar_max;

        sql::assignValue(f_guid          , r, "  [F_GUID]           ");
        sql::assignValue(f_bigint        , r, "  [F_BIGINT]         ");
        sql::assignValue(f_binary        , r, " ,[F_BINARY]         ");
        sql::assignValue(f_bit           , r, " ,[F_BIT]            ");
        sql::assignValue(f_char          , r, " ,[F_CHAR]           ");
        sql::assignValue(f_date          , r, " ,[F_DATE]           ");
        sql::assignValue(f_datetime      , r, " ,[F_DATETIME]       ");
        sql::assignValue(f_datetime2     , r, " ,[F_DATETIME2]      ");
        sql::assignValue(f_datetimeoffset, r, " ,[F_DATETIMEOFFSET] ");
        sql::assignValue(f_decimal       , r, " ,[F_DECIMAL]        ");
        sql::assignValue(f_float         , r, " ,[F_FLOAT]          ");
        sql::assignValue(f_int           , r, " ,[F_INT]            ");
        sql::assignValue(f_nchar         , r, " ,[F_NCHAR]          ");
        sql::assignValue(f_numeric       , r, " ,[F_NUMERIC]        ");
        sql::assignValue(f_nvarchar_n    , r, " ,[F_NVARCHAR_N]     ");
        sql::assignValue(f_nvarchar_max  , r, " ,[F_NVARCHAR_MAX]   ");
        sql::assignValue(f_real          , r, " ,[F_REAL]           ");
        sql::assignValue(f_smalldatetime , r, " ,[F_SMALLDATETIME]  ");
        sql::assignValue(f_smallint      , r, " ,[F_SMALLINT]       ");
        sql::assignValue(f_time          , r, " ,[F_TIME]           ");
        sql::assignValue(f_tinyint       , r, " ,[F_TINYINT]        ");
        sql::assignValue(f_varbinary_n   , r, " ,[F_VARBINARY_N]    ");
        sql::assignValue(f_varbinary_max , r, " ,[F_VARBINARY_MAX]  ");
        sql::assignValue(f_varchar_n     , r, " ,[F_VARCHAR_N]      ");
        sql::assignValue(f_varchar_max   , r, " ,[F_VARCHAR_MAX]    ");

        log_debug << "[F_GUID]          " << f_guid          ;
        log_debug << "[F_BIGINT]        " << f_bigint        ;
        log_debug << "[F_BINARY]        " << f_binary        ;
        log_debug << "[F_BIT]           " << f_bit           ;
        log_debug << "[F_CHAR]          " << f_char          ;
        log_debug << "[F_DATE]          " << f_date          ;
        log_debug << "[F_DATETIME]      " << f_datetime      ;
        log_debug << "[F_DATETIME2]     " << f_datetime2     ;
        log_debug << "[F_DATETIMEOFFSET]" << f_datetimeoffset;
        log_debug << "[F_DECIMAL]       " << f_decimal       ;
        log_debug << "[F_FLOAT]         " << f_float         ;
        log_debug << "[F_INT]           " << f_int           ;
        log_debug << "[F_NCHAR]         " << f_nchar         ;
        log_debug << "[F_NUMERIC]       " << f_numeric       ;
        log_debug << "[F_NVARCHAR_N]    " << f_nvarchar_n    ;
        log_debug << "[F_NVARCHAR_MAX]  " << f_nvarchar_max  ;
        log_debug << "[F_REAL]          " << f_real          ;
        log_debug << "[F_SMALLDATETIME] " << f_smalldatetime ;
        log_debug << "[F_SMALLINT]      " << f_smallint      ;
        log_debug << "[F_TIME]          " << f_time          ;
        log_debug << "[F_TINYINT]       " << f_tinyint       ;
        log_debug << "[F_VARBINARY_N]   " << f_varbinary_n   ;
        log_debug << "[F_VARBINARY_MAX] " << f_varbinary_max ;
        log_debug << "[F_VARCHAR_N]     " << f_varchar_n     ;
        log_debug << "[F_VARCHAR_MAX]   " << f_varchar_max   ;

        log_debug << "-----------";
    }
}

void MainWindow::on_btnInsAndSelect4_clicked(bool)
{
    _queryModel.setQuery(QSqlQuery());
    QUuidEx id = QUuidEx::createUuid();

    db::mssql::Driver::Ptr dbcon = mspool().connect();
    QSqlQuery q {dbcon->createResult()};

    q.prepare("INSERT               "
              "  TABLE1             "
              " (                   "
              "  [F_GUID]           "
              " ,[F_BIGINT]         "
              " ,[F_BINARY]         "
              " ,[F_CHAR]           "
              " ,[F_DATE]           "
              " ,[F_DATETIME]       "
              " ,[F_DATETIME2]      "
              " ,[F_DATETIMEOFFSET] "
              " ,[F_FLOAT]          "
              " ,[F_INT]            "
              " ,[F_NCHAR]          "
              " ,[F_NVARCHAR_N]     "
              " ,[F_NVARCHAR_MAX]   "
              " ,[F_REAL]           "
              " ,[F_SMALLDATETIME]  "
              " ,[F_SMALLINT]       "
              " ,[F_TIME]           "
              " ,[F_TINYINT]        "
              " ,[F_VARBINARY_N]    "
              " ,[F_VARBINARY_MAX]  "
              " ,[F_VARCHAR_N]      "
              " ,[F_VARCHAR_MAX]    "
              " )                   "
              " VALUES              "
              " (                   "
              "  :F_GUID            "
              " ,:F_BIGINT          "
              " ,:F_BINARY          "
              " ,:F_CHAR            "
              " ,:F_DATE            "
              " ,:F_DATETIME        "
              " ,:F_DATETIME2       "
              " ,:F_DATETIMEOFFSET  "
              " ,:F_FLOAT           "
              " ,:F_INT             "
              " ,:F_NCHAR           "
              " ,:F_NVARCHAR_N      "
              " ,:F_NVARCHAR_MAX    "
              " ,:F_REAL            "
              " ,:F_SMALLDATETIME   "
              " ,:F_SMALLINT        "
              " ,:F_TIME            "
              " ,:F_TINYINT         "
              " ,:F_VARBINARY_N     "
              " ,:F_VARBINARY_MAX   "
              " ,:F_VARCHAR_N       "
              " ,:F_VARCHAR_MAX     "
              " );                  ");

    QDateTime dtime = QDateTime::currentDateTime();

    sql::bindValue(q, ":F_GUID            ", id);
    sql::bindValue(q, ":F_BIGINT          ", 1234 );
    sql::bindValue(q, ":F_BINARY          ", QByteArray("F_BINARY") );
    sql::bindValue(q, ":F_CHAR            ", "strfromqt"  );
    sql::bindValue(q, ":F_DATE            ", dtime );
    sql::bindValue(q, ":F_DATETIME        ", dtime );
    sql::bindValue(q, ":F_DATETIME2       ", dtime );
    sql::bindValue(q, ":F_DATETIMEOFFSET  ", dtime );
    sql::bindValue(q, ":F_FLOAT           ", 23.45);
    sql::bindValue(q, ":F_INT             ", 567);
    sql::bindValue(q, ":F_NCHAR           ", 0x89);
    sql::bindValue(q, ":F_NVARCHAR_N      ", QString("F_NVARCHAR_N"));
    sql::bindValue(q, ":F_NVARCHAR_MAX    ", QString("F_NVARCHAR_MAX"));
    sql::bindValue(q, ":F_REAL            ", 56.78);
    sql::bindValue(q, ":F_SMALLDATETIME   ", dtime);
    sql::bindValue(q, ":F_SMALLINT        ", 34);
    sql::bindValue(q, ":F_TIME            ", dtime);
    sql::bindValue(q, ":F_TINYINT         ", (qint16)2);
    sql::bindValue(q, ":F_VARBINARY_N     ", QString("F_VARBINARY_N"));
    sql::bindValue(q, ":F_VARBINARY_MAX   ", QString("F_VARBINARY_MAX"));
    sql::bindValue(q, ":F_VARCHAR_N       ", QString("F_VARCHAR_N"));
    sql::bindValue(q, ":F_VARCHAR_MAX     ", QString("F_VARCHAR_MAX"));

    if (!q.exec())
        return;

    db::mssql::Driver::Ptr dbcon2 = mspool().connect();
    QSqlQuery q2 {dbcon2->createResult()};

    q2.setForwardOnly(false);

    if (!q2.prepare("SELECT * FROM TABLE1 WHERE F_GUID = :F_GUID"))
        return;

    sql::bindValue(q2, ":F_GUID", id);

    if (q2.exec())
    {
        log_info << "--- Insert & Select-query 4 exec success ---";
    }

    log_info << "Select-query 4 records count: "
              << db::mssql::resultSize(q2, mspool().connect());

    _queryModel.setQuery(q2);


    log_info << "NumRowsAffected: " << q2.numRowsAffected();
}

void MainWindow::on_btnUpdate1_clicked(bool)
{
    _queryModel.setQuery(QSqlQuery());

    db::mssql::Driver::Ptr dbcon = mspool().connect();
    QSqlQuery q {dbcon->createResult()};

    if (sql::exec(q, "UPDATE TABLE1 SET F_FLOAT = F_FLOAT*2"))
    {
        log_info << "--- Update-query 1 exec success ---";
    }
}

void MainWindow::on_btnUpdate2_clicked(bool)
{
    _queryModel.setQuery(QSqlQuery());

    db::mssql::Driver::Ptr dbcon = mspool().connect();
    QSqlQuery q {dbcon->createResult()};

    QList<QString> match =
    {
        "[f_guid]"
    };

    QList<QString> fields =
    {
        "[F_GUID]",
        "[F_BIGINT]",
        "[F_BINARY]",
        "[F_BIT]",
        "[F_CHAR]",
        "[F_DATE]",
        "[F_DATETIME]",
        "[F_DATETIME2]",
        "[F_DATETIMEOFFSET]",
        "[F_DECIMAL]",
        "[F_FLOAT]",
        "[F_INT]",
        "[F_NCHAR]",
        "[F_NUMERIC]",
        "[F_NVARCHAR_N]",
        "[F_NVARCHAR_MAX]",
        "[F_REAL]",
        "[F_SMALLDATETIME]",
        "[F_SMALLINT]",
        "[F_TIME]",
        "[F_TINYINT]",
        "[F_VARBINARY_N]",
        "[F_VARBINARY_MAX]",
        "[F_VARCHAR_N]",
        "[F_VARCHAR_MAX]"
    };

    QString sql = sql::MERGE_ROW_MS("table2", fields, match);

    if (!q.prepare(sql))
        return;

    QUuidEx id {"e7da463a-e96d-43e0-baeb-99278a1845ee"};
    QDateTime dtime = QDateTime::currentDateTime();

    sql::bindValue(q, ":F_GUID          ", QUuidEx().createUuid() );
    sql::bindValue(q, ":F_BIGINT        ", 1234 );
    sql::bindValue(q, ":F_BINARY        ", QByteArray("F_BINARY") );
    sql::bindValue(q, ":F_BIT           ", false );
    sql::bindValue(q, ":F_CHAR          ", 0x57  );
    sql::bindValue(q, ":F_DATE          ", dtime );
    sql::bindValue(q, ":F_DATETIME      ", dtime );
    sql::bindValue(q, ":F_DATETIME2     ", dtime );
    sql::bindValue(q, ":F_DATETIMEOFFSET", dtime );
    sql::bindValue(q, ":F_DECIMAL       ", 12.34 );
    sql::bindValue(q, ":F_FLOAT         ", 23.45);
    sql::bindValue(q, ":F_INT           ", 567);
    sql::bindValue(q, ":F_NCHAR         ", 0x89);
    sql::bindValue(q, ":F_NUMERIC       ", 34.56);
    sql::bindValue(q, ":F_NVARCHAR_N    ", QString("F_NVARCHAR_N"));
    sql::bindValue(q, ":F_NVARCHAR_MAX  ", QString("F_NVARCHAR_MAX"));
    sql::bindValue(q, ":F_REAL          ", 56.78);
    sql::bindValue(q, ":F_SMALLDATETIME ", dtime);
    sql::bindValue(q, ":F_SMALLINT      ", 34);
    sql::bindValue(q, ":F_TIME          ", dtime);
    sql::bindValue(q, ":F_TINYINT       ", (qint16)2);
    sql::bindValue(q, ":F_VARBINARY_N   ", QString("F_VARBINARY_N"));
    sql::bindValue(q, ":F_VARBINARY_MAX ", QString("F_VARBINARY_MAX"));
    sql::bindValue(q, ":F_VARCHAR_N     ", QString("F_VARCHAR_N"));
    sql::bindValue(q, ":F_VARCHAR_MAX   ", QString("F_VARCHAR_MAX"));

    if (q.exec())
    {
        log_info << "--- Update-query 2 exec success ---";
    }
}

void MainWindow::on_btnUpdate3_clicked(bool)
{
    _queryModel.setQuery(QSqlQuery());

    db::mssql::Driver::Ptr dbcon = mspool().connect();
    QSqlQuery q {dbcon->createResult()};

    QList<QString> match =
    {
        "[f_guid]"
    };

    QList<QString> fields =
    {
        "[F_GUID]",
        "[F_BIGINT]",
        "[F_BINARY]",
        "[F_BIT]",
        "[F_CHAR]",
        "[F_DATE]",
        "[F_DATETIME]",
        "[F_DATETIME2]",
        "[F_DATETIMEOFFSET]",
        "[F_DECIMAL]",
        "[F_FLOAT]",
        "[F_INT]",
        "[F_NCHAR]",
        "[F_NUMERIC]",
        "[F_NVARCHAR_N]",
        "[F_NVARCHAR_MAX]",
        "[F_REAL]",
        "[F_SMALLDATETIME]",
        "[F_SMALLINT]",
        "[F_TIME]",
        "[F_TINYINT]",
        "[F_VARBINARY_N]",
        "[F_VARBINARY_MAX]",
        "[F_VARCHAR_N]",
        "[F_VARCHAR_MAX]"
    };

    QString sql = sql::MERGE_TABLE_MS("table2", "table1", fields, match);

    if (!q.prepare(sql))
        return;

    if (q.exec())
    {
        log_info << "--- Update-query 2 exec success ---";
    }

    _queryModel.setQuery(QSqlQuery());

    if (q.exec())
    {
        log_info << "--- Update-query 2 exec success ---";
    }
}

void MainWindow::on_btnInsert1_clicked(bool)
{
    _queryModel.setQuery(QSqlQuery());

    db::mssql::Driver::Ptr dbcon = mspool().connect();
    QSqlQuery q = QSqlQuery(dbcon->createResult());

    q.prepare("INSERT               "
              "  TABLE1             "
              " (                   "
              "  [F_GUID]           "
              " ,[F_BIGINT]         "
              " ,[F_BINARY]         "
              " ,[F_BIT]            "
              " ,[F_CHAR]           "
              " ,[F_DATE]           "
              " ,[F_DATETIME]       "
              " ,[F_DATETIME2]      "
              " ,[F_DATETIMEOFFSET] "
              " ,[F_DECIMAL]        "
              " ,[F_FLOAT]          "
              " ,[F_INT]            "
              " ,[F_NCHAR]          "
              " ,[F_NUMERIC]        "
              " ,[F_NVARCHAR_N]     "
              " ,[F_NVARCHAR_MAX]   "
              " ,[F_REAL]           "
              " ,[F_SMALLDATETIME]  "
              " ,[F_SMALLINT]       "
              " ,[F_TIME]           "
              " ,[F_TINYINT]        "
              " ,[F_VARBINARY_N]    "
              " ,[F_VARBINARY_MAX]  "
              " ,[F_VARCHAR_N]      "
              " ,[F_VARCHAR_MAX]    "
              " )                   "
              " VALUES              "
              " (                   "
              "  :F_GUID            "
              " ,:F_BIGINT          "
              " ,:F_BINARY          "
              " ,:F_BIT             "
              " ,:F_CHAR            "
              " ,:F_DATE            "
              " ,:F_DATETIME        "
              " ,:F_DATETIME2       "
              " ,:F_DATETIMEOFFSET  "
              " ,:F_DECIMAL         "
              " ,:F_FLOAT           "
              " ,:F_INT             "
              " ,:F_NCHAR           "
              " ,:F_NUMERIC         "
              " ,:F_NVARCHAR_N      "
              " ,:F_NVARCHAR_MAX    "
              " ,:F_REAL            "
              " ,:F_SMALLDATETIME   "
              " ,:F_SMALLINT        "
              " ,:F_TIME            "
              " ,:F_TINYINT         "
              " ,:F_VARBINARY_N     "
              " ,:F_VARBINARY_MAX   "
              " ,:F_VARCHAR_N       "
              " ,:F_VARCHAR_MAX     "
              " );                  ");

    QDateTime dtime = QDateTime::currentDateTime();

    sql::bindValue(q, ":F_GUID          ", QUuidEx().createUuid() );
    sql::bindValue(q, ":F_BIGINT        ", 1234567 );
    sql::bindValue(q, ":F_BINARY        ", QByteArray("F_BINARY") );
    sql::bindValue(q, ":F_BIT           ", false );
    sql::bindValue(q, ":F_CHAR          ", 0x57  );
    sql::bindValue(q, ":F_DATE          ", dtime );
    sql::bindValue(q, ":F_DATETIME      ", dtime );
    sql::bindValue(q, ":F_DATETIME2     ", dtime );
    sql::bindValue(q, ":F_DATETIMEOFFSET", dtime );
    sql::bindValue(q, ":F_DECIMAL       ", 12.34 );
    sql::bindValue(q, ":F_FLOAT         ", 23.45);
    sql::bindValue(q, ":F_INT           ", 567);
    sql::bindValue(q, ":F_NCHAR         ", 0x89);
    sql::bindValue(q, ":F_NUMERIC       ", 34.56);
    sql::bindValue(q, ":F_NVARCHAR_N    ", QString("F_NVARCHAR_N"));
    sql::bindValue(q, ":F_NVARCHAR_MAX  ", QString("F_NVARCHAR_MAX"));
    sql::bindValue(q, ":F_REAL          ", 56.78);
    sql::bindValue(q, ":F_SMALLDATETIME ", dtime);
    sql::bindValue(q, ":F_SMALLINT      ", 34);
    sql::bindValue(q, ":F_TIME          ", dtime);
    sql::bindValue(q, ":F_TINYINT       ", 2);
    sql::bindValue(q, ":F_VARBINARY_N   ", QString("F_VARBINARY_N"));
    sql::bindValue(q, ":F_VARBINARY_MAX ", QString("F_VARBINARY_MAX"));
    sql::bindValue(q, ":F_VARCHAR_N     ", QString("F_VARCHAR_N"));
    sql::bindValue(q, ":F_VARCHAR_MAX   ", QString("F_VARCHAR_MAX"));

    if (q.exec())
    {
        log_info << "--- Insert-query 1 exec success ---";
    }
}

void MainWindow::on_btnInsert2_clicked(bool)
{
    _queryModel.setQuery(QSqlQuery());

    db::mssql::Driver::Ptr dbcon = mspool().connect();
    db::mssql::Transaction::Ptr transact = dbcon->createTransact();
    QSqlQuery q {db::mssql::createResult(transact)};

    if (!transact->begin())
        return;

    q.prepare("INSERT               "
              "  TABLE1             "
              " (                   "
              "  [F_GUID]           "
              " ,[F_BIGINT]         "
              " ,[F_BINARY]         "
              " ,[F_BIT]            "
              " ,[F_CHAR]           "
              " ,[F_DATE]           "
              " ,[F_DATETIME]       "
              " ,[F_DATETIME2]      "
              " ,[F_DATETIMEOFFSET] "
              " ,[F_DECIMAL]        "
              " ,[F_FLOAT]          "
              " ,[F_INT]            "
              " ,[F_NCHAR]          "
              " ,[F_NUMERIC]        "
              " ,[F_NVARCHAR_N]     "
              " ,[F_NVARCHAR_MAX]   "
              " ,[F_REAL]           "
              " ,[F_SMALLDATETIME]  "
              " ,[F_SMALLINT]       "
              " ,[F_TIME]           "
              " ,[F_TINYINT]        "
              " ,[F_VARBINARY_N]    "
              " ,[F_VARBINARY_MAX]  "
              " ,[F_VARCHAR_N]      "
              " ,[F_VARCHAR_MAX]    "
              " )                   "
              " VALUES              "
              " (                   "
              "  :F_GUID            "
              " ,:F_BIGINT          "
              " ,:F_BINARY          "
              " ,:F_BIT             "
              " ,:F_CHAR            "
              " ,:F_DATE            "
              " ,:F_DATETIME        "
              " ,:F_DATETIME2       "
              " ,:F_DATETIMEOFFSET  "
              " ,:F_DECIMAL         "
              " ,:F_FLOAT           "
              " ,:F_INT             "
              " ,:F_NCHAR           "
              " ,:F_NUMERIC         "
              " ,:F_NVARCHAR_N      "
              " ,:F_NVARCHAR_MAX    "
              " ,:F_REAL            "
              " ,:F_SMALLDATETIME   "
              " ,:F_SMALLINT        "
              " ,:F_TIME            "
              " ,:F_TINYINT         "
              " ,:F_VARBINARY_N     "
              " ,:F_VARBINARY_MAX   "
              " ,:F_VARCHAR_N       "
              " ,:F_VARCHAR_MAX     "
              " );                  ");

    QDateTime dtime = QDateTime::currentDateTime();

    sql::bindValue(q, ":F_GUID          ", QUuidEx().createUuid() );
    sql::bindValue(q, ":F_BIGINT        ", 1234 );
    sql::bindValue(q, ":F_BINARY        ", QByteArray("F_BINARY") );
    sql::bindValue(q, ":F_BIT           ", false );
    sql::bindValue(q, ":F_CHAR          ", 0x57  );
    sql::bindValue(q, ":F_DATE          ", dtime );
    sql::bindValue(q, ":F_DATETIME      ", dtime );
    sql::bindValue(q, ":F_DATETIME2     ", dtime );
    sql::bindValue(q, ":F_DATETIMEOFFSET", dtime );
    sql::bindValue(q, ":F_DECIMAL       ", 12.34 );
    sql::bindValue(q, ":F_FLOAT         ", 23.45);
    sql::bindValue(q, ":F_INT           ", 567);
    sql::bindValue(q, ":F_NCHAR         ", 0x89);
    sql::bindValue(q, ":F_NUMERIC       ", 34.56);
    sql::bindValue(q, ":F_NVARCHAR_N    ", QString("F_NVARCHAR_N"));
    sql::bindValue(q, ":F_NVARCHAR_MAX  ", QString("F_NVARCHAR_MAX"));
    sql::bindValue(q, ":F_REAL          ", 56.78);
    sql::bindValue(q, ":F_SMALLDATETIME ", dtime);
    sql::bindValue(q, ":F_SMALLINT      ", 34);
    sql::bindValue(q, ":F_TIME          ", dtime);
    sql::bindValue(q, ":F_TINYINT       ", 2);
    sql::bindValue(q, ":F_VARBINARY_N   ", QString("F_VARBINARY_N"));
    sql::bindValue(q, ":F_VARBINARY_MAX ", QString("F_VARBINARY_MAX"));
    sql::bindValue(q, ":F_VARCHAR_N     ", QString("F_VARCHAR_N"));
    sql::bindValue(q, ":F_VARCHAR_MAX   ", QString("F_VARCHAR_MAX"));

    if (q.exec())
    {
        log_info << "--- Insert-query 2 exec success ---";
    }

    if (transact->commit())
    {
        log_info << "--- Insert-query 2 exec success ---";
    }
}

void insertThread(db::mssql::Transaction::Ptr transact, int index)
{
    QSqlQuery q {db::mssql::createResult(transact)};

    q.prepare("INSERT                  "
              "  TABLE1                "
              "  (                     "
              "     [F_GUID]           "
              "    ,[F_BIGINT]         "
              "    ,[F_BINARY]         "
              "    ,[F_BIT]            "
              "    ,[F_CHAR]           "
              "    ,[F_DATE]           "
              "    ,[F_DATETIME]       "
              "    ,[F_DATETIME2]      "
              "    ,[F_DATETIMEOFFSET] "
              "    ,[F_DECIMAL]        "
              "    ,[F_FLOAT]          "
              "    ,[F_INT]            "
              "    ,[F_NCHAR]          "
              "    ,[F_NUMERIC]        "
              "    ,[F_NVARCHAR_N]     "
              "    ,[F_NVARCHAR_MAX]   "
              "    ,[F_REAL]           "
              "    ,[F_SMALLDATETIME]  "
              "    ,[F_SMALLINT]       "
              "    ,[F_TIME]           "
              "    ,[F_TINYINT]        "
              "    ,[F_VARBINARY_N]    "
              "    ,[F_VARBINARY_MAX]  "
              "    ,[F_VARCHAR_N]      "
              "    ,[F_VARCHAR_MAX]    "
              "  )                     "
              "  VALUES                "
              "  (                     "
              "     :F_GUID            "
              "    ,:F_BIGINT          "
              "    ,:F_BINARY          "
              "    ,:F_BIT             "
              "    ,:F_CHAR            "
              "    ,:F_DATE            "
              "    ,:F_DATETIME        "
              "    ,:F_DATETIME2       "
              "    ,:F_DATETIMEOFFSET  "
              "    ,:F_DECIMAL         "
              "    ,:F_FLOAT           "
              "    ,:F_INT             "
              "    ,:F_NCHAR           "
              "    ,:F_NUMERIC         "
              "    ,:F_NVARCHAR_N      "
              "    ,:F_NVARCHAR_MAX    "
              "    ,:F_REAL            "
              "    ,:F_SMALLDATETIME   "
              "    ,:F_SMALLINT        "
              "    ,:F_TIME            "
              "    ,:F_TINYINT         "
              "    ,:F_VARBINARY_N     "
              "    ,:F_VARBINARY_MAX   "
              "    ,:F_VARCHAR_N       "
              "    ,:F_VARCHAR_MAX     "
              "  );                    ");

    QDateTime dtime = QDateTime::currentDateTime();

    for (int i = 0; i < 1000; ++i)
    {
        sql::bindValue(q, ":F_GUID          ", QUuidEx().createUuid() );
        sql::bindValue(q, ":F_BIGINT        ", (index == 0) ? InsertMode::Thread1 : InsertMode::Thread2 );
        sql::bindValue(q, ":F_BINARY        ", QByteArray("F_BINARY") );
        sql::bindValue(q, ":F_BIT           ", index );
        sql::bindValue(q, ":F_CHAR          ", 0x57  );
        sql::bindValue(q, ":F_DATE          ", dtime );
        sql::bindValue(q, ":F_DATETIME      ", dtime );
        sql::bindValue(q, ":F_DATETIME2     ", dtime );
        sql::bindValue(q, ":F_DATETIMEOFFSET", dtime );
        sql::bindValue(q, ":F_DECIMAL       ", 12.34 );
        sql::bindValue(q, ":F_FLOAT         ", 23.45);
        sql::bindValue(q, ":F_INT           ", 567);
        sql::bindValue(q, ":F_NCHAR         ", 0x89);
        sql::bindValue(q, ":F_NUMERIC       ", 34.56);
        sql::bindValue(q, ":F_NVARCHAR_N    ", QString("F_NVARCHAR_N"));
        sql::bindValue(q, ":F_NVARCHAR_MAX  ", QString("F_NVARCHAR_MAX"));
        sql::bindValue(q, ":F_REAL          ", 56.78);
        sql::bindValue(q, ":F_SMALLDATETIME ", dtime);
        sql::bindValue(q, ":F_SMALLINT      ", 34);
        sql::bindValue(q, ":F_TIME          ", dtime);
        sql::bindValue(q, ":F_TINYINT       ", 2);
        sql::bindValue(q, ":F_VARBINARY_N   ", QString("F_VARBINARY_N"));
        sql::bindValue(q, ":F_VARBINARY_MAX ", QString("F_VARBINARY_MAX"));
        sql::bindValue(q, ":F_VARCHAR_N     ", QString("F_VARCHAR_N"));
        sql::bindValue(q, ":F_VARCHAR_MAX   ", QString("F_VARCHAR_MAX"));

        if (!q.exec())
            return;
    }
}

void MainWindow::on_btnInsert3_clicked(bool)
{
    _queryModel.setQuery(QSqlQuery());

    auto threadFunc = [this](int index)
    {
        trd::ThreadIdLock threadIdLock {&_threadIds}; (void) threadIdLock;

        db::mssql::Driver::Ptr dbcon = mspool().connect();
        db::mssql::Transaction::Ptr transact = dbcon->createTransact();

        if (!transact->begin())
            return;

        insertThread(transact, index);

        if (transact->commit())
        {
            log_info << log_format(
                "--- Insert-query 3 (index: %?)) exec success ---", index);
        }
    };

    std::thread t1 {threadFunc, 0};
    std::thread t2 {threadFunc, 1};

    t1.detach();
    t2.detach();
}

void MainWindow::on_btnDeleteAll_clicked(bool)
{
    _queryModel.setQuery(QSqlQuery());

    db::mssql::Driver::Ptr dbcon = mspool().connect();
    QSqlQuery q {dbcon->createResult()};

    if (sql::exec(q, "delete from types_table_2"))
    {
        log_info << "--- Delete-query exec success ---";
    }
}

void MainWindow::on_btnAbortQuery_clicked(bool)
{
    mspool().abortOperations();
}

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
        " ,[F_BOOL]           "
        " ,[F_CHAR]           "
        " ,[F_NCHAR]          "
        " ,[F_TIME]           "
        " ,[F_DATE]           "
        " ,[F_DATETIME]       "
        " ,[F_DATETIME2]      "
        " ,[F_DATETIMEOFFSET] "
      //" ,[F_DECIMAL]        " // Тип не поддерживается
        " ,[F_FLOAT]          "
        " ,[F_REAL]           "
        " ,[F_INT]            "
      //" ,[F_NUMERIC]        " // Тип не поддерживается
      //" ,[F_SMALLDATETIME]  " // Тип не поддерживается
        " ,[F_SMALLINT]       "
        " ,[F_TINYINT]        "
        " ,[F_VARBINARY_MAX]  "
        " ,[F_NVARCHAR_MAX]   "
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
        " SELECT TOP %1       "
        "  [F_GUID]           "
        " ,[F_BIGINT]         "
        " ,[F_BINARY]         "
        " ,[F_BOOL]           "
        " ,[F_CHAR]           "
        " ,[F_NCHAR]          "
        " ,[F_TIME]           "
        " ,[F_DATE]           "
        " ,[F_DATETIME]       "
        " ,[F_DATETIME2]      "
        " ,[F_DATETIMEOFFSET] "
      //" ,[F_DECIMAL]        " // Тип не поддерживается
        " ,[F_FLOAT]          "
        " ,[F_REAL]           "
        " ,[F_INT]            "
      //" ,[F_NUMERIC]        " // Тип не поддерживается
      //" ,[F_SMALLDATETIME]  " // Тип не поддерживается
        " ,[F_SMALLINT]       "
        " ,[F_TINYINT]        "
        " ,[F_VARBINARY_MAX]  "
        " ,[F_NVARCHAR_MAX]   "
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
        bool       f_bool;
        QString    f_char;
        QString    f_nchar;
        QTime      f_time;
        QDate      f_date;
        QDateTime  f_datetime;
        QDateTime  f_datetime2;
        QDateTime  f_datetimeoffset;
      //double     f_decimal; // Тип не поддерживается
        float      f_float;
        double     f_real;
        qint32     f_int;
      //double     f_numeric;       // Тип не поддерживается
      //QDateTime  f_smalldatetime; // Тип не поддерживается
        qint16     f_smallint;
        qint8      f_tinyint;
        QByteArray f_varbinary_max;
        QString    f_nvarchar_max;

        sql::assignValue(f_guid          , r, "F_GUID           ");
        sql::assignValue(f_bigint        , r, "F_BIGINT         ");
        sql::assignValue(f_binary        , r, "F_BINARY         ");
        sql::assignValue(f_bool          , r, "F_BOOL           ");
        sql::assignValue(f_char          , r, "F_CHAR           ");
        sql::assignValue(f_nchar         , r, "F_NCHAR          ");
        sql::assignValue(f_time          , r, "F_TIME           ");
        sql::assignValue(f_date          , r, "F_DATE           ");
        sql::assignValue(f_datetime      , r, "F_DATETIME       ");
        sql::assignValue(f_datetime2     , r, "F_DATETIME2      ");
        sql::assignValue(f_datetimeoffset, r, "F_DATETIMEOFFSET ");
        sql::assignValue(f_float         , r, "F_FLOAT          ");
        sql::assignValue(f_real          , r, "F_REAL           ");
        sql::assignValue(f_int           , r, "F_INT            ");
        sql::assignValue(f_smallint      , r, "F_SMALLINT       ");
        sql::assignValue(f_tinyint       , r, "F_TINYINT        ");
        sql::assignValue(f_varbinary_max , r, "F_VARBINARY_MAX  ");
        sql::assignValue(f_nvarchar_max  , r, "F_NVARCHAR_MAX   ");

        log_debug << "F_GUID           " << f_guid          ;
        log_debug << "F_BIGINT         " << f_bigint        ;
        log_debug << "F_BINARY         " << f_binary        ;
        log_debug << "F_BOOL           " << f_bool          ;
        log_debug << "F_CHAR           " << f_char          ;
        log_debug << "F_NCHAR          " << f_nchar         ;
        log_debug << "F_TIME           " << f_time          ;
        log_debug << "F_DATE           " << f_date          ;
        log_debug << "F_DATETIME       " << f_datetime      ;
        log_debug << "F_DATETIME2      " << f_datetime2     ;
        log_debug << "F_DATETIMEOFFSET " << f_datetimeoffset;
        log_debug << "F_FLOAT          " << f_float         ;
        log_debug << "F_REAL           " << f_real          ;
        log_debug << "F_INT            " << f_int           ;
        log_debug << "F_SMALLINT       " << f_smallint      ;
        log_debug << "F_TINYINT        " << f_tinyint       ;
        log_debug << "F_VARBINARY_MAX  " << f_varbinary_max ;
        log_debug << "F_NVARCHAR_MAX   " << f_nvarchar_max  ;

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
              " ,[F_BOOL]           "
              " ,[F_CHAR]           "
              " ,[F_NCHAR]          "
              " ,[F_TIME]           "
              " ,[F_DATE]           "
              " ,[F_DATETIME]       "
              " ,[F_DATETIME2]      "
              " ,[F_DATETIMEOFFSET] "
            //" ,[F_DECIMAL]        " // Тип не поддерживается
              " ,[F_FLOAT]          "
              " ,[F_REAL]           "
              " ,[F_INT]            "
            //" ,[F_NUMERIC]        " // Тип не поддерживается
            //" ,[F_SMALLDATETIME]  " // Тип не поддерживается
              " ,[F_SMALLINT]       "
              " ,[F_TINYINT]        "
              " ,[F_VARBINARY_MAX]  "
              " ,[F_NVARCHAR_MAX]   "
              " )                   "
              " VALUES              "
              " (                   "
              "  :F_GUID            "
              " ,:F_BIGINT          "
              " ,:F_BINARY          "
              " ,:F_BOOL            "
              " ,:F_CHAR            "
              " ,:F_NCHAR           "
              " ,:F_TIME            "
              " ,:F_DATE            "
              " ,:F_DATETIME        "
              " ,:F_DATETIME2       "
              " ,:F_DATETIMEOFFSET  "
              " ,:F_FLOAT           "
              " ,:F_REAL            "
              " ,:F_INT             "
              " ,:F_SMALLINT        "
              " ,:F_TINYINT         "
              " ,:F_VARBINARY_MAX   "
              " ,:F_NVARCHAR_MAX    "
              " );                  ");

    QDateTime dtime = QDateTime::currentDateTime();

    sql::bindValue(q, ":F_GUID            ", id);
    sql::bindValue(q, ":F_BIGINT          ", 1234 );
    sql::bindValue(q, ":F_BINARY          ", QByteArray("F_BINARY") );
    sql::bindValue(q, ":F_BOOL            ", true );
    sql::bindValue(q, ":F_CHAR            ", "strfromqt"  );
    sql::bindValue(q, ":F_NCHAR           ", 0x89);
    sql::bindValue(q, ":F_TIME            ", dtime);
    sql::bindValue(q, ":F_DATE            ", dtime );
    sql::bindValue(q, ":F_DATETIME        ", dtime );
    sql::bindValue(q, ":F_DATETIME2       ", dtime );
    sql::bindValue(q, ":F_DATETIMEOFFSET  ", dtime );
    sql::bindValue(q, ":F_FLOAT           ", 23.45);
    sql::bindValue(q, ":F_REAL            ", 56.78);
    sql::bindValue(q, ":F_INT             ", 567);
    sql::bindValue(q, ":F_SMALLINT        ", 34);
    sql::bindValue(q, ":F_TINYINT         ", (qint16)2);
    sql::bindValue(q, ":F_VARBINARY_MAX   ", QString("F_VARBINARY_MAX"));
    sql::bindValue(q, ":F_NVARCHAR_MAX    ", QString(u8"F_NVARCHAR_MAX Текст по русски"));

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

    QStringList match =
    {
        "[F_GUID]"
    };

    QStringList fields =
    {
        " [F_GUID]           "
        ",[F_BIGINT]         "
        ",[F_BINARY]         "
        ",[F_BOOL]           "
        ",[F_CHAR]           "
        ",[F_NCHAR]          "
        ",[F_TIME]           "
        ",[F_DATE]           "
        ",[F_DATETIME]       "
        ",[F_DATETIME2]      "
        ",[F_DATETIMEOFFSET] "
        ",[F_FLOAT]          "
        ",[F_REAL]           "
        ",[F_INT]            "
        ",[F_SMALLINT]       "
        ",[F_TINYINT]        "
        ",[F_VARBINARY_MAX]  "
        ",[F_NVARCHAR_MAX]   "
    };

    QString sql = sql::MERGE_ROW_MS("table2", fields, match);

    if (!q.prepare(sql))
        return;

    QUuidEx id {"e7da463a-e96d-43e0-baeb-99278a1845ee"};
    QDateTime dtime = QDateTime::currentDateTime();

    sql::bindValue(q, ":F_GUID          ", QUuidEx().createUuid() );
    sql::bindValue(q, ":F_BIGINT        ", 1234 );
    sql::bindValue(q, ":F_BINARY        ", QByteArray("F_BINARY") );
    sql::bindValue(q, ":F_BOOL          ", true );
    sql::bindValue(q, ":F_CHAR          ", 0x57  );
    sql::bindValue(q, ":F_NCHAR         ", 0x89);
    sql::bindValue(q, ":F_TIME          ", dtime);
    sql::bindValue(q, ":F_DATE          ", dtime );
    sql::bindValue(q, ":F_DATETIME      ", dtime );
    sql::bindValue(q, ":F_DATETIME2     ", dtime );
    sql::bindValue(q, ":F_DATETIMEOFFSET", dtime );
    sql::bindValue(q, ":F_FLOAT         ", 23.45);
    sql::bindValue(q, ":F_REAL          ", 56.78);
    sql::bindValue(q, ":F_INT           ", 567);
    sql::bindValue(q, ":F_SMALLINT      ", 34);
    sql::bindValue(q, ":F_TINYINT       ", (qint16)2);
    sql::bindValue(q, ":F_VARBINARY_MAX ", QString("F_VARBINARY_MAX"));
    sql::bindValue(q, ":F_NVARCHAR_MAX  ", QString(u8"F_NVARCHAR_MAX Текст по русски 2222"));

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

    q.setForwardOnly(false);

    QString fields =
        " [F_GUID]           "
        ",[F_BIGINT]         "
        ",[F_BINARY]         "
        ",[F_BOOL]           "
        ",[F_CHAR]           "
        ",[F_NCHAR]          "
        ",[F_TIME]           "
        ",[F_DATE]           "
        ",[F_DATETIME]       "
        ",[F_DATETIME2]      "
        ",[F_DATETIMEOFFSET] "
        ",[F_FLOAT]          "
        ",[F_REAL]           "
        ",[F_INT]            "
        ",[F_SMALLINT]       "
        ",[F_TINYINT]        "
        ",[F_VARBINARY_MAX]  "
        ",[F_NVARCHAR_MAX]   ";

    QString query = "SELECT %1 FROM TABLE1";
    query = query.arg(fields);

    if (!sql::exec(q, query))
        return;

    db::mssql::Driver::Ptr dbcon2 = mspool().connect();
    db::mssql::Transaction::Ptr transact2 = dbcon2->createTransact();
    QSqlQuery q2 {db::mssql::createResult(transact2)};

    if (!transact2->begin())
        return;

    QString sql2 = sql::MERGE_ROW_MS("table2", fields, "[F_GUID]");

    if (!q2.prepare(sql2))
        return;

    while (q.next())
    {
        QSqlRecord r = q.record();

        QUuidEx    f_guid;
        qint64     f_bigint;
        QByteArray f_binary;
        bool       f_bool;
        QString    f_char;
        QString    f_nchar;
        QTime      f_time;
        QDate      f_date;
        QDateTime  f_datetime;
        QDateTime  f_datetime2;
        QDateTime  f_datetimeoffset;
        float      f_float;
        double     f_real;
        qint32     f_int;
        qint16     f_smallint;
        qint8      f_tinyint;
        QByteArray f_varbinary_max;
        QString    f_nvarchar_max;

        sql::assignValue(f_guid          , r, "F_GUID           ");
        sql::assignValue(f_bigint        , r, "F_BIGINT         ");
        sql::assignValue(f_binary        , r, "F_BINARY         ");
        sql::assignValue(f_bool          , r, "F_BOOL           ");
        sql::assignValue(f_char          , r, "F_CHAR           ");
        sql::assignValue(f_nchar         , r, "F_NCHAR          ");
        sql::assignValue(f_time          , r, "F_TIME           ");
        sql::assignValue(f_date          , r, "F_DATE           ");
        sql::assignValue(f_datetime      , r, "F_DATETIME       ");
        sql::assignValue(f_datetime2     , r, "F_DATETIME2      ");
        sql::assignValue(f_datetimeoffset, r, "F_DATETIMEOFFSET ");
        sql::assignValue(f_float         , r, "F_FLOAT          ");
        sql::assignValue(f_real          , r, "F_REAL           ");
        sql::assignValue(f_int           , r, "F_INT            ");
        sql::assignValue(f_smallint      , r, "F_SMALLINT       ");
        sql::assignValue(f_tinyint       , r, "F_TINYINT        ");
        sql::assignValue(f_varbinary_max , r, "F_VARBINARY_MAX  ");
        sql::assignValue(f_nvarchar_max  , r, "F_NVARCHAR_MAX   ");

        sql::bindValue(q2, ":F_GUID          ", f_guid           );
        sql::bindValue(q2, ":F_BIGINT        ", f_bigint         );
        sql::bindValue(q2, ":F_BINARY        ", f_binary         );
        sql::bindValue(q2, ":F_BOOL          ", f_bool           );
        sql::bindValue(q2, ":F_CHAR          ", f_char.trimmed() );
        sql::bindValue(q2, ":F_NCHAR         ", f_nchar.trimmed());
      //sql::bindValue(q2, ":F_CHAR          ", QString()        );
      //sql::bindValue(q2, ":F_NCHAR         ", QString()        );
        sql::bindValue(q2, ":F_TIME          ", f_time           );
        sql::bindValue(q2, ":F_DATE          ", f_date           );
        sql::bindValue(q2, ":F_DATETIME      ", f_datetime       );
        sql::bindValue(q2, ":F_DATETIME2     ", f_datetime2      );
        sql::bindValue(q2, ":F_DATETIMEOFFSET", f_datetimeoffset );
        sql::bindValue(q2, ":F_FLOAT         ", f_float          );
        sql::bindValue(q2, ":F_REAL          ", f_real           );
        sql::bindValue(q2, ":F_INT           ", f_int            );
        sql::bindValue(q2, ":F_SMALLINT      ", f_smallint       );
        sql::bindValue(q2, ":F_TINYINT       ", f_tinyint        );
        sql::bindValue(q2, ":F_VARBINARY_MAX ", f_varbinary_max  );
        sql::bindValue(q2, ":F_NVARCHAR_MAX  ", f_nvarchar_max   );

        if (!q2.exec())
            return;
    }

    if (transact2->commit())
    {
        log_info << "--- Update-query 3 transact success ---";
    }
}

void MainWindow::on_btnUpdate4_clicked(bool)
{
    _queryModel.setQuery(QSqlQuery());

    db::mssql::Driver::Ptr dbcon = mspool().connect();
    QSqlQuery q {dbcon->createResult()};

    QString match = "[F_GUID]";

    QString fields =
        " [F_GUID]           "
        ",[F_BIGINT]         "
        ",[F_BINARY]         "
        ",[F_BOOL]           "
        ",[F_CHAR]           "
        ",[F_NCHAR]          "
        ",[F_TIME]           "
        ",[F_DATE]           "
        ",[F_DATETIME]       "
        ",[F_DATETIME2]      "
        ",[F_DATETIMEOFFSET] "
        ",[F_FLOAT]          "
        ",[F_REAL]           "
        ",[F_INT]            "
        ",[F_SMALLINT]       "
        ",[F_TINYINT]        "
        ",[F_VARBINARY_MAX]  "
        ",[F_NVARCHAR_MAX]   ";

    QString sql = sql::MERGE_TABLE_MS("table2", "table1", fields, match);

    if (!q.prepare(sql))
        return;

    if (q.exec())
    {
        log_info << "--- Update-query 4 exec success ---";
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
              " ,[F_BOOL]           "
              " ,[F_CHAR]           "
              " ,[F_NCHAR]          "
              " ,[F_TIME]           "
              " ,[F_DATE]           "
              " ,[F_DATETIME]       "
              " ,[F_DATETIME2]      "
              " ,[F_DATETIMEOFFSET] "
            //" ,[F_DECIMAL]        " // Тип не поддерживается
              " ,[F_FLOAT]          "
              " ,[F_REAL]           "
              " ,[F_INT]            "
            //" ,[F_NUMERIC]        " // Тип не поддерживается
            //" ,[F_SMALLDATETIME]  " // Тип не поддерживается
              " ,[F_SMALLINT]       "
              " ,[F_TINYINT]        "
              " ,[F_VARBINARY_MAX]  "
              " ,[F_NVARCHAR_MAX]   "
              " )                   "
              " VALUES              "
              " (                   "
              "  :F_GUID            "
              " ,:F_BIGINT          "
              " ,:F_BINARY          "
              " ,:F_BOOL            "
              " ,:F_CHAR            "
              " ,:F_NCHAR           "
              " ,:F_DATE            "
              " ,:F_TIME            "
              " ,:F_DATETIME        "
              " ,:F_DATETIME2       "
              " ,:F_DATETIMEOFFSET  "
            //" ,:F_DECIMAL         "
              " ,:F_FLOAT           "
              " ,:F_REAL            "
              " ,:F_INT             "
            //" ,:F_NUMERIC         "
            //" ,:F_SMALLDATETIME   "
              " ,:F_SMALLINT        "
              " ,:F_TINYINT         "
              " ,:F_VARBINARY_MAX   "
              " ,:F_NVARCHAR_MAX    "
              " );                  ");

    QUuid uuid = QUuidEx().createUuid();
    QDateTime dtime = QDateTime::currentDateTime();

    sql::bindValue(q, ":F_GUID          ", uuid);
    sql::bindValue(q, ":F_BIGINT        ", 123456214878);
    sql::bindValue(q, ":F_BINARY        ", QByteArray("F_BINARY F_BINARY F_BINARY F_BINARY F_BINARY F_BINARY F_BINARY F_BINARY F_BINARY F_BINARY F_BINARY"));
    sql::bindValue(q, ":F_BOOL          ", true);
    sql::bindValue(q, ":F_CHAR          ", QString(u8"F_CHAR поле"));
    sql::bindValue(q, ":F_NCHAR         ", QString(u8"F_NCHAR поле"));
    sql::bindValue(q, ":F_TIME          ", dtime );
    sql::bindValue(q, ":F_DATE          ", dtime );
    sql::bindValue(q, ":F_DATETIME      ", dtime );
    sql::bindValue(q, ":F_DATETIME2     ", dtime );
    sql::bindValue(q, ":F_DATETIMEOFFSET", dtime );
  //sql::bindValue(q, ":F_DECIMAL       ", 12.34 ); // Тип не поддерживается
    sql::bindValue(q, ":F_FLOAT         ", 23.45 );
    sql::bindValue(q, ":F_REAL          ", 56.78 );
    sql::bindValue(q, ":F_INT           ", 567   );
  //sql::bindValue(q, ":F_NUMERIC       ", 34.56 ); // Тип не поддерживается
  //sql::bindValue(q, ":F_SMALLDATETIME ", dtime ); // Тип не поддерживается
    sql::bindValue(q, ":F_SMALLINT      ", 34    );
    sql::bindValue(q, ":F_TINYINT       ", 2     );
    sql::bindValue(q, ":F_VARBINARY_MAX ", QString("F_VARBINARY_MAX"));
    sql::bindValue(q, ":F_NVARCHAR_MAX  ", QString(u8"Тестовая строка на русском языке. Продолжение тестовой строки"));

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
              " ,[F_BOOL]           "
              " ,[F_CHAR]           "
              " ,[F_NCHAR]          "
              " ,[F_TIME]           "
              " ,[F_DATE]           "
              " ,[F_DATETIME]       "
              " ,[F_DATETIME2]      "
              " ,[F_DATETIMEOFFSET] "
            //" ,[F_DECIMAL]        " // Тип не поддерживается
              " ,[F_FLOAT]          "
              " ,[F_REAL]           "
              " ,[F_INT]            "
            //" ,[F_NUMERIC]        " // Тип не поддерживается
            //" ,[F_SMALLDATETIME]  " // Тип не поддерживается
              " ,[F_SMALLINT]       "
              " ,[F_TINYINT]        "
              " ,[F_VARBINARY_MAX]  "
              " ,[F_NVARCHAR_MAX]   "
              " )                   "
              " VALUES              "
              " (                   "
              "  :F_GUID            "
              " ,:F_BIGINT          "
              " ,:F_BINARY          "
              " ,:F_BOOL            "
              " ,:F_CHAR            "
              " ,:F_NCHAR           "
              " ,:F_DATE            "
              " ,:F_TIME            "
              " ,:F_DATETIME        "
              " ,:F_DATETIME2       "
              " ,:F_DATETIMEOFFSET  "
            //" ,:F_DECIMAL         "
              " ,:F_FLOAT           "
              " ,:F_REAL            "
              " ,:F_INT             "
            //" ,:F_NUMERIC         "
            //" ,:F_SMALLDATETIME   "
              " ,:F_SMALLINT        "
              " ,:F_TINYINT         "
              " ,:F_VARBINARY_MAX   "
              " ,:F_NVARCHAR_MAX    "
              " );                  ");

    QUuid uuid = QUuidEx().createUuid(); (void) uuid;
    QDateTime dtime = QDateTime::currentDateTime();

  //sql::bindValue(q, ":F_GUID          ", uuid );
    sql::bindValue(q, ":F_GUID          ", QVariant(QVariant::Uuid));
    sql::bindValue(q, ":F_BIGINT        ", 123456214878);
    sql::bindValue(q, ":F_BINARY        ", QByteArray("F_BINARY F_BINARY F_BINARY F_BINARY F_BINARY F_BINARY F_BINARY F_BINARY F_BINARY F_BINARY F_BINARY"));
    sql::bindValue(q, ":F_BOOL          ", true  );
    sql::bindValue(q, ":F_CHAR          ", 0x57  );
    sql::bindValue(q, ":F_NCHAR         ", 0x89  );
    sql::bindValue(q, ":F_TIME          ", dtime );
    sql::bindValue(q, ":F_DATE          ", dtime );
    sql::bindValue(q, ":F_DATETIME      ", dtime );
    sql::bindValue(q, ":F_DATETIME2     ", dtime );
    sql::bindValue(q, ":F_DATETIMEOFFSET", dtime );
  //sql::bindValue(q, ":F_DECIMAL       ", 12.34 ); // Тип не поддерживается
    sql::bindValue(q, ":F_FLOAT         ", 23.45 );
    sql::bindValue(q, ":F_REAL          ", 56.78 );
    sql::bindValue(q, ":F_INT           ", 567   );
  //sql::bindValue(q, ":F_NUMERIC       ", 34.56 ); // Тип не поддерживается
  //sql::bindValue(q, ":F_SMALLDATETIME ", dtime ); // Тип не поддерживается
    sql::bindValue(q, ":F_SMALLINT      ", 34    );
    sql::bindValue(q, ":F_TINYINT       ", 2     );
    sql::bindValue(q, ":F_VARBINARY_MAX ", QString("F_VARBINARY_MAX"));
    sql::bindValue(q, ":F_NVARCHAR_MAX  ", QString(u8"Тестовая строка на русском языке. Продолжение тестовой строки"));

    if (q.exec())
    {
        log_info << "--- Insert-query 2 exec success ---";
    }

    if (transact->commit())
    {
        log_info << "--- Insert-query 2 transact success ---";
    }
}

void insertThread(db::mssql::Transaction::Ptr transact, int index)
{
    QSqlQuery q {db::mssql::createResult(transact)};

    q.prepare("INSERT               "
              "  TABLE1             "
              " (                   "
              "  [F_GUID]           "
              " ,[F_BIGINT]         "
              " ,[F_BINARY]         "
              " ,[F_BOOL]           "
              " ,[F_CHAR]           "
              " ,[F_NCHAR]          "
              " ,[F_TIME]           "
              " ,[F_DATE]           "
              " ,[F_DATETIME]       "
              " ,[F_DATETIME2]      "
              " ,[F_DATETIMEOFFSET] "
            //" ,[F_DECIMAL]        " // Тип не поддерживается
              " ,[F_FLOAT]          "
              " ,[F_REAL]           "
              " ,[F_INT]            "
            //" ,[F_NUMERIC]        " // Тип не поддерживается
            //" ,[F_SMALLDATETIME]  " // Тип не поддерживается
              " ,[F_SMALLINT]       "
              " ,[F_TINYINT]        "
              " ,[F_VARBINARY_MAX]  "
              " ,[F_NVARCHAR_MAX]   "
              " )                   "
              " VALUES              "
              " (                   "
              "  :F_GUID            "
              " ,:F_BIGINT          "
              " ,:F_BINARY          "
              " ,:F_BOOL            "
              " ,:F_CHAR            "
              " ,:F_NCHAR           "
              " ,:F_DATE            "
              " ,:F_TIME            "
              " ,:F_DATETIME        "
              " ,:F_DATETIME2       "
              " ,:F_DATETIMEOFFSET  "
            //" ,:F_DECIMAL         "
              " ,:F_FLOAT           "
              " ,:F_REAL            "
              " ,:F_INT             "
            //" ,:F_NUMERIC         "
            //" ,:F_SMALLDATETIME   "
              " ,:F_SMALLINT        "
              " ,:F_TINYINT         "
              " ,:F_VARBINARY_MAX   "
              " ,:F_NVARCHAR_MAX    "
              " );                  ");

    QDateTime dtime = QDateTime::currentDateTime();

    for (int i = 0; i < 1000; ++i)
    {
        sql::bindValue(q, ":F_GUID          ", QUuidEx().createUuid());
        sql::bindValue(q, ":F_BIGINT        ", index);
        sql::bindValue(q, ":F_BINARY        ", QByteArray("F_BINARY F_BINARY F_BINARY F_BINARY F_BINARY F_BINARY F_BINARY F_BINARY F_BINARY F_BINARY F_BINARY"));
        sql::bindValue(q, ":F_BOOL          ", true  );
        sql::bindValue(q, ":F_CHAR          ", 0x57  );
        sql::bindValue(q, ":F_NCHAR         ", 0x89  );
        sql::bindValue(q, ":F_TIME          ", dtime );
        sql::bindValue(q, ":F_DATE          ", dtime );
        sql::bindValue(q, ":F_DATETIME      ", dtime );
        sql::bindValue(q, ":F_DATETIME2     ", dtime );
        sql::bindValue(q, ":F_DATETIMEOFFSET", dtime );
      //sql::bindValue(q, ":F_DECIMAL       ", 12.34 ); // Тип не поддерживается
        sql::bindValue(q, ":F_FLOAT         ", 23.45 );
        sql::bindValue(q, ":F_REAL          ", 56.78 );
        sql::bindValue(q, ":F_INT           ", 567   );
      //sql::bindValue(q, ":F_NUMERIC       ", 34.56 ); // Тип не поддерживается
      //sql::bindValue(q, ":F_SMALLDATETIME ", dtime ); // Тип не поддерживается
        sql::bindValue(q, ":F_SMALLINT      ", 34    );
        sql::bindValue(q, ":F_TINYINT       ", 2     );
        sql::bindValue(q, ":F_VARBINARY_MAX ", QString("F_VARBINARY_MAX"));
        sql::bindValue(q, ":F_NVARCHAR_MAX  ", QString(u8"Тестовая строка на русском языке. Продолжение тестовой строки"));

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

void MainWindow::on_btnInsert4_clicked(bool)
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
              " ,[F_BOOL]           "
              " ,[F_CHAR]           "
              " ,[F_NCHAR]          "
              " ,[F_TIME]           "
              " ,[F_DATE]           "
              " ,[F_DATETIME]       "
              " ,[F_DATETIME2]      "
              " ,[F_DATETIMEOFFSET] "
            //" ,[F_DECIMAL]        " // Тип не поддерживается
              " ,[F_FLOAT]          "
              " ,[F_REAL]           "
              " ,[F_INT]            "
            //" ,[F_NUMERIC]        " // Тип не поддерживается
            //" ,[F_SMALLDATETIME]  " // Тип не поддерживается
              " ,[F_SMALLINT]       "
              " ,[F_TINYINT]        "
              " ,[F_VARBINARY_MAX]  "
              " ,[F_NVARCHAR_MAX]   "
              " )                   "
              " VALUES              "
              " (                   "
              "  :F_GUID            "
              " ,:F_BIGINT          "
              " ,:F_BINARY          "
              " ,:F_BOOL            "
              " ,:F_CHAR            "
              " ,:F_NCHAR           "
              " ,:F_DATE            "
              " ,:F_TIME            "
              " ,:F_DATETIME        "
              " ,:F_DATETIME2       "
              " ,:F_DATETIMEOFFSET  "
            //" ,:F_DECIMAL         "
              " ,:F_FLOAT           "
              " ,:F_REAL            "
              " ,:F_INT             "
            //" ,:F_NUMERIC         "
            //" ,:F_SMALLDATETIME   "
              " ,:F_SMALLINT        "
              " ,:F_TINYINT         "
              " ,:F_VARBINARY_MAX   "
              " ,:F_NVARCHAR_MAX    "
              " );                  ");

    QUuidEx uuid = QUuidEx().createUuid();

    sql::bindValue(q, ":F_GUID          ", uuid);
    sql::bindValue(q, ":F_BIGINT        ", QVariant(QVariant::Int));
    sql::bindValue(q, ":F_BINARY        ", QVariant(QVariant::ByteArray));
    sql::bindValue(q, ":F_BOOL          ", QVariant(QVariant::Bool));
    sql::bindValue(q, ":F_CHAR          ", QVariant(QVariant::Char));
    sql::bindValue(q, ":F_NCHAR         ", QVariant(QVariant::Char));
    sql::bindValue(q, ":F_TIME          ", QVariant(QVariant::Time));
    sql::bindValue(q, ":F_DATE          ", QVariant(QVariant::Date));
    sql::bindValue(q, ":F_DATETIME      ", QVariant(QVariant::DateTime));
    sql::bindValue(q, ":F_DATETIME2     ", QVariant(QVariant::DateTime));
    sql::bindValue(q, ":F_DATETIMEOFFSET", QVariant(QVariant::DateTime));
  //sql::bindValue(q, ":F_DECIMAL       ", 12.34 ); // Тип не поддерживается
    sql::bindValue(q, ":F_FLOAT         ", QVariant(QVariant::Double));
    sql::bindValue(q, ":F_REAL          ", QVariant(QVariant::Double));
    sql::bindValue(q, ":F_INT           ", QVariant(QVariant::Int));
  //sql::bindValue(q, ":F_NUMERIC       ", 34.56 ); // Тип не поддерживается
  //sql::bindValue(q, ":F_SMALLDATETIME ", dtime ); // Тип не поддерживается
    sql::bindValue(q, ":F_SMALLINT      ", QVariant(QVariant::Int));
    sql::bindValue(q, ":F_TINYINT       ", QVariant(QVariant::Int));
    sql::bindValue(q, ":F_VARBINARY_MAX ", QVariant(QVariant::ByteArray));
    sql::bindValue(q, ":F_NVARCHAR_MAX  ", QVariant(QVariant::String));

    if (q.exec())
    {
        log_info << "--- Insert-query 4 exec success ---";
    }
}

void MainWindow::on_btnDeleteAll_clicked(bool)
{
    _queryModel.setQuery(QSqlQuery());

    db::mssql::Driver::Ptr dbcon = mspool().connect();
    QSqlQuery q {dbcon->createResult()};

    if (sql::exec(q, "delete from table1"))
    {
        log_info << "--- Delete-query exec success ---";
    }
}

void MainWindow::on_btnAbortQuery_clicked(bool)
{
    mspool().abortOperations();
}

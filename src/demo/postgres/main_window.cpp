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
#include "database/postgres_pool.h"

#include <unistd.h>
#include <thread>

using namespace db::postgres;

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
            pgpool().abortOperation(tid);
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
    log_debug << "Call db::postgres::pool().connect()";
    _connect = db::postgres::pool().connect();
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

    db::postgres::Driver::Ptr dbcon = pgpool().connect();
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

    db::postgres::Driver::Ptr dbcon = pgpool().connect();
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

    db::postgres::Driver::Ptr dbcon = pgpool().connect();
    QSqlQuery q {dbcon->createResult()};

    q.setForwardOnly(false);

    if (!q.prepare("SELECT * FROM TABLE1"))
        return;

    if (q.exec())
    {
        log_info << "--- Select-query 1 exec success ---";
    }

    log_info << "Select-query 1 records count: "
             << db::postgres::resultSize(q, pgpool().connect());

    _queryModel.setQuery(q);

    log_info << "NumRowsAffected: " << q.numRowsAffected();
}

void MainWindow::on_btnSelect2_clicked(bool)
{
    _queryModel.setQuery(QSqlQuery());
    //_queryModel.query().clear();

    db::postgres::Driver::Ptr dbcon = pgpool().connect();
    QSqlQuery q {dbcon->createResult()};

    q.setForwardOnly(false);

    if (sql::exec(q,
        " SELECT        "
        "   ID          "
        "  ,F_BOOL      "
        "  ,F_INT       "
        "  ,F_INT64     "
        "  ,F_ENUM      "
        "  ,F_FLOAT     "
        "  ,F_DATE      "
        "  ,F_TIME      "
        "  ,F_DATETIME  "
        "  ,F_STRING    "
        " FROM          "
        "   TABLE1      "
        " LIMIT ?       ", 10))
    {
        log_info << "--- Select-query 2 exec success ---";
    }

    log_info << "Select-query 2 records count: "
             << db::postgres::resultSize(q, pgpool().connect());

    _queryModel.setQuery(q);

    log_info << "NumRowsAffected: " << q.numRowsAffected();
}

void MainWindow::on_btnSelect3_clicked(bool)
{
    _queryModel.setQuery(QSqlQuery());

    db::postgres::Driver::Ptr dbcon = pgpool().connect();
    QSqlQuery q {dbcon->createResult()};

    q.setForwardOnly(false);

    if (sql::exec(q,
        " SELECT         "
        "   ID           "
        "  ,F_BOOL       "
        "  ,F_INT        "
        "  ,F_INT64      "
        "  ,F_ENUM       "
        "  ,F_FLOAT      "
        "  ,F_DATE       "
        "  ,F_TIME       "
        "  ,F_DATETIME   "
        "  ,F_STRING     "
        "  ,F_ARR_INT    "
        "  ,F_ARR_UUID   "
        "  ,F_ARR_FLOAT  "
        "  ,F_ARR_DOUBLE "
        " FROM           "
        "   TABLE1       "
        " LIMIT 5        "))
    {
        log_info << "--- Select-query 3 exec success ---";
    }

    while (q.next())
    {
        QSqlRecord r = q.record();

        QUuidEx    id;
        bool       f_bool;
        qint32     f_int;
        qint64     f_int64;
        InsertMode f_enum;
        float      f_float;
        QDate      f_date;
        QTime      f_time;
        QDateTime  f_datetime;
        QString    f_string;
        QVector<qint32> f_arr_int;
        QVector<QUuidEx> f_arr_uuid;
        QVector<float> f_arr_float;
        QVector<double> f_arr_double;

        sql::assignValue(id          , r, "ID           ");
        sql::assignValue(f_bool      , r, "F_BOOL       ");
        sql::assignValue(f_int       , r, "F_INT        ");
        sql::assignValue(f_int64     , r, "F_INT64      ");
        sql::assignValue(f_enum      , r, "F_ENUM       ");
        sql::assignValue(f_float     , r, "F_FLOAT      ");
        sql::assignValue(f_date      , r, "F_DATE       ");
        sql::assignValue(f_time      , r, "F_TIME       ");
        sql::assignValue(f_datetime  , r, "F_DATETIME   ");
        sql::assignValue(f_string    , r, "F_STRING     ");
        sql::assignValue(f_arr_int   , r, "F_ARR_INT    ");
        sql::assignValue(f_arr_uuid  , r, "F_ARR_UUID   ");
        sql::assignValue(f_arr_float , r, "F_ARR_FLOAT  ");
        sql::assignValue(f_arr_double, r, "F_ARR_DOUBLE ");

        log_debug << "ID           " << id        ;
        log_debug << "F_BOOL       " << f_bool    ;
        log_debug << "F_INT        " << f_int     ;
        log_debug << "F_INT64      " << f_int64   ;
        log_debug << "F_ENUM       " << f_enum    ;
        log_debug << "F_FLOAT      " << f_float   ;
        log_debug << "F_DATE       " << f_date    ;
        log_debug << "F_TIME       " << f_time    ;
        log_debug << "F_DATETIME   " << f_datetime;
        log_debug << "F_STRING     " << f_string  ;
        log_debug << "F_ARR_INT    " << QVariant::fromValue(f_arr_int);
        log_debug << "F_ARR_UUID   " << QVariant::fromValue(f_arr_uuid);
        log_debug << "F_ARR_FLOAT  " << QVariant::fromValue(f_arr_float);
        log_debug << "F_ARR_DOUBLE " << QVariant::fromValue(f_arr_double);
        log_debug << "-----------";
    }
}

void MainWindow::on_btnInsAndSelect4_clicked(bool)
{
    //_queryModel.setQuery(QSqlQuery());

    QUuidEx id = QUuidEx::createUuid();

    db::postgres::Driver::Ptr dbcon = pgpool().connect();
    QSqlQuery q {dbcon->createResult()};

    QString fields =
        " ID          "
        ",F_BOOL      "
        ",F_INT       "
        ",F_INT64     "
        ",F_ENUM      "
        ",F_FLOAT     "
        ",F_DATE      "
        ",F_TIME      "
        ",F_DATETIME  ";

    QString sql = sql::INSERT_OR_UPDATE_PG("TABLE1", fields, "ID");

    if (!q.prepare(sql))
        return;

    QDateTime dtime = QDateTime::currentDateTime();

    sql::bindValue(q, ":ID          " , id                 );
    sql::bindValue(q, ":F_BOOL      " , true               );
    sql::bindValue(q, ":F_INT       " , 789                );
    sql::bindValue(q, ":F_INT64     " , -5064              );
    sql::bindValue(q, ":F_ENUM      " , InsertMode::Single );
    sql::bindValue(q, ":F_FLOAT     " , 0.56               );
    sql::bindValue(q, ":F_DATE      " , dtime              );
    sql::bindValue(q, ":F_TIME      " , dtime              );
    sql::bindValue(q, ":F_DATETIME  " , dtime              );

    if (!q.exec())
        return;

    db::postgres::Driver::Ptr dbcon2 = pgpool().connect();
    QSqlQuery q2 {dbcon2->createResult()};

    q2.setForwardOnly(false);

    if (!sql::exec(q2, "SELECT * FROM TABLE1 WHERE ID = $1", id))
        return;

    _queryModel.setQuery(q2);

    log_info << "--- Insert & Select-query 4 exec success ---";

    log_info << "Select-query 4 records count: "
             << db::postgres::resultSize(q2, pgpool().connect());

    log_info << "NumRowsAffected: " << q2.numRowsAffected();
}

void MainWindow::on_btnUpdate1_clicked(bool)
{
    _queryModel.setQuery(QSqlQuery());

    db::postgres::Driver::Ptr dbcon = pgpool().connect();
    QSqlQuery q {dbcon->createResult()};

    if (sql::exec(q, "UPDATE TABLE1 SET F_FLOAT = F_FLOAT*2"))
    {
        log_info << "--- Update-query 1 exec success ---";
    }
}

void MainWindow::on_btnUpdate2_clicked(bool)
{
    _queryModel.setQuery(QSqlQuery());

    db::postgres::Driver::Ptr dbcon = pgpool().connect();
    QSqlQuery q {dbcon->createResult()};

    QString fields =
        " ID          "
        ",F_BOOL      "
        ",F_INT       "
        ",F_INT64     "
        ",F_UINT64    "
        ",F_ENUM      "
        ",F_FLOAT     "
        ",F_DOUBLE    "
        ",F_DATE      "
        ",F_TIME      "
        ",F_DATETIME  "
        ",F_BYTEARRAY "
        ",F_STRING    "
        ",F_UUID      "
        ",F_ARR_INT   "
        ",F_ARR_UUID  ";

    QString sql = sql::INSERT_OR_UPDATE_PG("TABLE1", fields, "ID");

    if (!q.prepare(sql))
        return;

    QUuidEx id {"e7da463a-e96d-43e0-baeb-99278a1845ee"};
    QDateTime dtime = QDateTime::currentDateTime();

    sql::bindValue(q, ":ID          " , id                  );
    sql::bindValue(q, ":F_BOOL      " , false               );
    sql::bindValue(q, ":F_INT       " , 0                   );
    sql::bindValue(q, ":F_INT64     " , 0                   );
    sql::bindValue(q, ":F_UINT64    " , 0                   );
    sql::bindValue(q, ":F_ENUM      " , InsertMode::Unknown );
    sql::bindValue(q, ":F_FLOAT     " , -1                  );
    sql::bindValue(q, ":F_DATE      " , dtime               );
    sql::bindValue(q, ":F_TIME      " , dtime               );
    sql::bindValue(q, ":F_DATETIME  " , dtime               );

    if (q.exec())
    {
        log_info << "--- Update-query 2 exec success ---";
    }
}

void MainWindow::on_btnInsert1_clicked(bool)
{
    _queryModel.setQuery(QSqlQuery());

    db::postgres::Driver::Ptr dbcon = pgpool().connect();
    QSqlQuery q {dbcon->createResult()};

    QString sql =
        " INSERT INTO TABLE1 (                      "
        "   ID                                      "
        "  ,F_BOOL                                  "
        "  ,F_INT                                   "
        "  ,F_INT64                                 "
        "  ,F_UINT64                                "
        "  ,F_ENUM                                  "
        "  ,F_FLOAT                                 "
        "  ,F_DOUBLE                                "
        "  ,F_DATE                                  "
        "  ,F_TIME                                  "
        "  ,F_DATETIME                              "
        "  ,F_BYTEARRAY                             "
        "  ,F_STRING                                "
        "  ,F_UUID                                  "
        "  ,F_ARR_INT                               "
        "  ,F_ARR_UUID                              "
        "  ,F_ARR_FLOAT                             "
        "  ,F_ARR_DOUBLE                            "
        " ) VALUES (                                "
        "   'e7da463a-e96d-43e0-baeb-99278a1845ee'  "
        "  ,true                                    "
        "  ,123                                     "
        "  ,123456                                  "
        "  ,:F_UINT64                               "
        "  ,:F_ENUM                                 "
        "  ,0.25                                    "
        "  ,0.25635                                 "
        "  ,'1975-10-10'                            "
        "  ,'01:30:00'                              "
        "  ,'1975-10-10 01:30:00'                   "
        "  ,:F_BYTEARRAY                            "
        "  ,'STRING TEXT'                           "
        "  ,:F_UUID                                 "
        "  ,:F_ARR_INT                              "
        "  ,:F_ARR_UUID                             "
        "  ,:F_ARR_FLOAT                            "
        "  ,:F_ARR_DOUBLE                           "
        " )                                         ";

    if (!q.prepare(sql))
        return;

    quint64 f_uint64 = 123456;
    QByteArray f_bytearray {"\x31\x32\x33\x34", 4};
    QUuidEx f_uuid {"8e87196a-33d1-4771-9847-ec8703edc46b"};
    QVector<qint32> f_arr_int {1, 2, 3, 4, 5};

    QVector<QUuidEx> f_arr_uuid;
    f_arr_uuid.append(QUuidEx("c9489d48-ca11-49db-9da0-6e2e14c3f105"));
    f_arr_uuid.append(QUuidEx("1e8b4002-ebe0-4ad9-8c2d-607b61d76b61"));
    f_arr_uuid.append(QUuidEx("97e724e5-89d3-44fb-a207-a6a43dcd7359"));

    QVector<float> f_arr_float;
    f_arr_float.append(0.56);
    f_arr_float.append(7.89);
    f_arr_float.append(15.336);
    f_arr_float.append(22.082);

    QVector<double> f_arr_double;
    f_arr_double.append(0.35);
    f_arr_double.append(8.46);
    f_arr_double.append(10.214);

    sql::bindValue(q, ":F_UINT64     " , f_uint64           );
    sql::bindValue(q, ":F_ENUM       " , InsertMode::Single );
    sql::bindValue(q, ":F_BYTEARRAY  " , f_bytearray        );
    sql::bindValue(q, ":F_UUID       " , f_uuid             );
    sql::bindValue(q, ":F_ARR_INT    " , f_arr_int          );
    sql::bindValue(q, ":F_ARR_UUID   " , f_arr_uuid         );
    sql::bindValue(q, ":F_ARR_FLOAT  " , f_arr_float        );
    sql::bindValue(q, ":F_ARR_DOUBLE " , f_arr_double       );

    if (q.exec())
    {
        log_info << "--- Insert-query 1 exec success ---";
    }
}

void MainWindow::on_btnInsert2_clicked(bool)
{
    _queryModel.setQuery(QSqlQuery());

    db::postgres::Driver::Ptr dbcon = pgpool().connect();
    db::postgres::Transaction::Ptr transact = dbcon->createTransact();
    QSqlQuery q {db::postgres::createResult(transact)};

    if (!transact->begin())
        return;

    QString fields =
        " ID          "
        ",F_BOOL      "
        ",F_INT       "
        ",F_INT64     "
        ",F_UINT64    "
        ",F_ENUM      "
        ",F_FLOAT     "
        ",F_DOUBLE    "
        ",F_DATE      "
        ",F_TIME      "
        ",F_DATETIME  "
        ",F_BYTEARRAY "
        ",F_STRING    "
        ",F_UUID      "
        ",F_ARR_INT   "
        ",F_ARR_UUID  ";

    QString sql = sql::INSERT_OR_UPDATE_PG("TABLE1", fields, "ID");

    if (!q.prepare(sql))
        return;

    QDateTime dtime = QDateTime::currentDateTime();
    QByteArray f_bytearray {"\x31\x32\x33\x34", 4};

    QUuidEx f_uuid = QUuidEx::createUuid();
    QVector<qint32> f_arr_int {1, 2, 3, 4};

    QVector<QUuidEx> f_arr_uuid;
    f_arr_uuid.append(QUuidEx::createUuid());
    f_arr_uuid.append(QUuidEx::createUuid());

    sql::bindValue(q, ":ID          " , QUuidEx::createUuid() );
    sql::bindValue(q, ":F_BOOL      " , true                  );
    sql::bindValue(q, ":F_INT       " , 1234                  );
    sql::bindValue(q, ":F_INT64     " , 123456                );
    sql::bindValue(q, ":F_UINT64    " , quint64(123456)       );
    sql::bindValue(q, ":F_ENUM      " , InsertMode::Multi     );
    sql::bindValue(q, ":F_FLOAT     " , 0.35                  );
    sql::bindValue(q, ":F_DOUBLE    " , 1.35750               );
    sql::bindValue(q, ":F_DATE      " , dtime                 );
    sql::bindValue(q, ":F_TIME      " , dtime                 );
    sql::bindValue(q, ":F_DATETIME  " , dtime                 );
    sql::bindValue(q, ":F_BYTEARRAY " , f_bytearray           );
    sql::bindValue(q, ":F_STRING    " , QString("TEXT 2")     );
    sql::bindValue(q, ":F_UUID      " , f_uuid                );
    sql::bindValue(q, ":F_ARR_INT   " , f_arr_int             );
    sql::bindValue(q, ":F_ARR_UUID  " , f_arr_uuid            );

    if (!q.exec())
        return;

    QVector<qint32> f_arr_int2 {5, 6, 7, 8};

    sql::bindValue(q, ":ID          " , QUuidEx::createUuid() );
    sql::bindValue(q, ":F_BOOL      " , false                 );
    sql::bindValue(q, ":F_INT       " , -5678                 );
    sql::bindValue(q, ":F_INT64     " , -56789                );
    sql::bindValue(q, ":F_UINT64    " , quint64(-56789)       );
    sql::bindValue(q, ":F_ENUM      " , InsertMode::Multi     );
    sql::bindValue(q, ":F_FLOAT     " , 0.78                  );
    sql::bindValue(q, ":F_DOUBLE    " , 2.789525              );
    sql::bindValue(q, ":F_DATE      " , dtime                 );
    sql::bindValue(q, ":F_TIME      " , dtime                 );
    sql::bindValue(q, ":F_DATETIME  " , dtime                 );
    sql::bindValue(q, ":F_BYTEARRAY " , QVariant(QVariant::ByteArray));
    sql::bindValue(q, ":F_STRING    " , QVariant(QVariant::String));
    sql::bindValue(q, ":F_UUID      " , f_uuid                );
    sql::bindValue(q, ":F_ARR_INT   " , f_arr_int2            );
  //sql::bindValue(q, ":F_ARR_UUID  " , f_arr_uuid            );

    if (!q.exec())
        return;

    if (transact->commit())
    {
        log_info << "--- Insert-query 2 exec success ---";
    }
}

void insertThread(db::postgres::Transaction::Ptr transact, int index)
{
    QSqlQuery q {db::postgres::createResult(transact)};

    QString fields =
        " ID          "
        ",F_BOOL      "
        ",F_INT       "
        ",F_INT64     "
        ",F_UINT64    "
        ",F_ENUM      "
        ",F_FLOAT     "
        ",F_DOUBLE    "
        ",F_DATE      "
        ",F_TIME      "
        ",F_DATETIME  "
        ",F_BYTEARRAY "
        ",F_STRING    "
        ",F_UUID      "
        ",F_ARR_INT   "
        ",F_ARR_UUID  ";

    QString sql = sql::INSERT_OR_UPDATE_PG("TABLE1", fields, "ID");

    if (!q.prepare(sql))
        return;

    QVector<qint32> f_arr_int {1, 2, 3, 4};

    QByteArray f_bytearray;
    if (index)
        f_bytearray = QByteArray{"\x31\x32\x33\x34", 4};

    for (int i = 0; i < 100; ++i)
    {
        QUuidEx f_uuid = QUuidEx::createUuid();
        QDateTime dtime = QDateTime::currentDateTime();

        QVector<QUuidEx> f_arr_uuid;
        f_arr_uuid.append(QUuidEx::createUuid());
        f_arr_uuid.append(QUuidEx::createUuid());

        sql::bindValue(q, ":ID          " , QUuidEx::createUuid() );
        sql::bindValue(q, ":F_BOOL      " , index                 );
        sql::bindValue(q, ":F_INT       " , i                     );
        sql::bindValue(q, ":F_INT64     " , trd::gettid()         );
        sql::bindValue(q, ":F_UINT64    " , quint64(123456)       );
        sql::bindValue(q, ":F_ENUM      " , (index == 0) ? InsertMode::Thread1 : InsertMode::Thread2 );
        sql::bindValue(q, ":F_FLOAT     " , 0.455                 );
        sql::bindValue(q, ":F_DOUBLE    " , 2.563                 );
        sql::bindValue(q, ":F_DATE      " , dtime                 );
        sql::bindValue(q, ":F_TIME      " , dtime                 );
        sql::bindValue(q, ":F_DATETIME  " , dtime                 );
        sql::bindValue(q, ":F_BYTEARRAY " , f_bytearray           );
        sql::bindValue(q, ":F_STRING    " , QString("TEXT 3")     );
        sql::bindValue(q, ":F_UUID      " , f_uuid                );
        sql::bindValue(q, ":F_ARR_INT   " , f_arr_int             );
        sql::bindValue(q, ":F_ARR_UUID  " , f_arr_uuid            );

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

        db::postgres::Driver::Ptr dbcon = pgpool().connect();
        db::postgres::Transaction::Ptr transact = dbcon->createTransact();

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

    db::postgres::Driver::Ptr dbcon = pgpool().connect();
    QSqlQuery q {dbcon->createResult()};

    if (sql::exec(q, "delete from table1"))
    {
        log_info << "--- Delete-query exec success ---";
    }
}

void MainWindow::on_btnAbortQuery_clicked(bool)
{
    pgpool().abortOperations();
}

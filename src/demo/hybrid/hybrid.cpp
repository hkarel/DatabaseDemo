#include "main_window.h"

#include "shared/container_ptr.h"
#include "shared/utils.h"
#include "shared/steady_timer.h"
#include "shared/logger/logger.h"
#include "shared/logger/config.h"
#include "shared/logger/format.h"
#include "shared/config/appl_conf.h"
#include "shared/config/logger_conf.h"
#include "shared/qt/logger_operators.h"
#include "shared/qt/version_number.h"

#include "database/mssql_pool.h"
#include "database/postgres_pool.h"

#include <QApplication>
#include <stdlib.h>
#include <unistd.h>

#define APPLICATION_NAME "Mssql Demo"

using namespace std;

void stopLog()
{
    alog::logger().flush();
    alog::logger().waitingFlush();
    alog::logger().stop();
}

void stopProgram()
{
    #define STOP_THREAD(THREAD_FUNC, NAME, TIMEOUT) \
        if (!THREAD_FUNC.stop(TIMEOUT * 1000)) { \
            log_info << "Thread '" NAME "': Timeout expired, thread will be terminated"; \
            THREAD_FUNC.terminate(); \
        }

//    STOP_THREAD(video::transport::sendPool(),  "VideoSendPool",     20)
//    STOP_THREAD(video::transport::recvPool(),  "VideoRecvPool",     20)
//    STOP_THREAD(video::jitter(),               "VideoJitter",       15)
//    STOP_THREAD(video::capture(),              "VideoCapture0",     15)

    #undef STOP_THREAD

    db::mssql::pool().close();

    log_info << log_format("'%?' is stopped", APPLICATION_NAME);
    stopLog();
}


int main(int argc, char *argv[])
{
//    // Тест скорости доступа к полю структуры 'A'
//    // при использовании shared_ptr и container_ptr
//    struct A {int b;};
//    steady_timer timer;
//    std::shared_ptr<A> a1 {new A};
//    container_ptr<A>   a2 {new A};
//    qint32 tm1, tm2;

//    for (int k = 0; k < 10; ++k)
//    {
//        std::srand(std::time(0));

//        timer.reset();
//        for (int i = 0; i < 10*1000000; ++i)
//        {
//            a1->b = std::rand();
//        }
//        (void) a1->b;
//        tm1 = timer.elapsed<std::chrono::microseconds>();

//        timer.reset();
//        for (int i = 0; i < 10*1000000; ++i)
//        {
//            a2->b = std::rand();
//        }
//        (void) a2->b;
//        tm2 = timer.elapsed<std::chrono::microseconds>();

//        int persent = (1.0 - float(tm1) / tm2) * 100;
//        qWarning("%d  %d  %d", tm1, tm2, persent);
//    }
//    return 0;


    // Устанавливаем в качестве разделителя целой и дробной части символ '.',
    // если этого не сделать - функции преобразования строк в числа (std::atof)
    // буду неправильно работать.
    qputenv("LC_NUMERIC", "C");

    int ret = 0;
    try
    {
        alog::logger().start();

#ifdef NDEBUG
        alog::logger().addSaverStdOut(alog::Level::Info, true);
#else
        alog::logger().addSaverStdOut(alog::Level::Debug2);
#endif

        // Путь к основному конфиг-файлу
        QString configFile = config::qdir() + "/hybrid.conf";
        if (QFile::exists(configFile))
        {
            if (!config::base().readFile(configFile.toStdString()))
            {
                stopLog();
                return 1;
            }
        }
        else
        {
            QFile file;
            QByteArray conf;

            // Создаем демонстрационные конфиг-файлы
            file.setFileName("://hybrid.conf");
            file.open(QIODevice::ReadOnly);
            conf = file.readAll();

            if (!config::base().readString(conf.toStdString()))
            {
                stopLog();
                return 1;
            }
        }
        config::base().setReadOnly(true);
        config::base().setSaveDisabled(true);

        QString configFileS;
#ifdef MINGW
        config::base().getValue("state.file_win", configFileS);
#else
        config::base().getValue("state.file", configFileS);
#endif
        config::dirExpansion(configFileS);
        config::state().readFile(configFileS.toStdString());

        // Создаем дефолтный сэйвер для логгера
        if (!alog::configDefaultSaver())
        {
            stopLog();
            return 1;
        }

        log_info << log_format(
            "'%?' is running (version: %?; gitrev: %?)",
            APPLICATION_NAME, productVersion().toString(), GIT_REVISION);
        alog::logger().flush();

        //alog::logger().removeSaverStdOut();
        //alog::logger().removeSaverStdErr();

        alog::printSaversInfo();

        QApplication appl {argc, argv};

        auto databaseInit1 = [](db::mssql::Driver::Ptr db) -> bool
        {
            QString hostAddress = "192.168.1.117";
            int port = 1435;
            QString user = "sa";
            QString password = "sa";
            QString name;
            QString options;

            YamlConfig::Func loadFunc = [&](YamlConfig* conf, YAML::Node& node, bool /*logWarn*/)
            {
                conf->getValue(node, "address", hostAddress);
                conf->getValue(node, "port", port);
                conf->getValue(node, "user", user);
                conf->getValue(node, "password", password);
                conf->getValue(node, "name", name);
                conf->getValue(node, "options", options);
                return true;
            };
            config::base().getValue("database_mssql", loadFunc);

            if (name.isEmpty())
            {
                log_error << "Database name is not defined";
                return false;
            }
            db->setWideChar(false);
            return db->open(name, user, password, hostAddress, port, options);
        };

        if (!db::mssql::pool().init(databaseInit1))
        {
//            QFile file;
//            QByteArray script;

//            // Выводим в лог скрипт для создания БД
//            file.setFileName("://create_database_mssql.sql");
//            file.open(QIODevice::ReadOnly);
//            script = file.readAll();

//            log_error << "Perhaps the database not exist"
//                      << ". See the script below for creating a database";
//            log_info  << script.toStdString();

            stopProgram();
            return 1;
        }
        db::mssql::pool().setSingleConnect(false);

        auto databaseInit2 = [](db::postgres::Driver::Ptr db) -> bool
        {
            QString hostAddress = "127.0.0.1";
            int port = 5432;
            QString user = "postgres";
            QString password = "postgres";
            QString name;
            QString options;

            YamlConfig::Func loadFunc = [&](YamlConfig* conf, YAML::Node& node, bool /*logWarn*/)
            {
                conf->getValue(node, "address", hostAddress);
                conf->getValue(node, "port", port);
                conf->getValue(node, "user", user);
                conf->getValue(node, "password", password);
                conf->getValue(node, "name", name);
                conf->getValue(node, "options", options);
                return true;
            };
            config::base().getValue("database_postgresql", loadFunc);

            if (name.isEmpty())
            {
                log_error << "Database name is not defined";
                return false;
            }
            return db->open("yamz-demo", "postgres", "postgres", "127.0.0.1", 5432, "");
        };

        if (!db::postgres::pool().init(databaseInit2))
        {
            stopProgram();
            return 1;
        }

        MainWindow mw;
        mw.init();
        mw.loadGeometry();
        mw.show();

        ret = appl.exec();

        mw.saveGeometry();
        mw.deinit();

        config::state().saveFile();
    }
    catch (YAML::ParserException& e)
    {
        log_error << "YAML error. Detail: " << e.what();
    }
    catch (std::exception& e)
    {
        log_error << "Failed initialization. Detail: " << e.what();
        ret = 1;
    }
    catch (...)
    {
        log_error << "Failed initialization. Unknown error";
        ret = 1;
    }

    stopProgram();
    return ret;
}

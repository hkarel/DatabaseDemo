import qbs
import QbsUtl

Product {
    name: "Hybrid"
    targetName: "db-demo-hybrid"
    condition: true

    type: "application"
    destinationDirectory: "./bin"

    Depends { name: "cpp" }
    Depends { name: "Database" }
    //Depends { name: "Mssql" }
    //Depends { name: "Postgres" }
    Depends { name: "SharedLib" }
    Depends { name: "Yaml" }
    Depends { name: "odbc" }
    Depends { name: "Qt"; submodules: ["core", "widgets", "sql"] }

    cpp.defines:  project.cppDefines
    cpp.cxxFlags: project.cxxFlags
    cpp.cxxLanguageVersion: project.cxxLanguageVersion

    cpp.includePaths: [
        "./",
        "/usr/include/postgresql",
    ]

    cpp.systemIncludePaths: QbsUtl.concatPaths(
        Qt.core.cpp.includePaths // Декларация для подавления Qt warning-ов
    )

    cpp.rpaths: QbsUtl.concatPaths(
        "$ORIGIN/../lib"
    )

    cpp.dynamicLibraries: [
        "pthread",
        "pq",
    ]

    Group {
         name: "mssql"
         prefix: "../../database/database/"
         files: [
             "mssql_driver.cpp",
             "mssql_driver.h",
             "mssql_pool.cpp",
             "mssql_pool.h",
         ]
    }

    Group {
         name: "postgres"
         prefix: "../../database/database/"
         files: [
             "postgres_driver.cpp",
             "postgres_driver.h",
             "postgres_pool.cpp",
             "postgres_pool.h",
         ]
    }

    files: [
        "main_window.cpp",
        "main_window.h",
        "main_window.ui",
        "hybrid.cpp",
        "hybrid.qrc",
    ]
}

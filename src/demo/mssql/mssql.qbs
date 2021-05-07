import qbs
import QbsUtl

Product {
    name: "Mssql"
    targetName: "db-demo-mssql"
    condition: true

    type: "application"
    destinationDirectory: "./bin"

    Depends { name: "cpp" }
    Depends { name: "Database" }
    Depends { name: "SharedLib" }
    Depends { name: "Yaml" }
    Depends { name: "odbc" }
    Depends { name: "Qt"; submodules: ["core", "widgets", "sql"] }

    cpp.defines:  project.cppDefines
    cpp.cxxFlags: project.cxxFlags
    cpp.cxxLanguageVersion: project.cxxLanguageVersion

    cpp.includePaths: [
        "./",
    ]

    cpp.systemIncludePaths: QbsUtl.concatPaths(
        Qt.core.cpp.includePaths // Декларация для подавления Qt warning-ов
    )

    cpp.rpaths: QbsUtl.concatPaths(
        "$ORIGIN/../lib"
    )

    cpp.dynamicLibraries: [
        "pthread",
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

    files: [
        "main_window.cpp",
        "main_window.h",
        "main_window.ui",
        "mssql.cpp",
        "mssql.qrc",
    ]
}

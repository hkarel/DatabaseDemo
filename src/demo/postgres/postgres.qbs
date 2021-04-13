import qbs
import QbsUtl

Product {
    name: "Postgres"
    targetName: "db-demo-postgres"
    condition: true

    type: "application"
    destinationDirectory: "./bin"

    Depends { name: "cpp" }
    Depends { name: "cppstdlib" }
    Depends { name: "Database" }
    Depends { name: "SharedLib" }
    Depends { name: "Yaml" }
    Depends { name: "Qt"; submodules: ["core", "widgets", "sql"] }

    cpp.defines:  project.cppDefines
    cpp.cxxFlags: project.cxxFlags
    cpp.cxxLanguageVersion: project.cxxLanguageVersion

    cpp.includePaths: [
        "./",
        "/usr/include/postgresql",
    ]

    cpp.systemIncludePaths: QbsUtl.concatPaths(
        Qt.core.cpp.includePaths
        //lib.firebird.includePath
    )

    cpp.rpaths: QbsUtl.concatPaths(
        cppstdlib.path,
        //lib.firebird.libraryPath,
        "$ORIGIN/../lib"
    )

    cpp.dynamicLibraries: [
        "pthread",
        "pq",
    ]

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
        "postgres.cpp",
        "postgres.qrc",
    ]
}

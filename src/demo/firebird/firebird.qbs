import qbs
import QbsUtl

Product {
    name: "Firebird"
    targetName: "db-demo-firebird"
    condition: true

    type: "application"
    destinationDirectory: "./bin"

    Depends { name: "cpp" }
    Depends { name: "cppstdlib" }
    Depends { name: "Database" }
    Depends { name: "SharedLib" }
    Depends { name: "Yaml" }
    Depends { name: "lib.firebird" }
    Depends { name: "Qt"; submodules: ["core", "widgets", "sql"] }

    cpp.defines:  project.cppDefines
    cpp.cxxFlags: project.cxxFlags
    cpp.cxxLanguageVersion: project.cxxLanguageVersion

    cpp.includePaths: [
        "./",
    ]

    cpp.systemIncludePaths: QbsUtl.concatPaths(
        Qt.core.cpp.includePaths // Декларация для подавления Qt warning-ов
       ,lib.firebird.includePath
    )

    cpp.rpaths: QbsUtl.concatPaths(
        lib.firebird.libraryPath,
        "$ORIGIN/../lib"
    )

     cpp.libraryPaths: QbsUtl.concatPaths(
         lib.firebird.libraryPath
     )

    cpp.dynamicLibraries: [
        "pthread",
    ].concat(lib.firebird.dynamicLibraries)

    Group {
         name: "firebird"
         prefix: "../../database/database/"
         files: [
             "firebird_driver.cpp",
             "firebird_driver.h",
             "firebird_pool.cpp",
             "firebird_pool.h",
         ]
    }

    files: [
        "main_window.cpp",
        "main_window.h",
        "main_window.ui",
        "firebird.cpp",
        "firebird.qrc",
    ]
}

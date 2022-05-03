import qbs
import QbsUtl

Product {
    name: "Database"
    targetName: "database"

    type: "staticlibrary"

    Depends { name: "cpp" }
    Depends { name: "SharedLib" }
    Depends { name: "Qt"; submodules: ["core", "sql"] }

    cpp.defines: project.cppDefines
    cpp.cxxFlags: project.cxxFlags
    cpp.cxxLanguageVersion: project.cxxLanguageVersion

    property var includePaths: [
        "./",
        "./database",
    ]

    // Декларация для подавления Qt warning-ов
    cpp.systemIncludePaths: Qt.core.cpp.includePaths

    files: [
        "database/connect_pool.h",
        "database/qmetatypes.h",
        "database/sql_func.cpp",
        "database/sql_func.h",
        "database/sqlcachedresult.cpp",
        "database/sqlcachedresult.h",
    ]

    Export {
        Depends { name: "cpp" }
        cpp.includePaths: exportingProduct.includePaths
    }
}

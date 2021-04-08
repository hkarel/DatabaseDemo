import qbs
import "qbs/imports/QbsUtl/qbsutl.js" as QbsUtl

Project {
    name: "Db Demo (Progect)"

    minimumQbsVersion: "1.18.0"
    qbsSearchPaths: ["qbs"]

    property var cppDefines: {
        var def = [];

        if (qbs.buildVariant === "release")
            def.push("NDEBUG");

        return def;
    }

    property var cxxFlags: [
        "-ggdb3",
        "-Wall",
        "-Wextra",
        "-Wno-unused-parameter",
        "-Wno-variadic-macros",
        "-Wno-register",
    ]
    property string cxxLanguageVersion: "c++17"

    references: [
        "src/shared/shared.qbs",
        "src/database/database.qbs",
        "src/yaml/yaml.qbs",
    ]
}

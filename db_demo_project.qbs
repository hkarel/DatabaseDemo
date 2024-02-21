import qbs
import "db_demo_base.qbs" as DbDemoBase

DbDemoBase {
    name: "DatabaseDemo (Progect)"

    readonly property bool demo_firebird: true
    readonly property bool demo_postgres: true
    readonly property bool demo_mssql: true
    //readonly property bool demo_hybrid: true

    references: [
        "src/database/database.qbs",
        "src/demo/demo.qbs",
        "src/shared/shared.qbs",
        "src/yaml/yaml.qbs",
    ]
}

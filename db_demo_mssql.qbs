import qbs
import "db_demo_base.qbs" as DbDemoBase

DbDemoBase {
    name: "DatabaseDemo (Progect)"

    readonly property bool demo_firebird: false
    readonly property bool demo_postgres: false
    readonly property bool demo_mssql: true

    references: [
        "src/database/database.qbs",
        "src/demo/demo.qbs",
        "src/shared/shared.qbs",
        "src/yaml/yaml.qbs",
    ]
}

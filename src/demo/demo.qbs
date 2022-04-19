import qbs

Project {
    name: "Demo"
    references: {
        var ref = []
        if (project.demo_firebird === true)
            ref.push("firebird/firebird.qbs");

        if (project.demo_postgres === true)
            ref.push("postgres/postgres.qbs");

        if (project.demo_mssql === true)
            ref.push("mssql/mssql.qbs");

        if (project.demo_hybrid === true)
            ref.push("hybrid/hybrid.qbs");

        return ref;
    }
}

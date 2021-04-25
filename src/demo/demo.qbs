import qbs

Project {
    name: "Demo"
    references: {
        var ref = []
        if (project.demo_firebird === true)
            ref.push("firebird/firebird.qbs");

        if (project.demo_postgres === true)
            ref.push("postgres/postgres.qbs");

        return ref;
    }
}

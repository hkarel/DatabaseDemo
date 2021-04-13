import qbs
import "qbs/imports/QbsUtl/qbsutl.js" as QbsUtl

Project {
    name: "DatabaseDemo (Progect)"

    minimumQbsVersion: "1.18.0"
    qbsSearchPaths: ["qbs"]

    readonly property var projectVersion: projectProbe.projectVersion
  //readonly property var databaseVersion: projectProbe.databaseVersion
    readonly property string projectGitRevision: projectProbe.projectGitRevision

    Probe {
        id: projectProbe
        property var projectVersion;
        property var databaseVersion;
        property string projectGitRevision;

        readonly property string projectBuildDirectory:  project.buildDirectory
        readonly property string projectSourceDirectory: project.sourceDirectory

        configure: {
            projectVersion = QbsUtl.getVersions(projectSourceDirectory + "/VERSION");
            projectGitRevision = QbsUtl.gitRevision(projectSourceDirectory);

            //var fileVersionDb = projectSourceDirectory + "/VERSION_DB";
            //if (!File.exists(fileVersionDb))
            //    throw new Error(("File '{0}' not found").format(fileVersionDb));

            //var file = new TextFile(fileVersionDb, TextFile.ReadOnly);
            //try {
            //    databaseVersion = file.readLine().trim();
            //}
            //finally {
            //    file.close();
            //}
        }
    }

    property var cppDefines: {
        var def = [
            "VERSION_PROJECT=\"" + projectVersion[0] + "\"",
            "VERSION_PROJECT_MAJOR=" + projectVersion[1],
            "VERSION_PROJECT_MINOR=" + projectVersion[2],
            "VERSION_PROJECT_PATCH=" + projectVersion[3],
          //"VERSION_DATABASE=" + databaseVersion,
            "GIT_REVISION=\"" + projectGitRevision + "\"",
            "QDATASTREAM_VERSION=QDataStream::Qt_5_12",
            "LOGGER_LESS_SNPRINTF",
        ];

        if (qbs.buildVariant === "release")
            def.push("NDEBUG");

        if (qbs.targetOS.contains("windows")
            && qbs.toolchain && qbs.toolchain.contains("mingw"))
        {
            def.push("CONFIG_DIR=\"ProgramData/database-demo\"");
        }
        else
            def.push("CONFIG_DIR=\"/etc/database-demo\"");

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
        "src/database/database.qbs",
        "src/demo/demo.qbs",
        "src/shared/shared.qbs",
        "src/yaml/yaml.qbs",
    ]
}

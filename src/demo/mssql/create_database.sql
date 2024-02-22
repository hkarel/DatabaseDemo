
-- Database: database-demo

-- DROP DATABASE "database-demo";

-- DROP TABLE table1;

CREATE TABLE [database-demo].dbo.table1 (
    f_guid uniqueidentifier NULL,
    f_bigint bigint NULL,
    f_binary binary(1000) NULL,
    f_char char(200) COLLATE Latin1_General_100_CI_AS_SC_UTF8 NULL,
    f_nchar nchar(200) COLLATE Latin1_General_100_CI_AS_SC_UTF8 NULL,
    f_time time(0) NULL,
    f_date date NULL,
    f_datetime datetime NULL,
    f_datetime2 datetime2 NULL,
    f_datetimeoffset datetimeoffset NULL,
    f_float float NULL,
    f_real real NULL,
    f_int int NULL,
    f_smallint smallint NULL,
    f_tinyint tinyint NULL,
    f_varbinary_max varbinary(MAX) NULL,
    f_nvarchar_max nvarchar(MAX) COLLATE Latin1_General_100_CI_AS_SC_UTF8 NULL
);

CREATE TABLE [database-demo].dbo.table2 (
    f_guid uniqueidentifier NULL,
    f_bigint bigint NULL,
    f_binary binary(1000) NULL,
    f_char char(200) COLLATE Latin1_General_100_CI_AS_SC_UTF8 NULL,
    f_nchar nchar(200) COLLATE Latin1_General_100_CI_AS_SC_UTF8 NULL,
    f_time time(0) NULL,
    f_date date NULL,
    f_datetime datetime NULL,
    f_datetime2 datetime2 NULL,
    f_datetimeoffset datetimeoffset NULL,
    f_float float NULL,
    f_real real NULL,
    f_int int NULL,
    f_smallint smallint NULL,
    f_tinyint tinyint NULL,
    f_varbinary_max varbinary(MAX) NULL,
    f_nvarchar_max nvarchar(MAX) COLLATE Latin1_General_100_CI_AS_SC_UTF8 NULL
);

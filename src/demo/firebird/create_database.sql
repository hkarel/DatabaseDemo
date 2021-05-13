
sudo /opt/firebird/bin/isql

CREATE DATABASE '/var/opt/database-demo/database-demo.fdb' page_size 8192
user 'SYSDBA' password 'masterkey';

CREATE DOMAIN UUID AS CHAR(16) CHARACTER SET OCTETS COLLATE OCTETS;
CREATE DOMAIN BOOL AS SMALLINT DEFAULT 0 NOT NULL CHECK (VALUE BETWEEN 0 AND 1);

CREATE TABLE TABLE1 (
    ID           UUID NOT NULL /* UUID = CHAR(16) */,
    F_BOOL       BOOL /* BOOL = SMALLINT DEFAULT 0 NOT NULL CHECK (VALUE BETWEEN 0 AND 1) */,
    F_INT        INTEGER,
    F_INT64      BIGINT,
    F_UINT64     BIGINT,
    F_ENUM       INTEGER,
    F_FLOAT      FLOAT,
    F_DOUBLE     DOUBLE PRECISION,
    F_DATE       DATE,
    F_TIME       TIME,
    F_DATETIME   TIMESTAMP,
    F_BYTEARRAY  BLOB SUB_TYPE 0 SEGMENT SIZE 80,
    F_STRING     VARCHAR(100) CHARACTER SET UTF8,
    F_UUID       UUID /* UUID = CHAR(16) */
);

ALTER TABLE TABLE1 ADD CONSTRAINT PK_TABLE1 PRIMARY KEY (ID);

-- Database: yamz-demo

-- DROP DATABASE "yamz-demo";

CREATE DATABASE "yamz-demo"
    WITH
    OWNER = postgres
    ENCODING = 'UTF8'
    LC_COLLATE = 'ru_RU.UTF-8'
    LC_CTYPE = 'ru_RU.UTF-8'
    TABLESPACE = pg_default
    CONNECTION LIMIT = -1;

-- Table: public.motor_table

-- DROP TABLE public.motor_table;

CREATE TABLE public.motor_table
(
    timeline bigint,
    name text
)

TABLESPACE pg_default;

ALTER TABLE public.motor_table OWNER to postgres;

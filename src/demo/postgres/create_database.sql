
-- Database: database-demo

-- DROP DATABASE "database-demo";

CREATE DATABASE "database-demo"
    WITH
    OWNER = postgres
    ENCODING = 'UTF8'
    LC_COLLATE = 'ru_RU.UTF-8'
    LC_CTYPE = 'ru_RU.UTF-8'
    TABLESPACE = pg_default
    CONNECTION LIMIT = -1;

-- Table: public.table1

-- DROP TABLE public.table1;

CREATE TABLE public.table1
(
    id uuid NOT NULL,
    f_bool boolean,
    f_int integer,
    f_int64 bigint,
    f_uint64 bigint,
    f_enum integer,
    f_float real,
    f_double double precision,
    f_date date,
    f_time time without time zone,
    f_datetime timestamp without time zone,
    f_datetimetz timestamp with time zone,
    f_bytearray bytea,
    f_string text COLLATE pg_catalog."default",
    f_uuid uuid,
    f_arr_int integer[],
    f_arr_uuid uuid[],
    f_arr_float real[],
    f_arr_double double precision[],
    CONSTRAINT teble1_pkey PRIMARY KEY (id)
)

TABLESPACE pg_default;

ALTER TABLE public.table1 OWNER to postgres;

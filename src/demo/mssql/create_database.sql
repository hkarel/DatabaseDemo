
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

-- DROP TABLE table1;

SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

CREATE TABLE [dbo].[table1](
	[f_bigint] [bigint] NULL,
	[f_binary] [binary](50) NULL,
	[f_char] [char](10) NULL,
	[f_date] [date] NULL,
	[f_datetime] [datetime] NULL,
	[f_datetime2] [datetime2](7) NULL,
	[f_datetimeoffset] [datetimeoffset](7) NULL,
	[f_float] [float] NULL,
	[f_int] [int] NULL,
	[f_nchar] [nchar](10) NULL,
	[f_nvarchar_n] [nvarchar](50) NULL,
	[f_nvarchar_max] [nvarchar](max) NULL,
	[f_real] [real] NULL,
	[f_smalldatetime] [smalldatetime] NULL,
	[f_smallint] [smallint] NULL,
	[f_time] [time](7) NULL,
	[f_tinyint] [tinyint] NULL,
	[f_varbinary_n] [varbinary](50) NULL,
	[f_varbinary_max] [varbinary](max) NULL,
	[f_varchar_n] [varchar](50) NULL,
	[f_varchar_max] [varchar](max) NULL
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]
GO
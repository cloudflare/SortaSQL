CREATE LANGUAGE plpgsql;
CREATE TYPE kc_row;

DROP FUNCTION kc_row_in(cstring) CASCADE;
CREATE FUNCTION kc_row_in(cstring)
    RETURNS kc_row
    AS 'libpg_kc', 'kc_row_in'
    LANGUAGE C IMMUTABLE STRICT;

DROP FUNCTION kc_row_out(kc_row) CASCADE;
CREATE FUNCTION kc_row_out(kc_row)
    RETURNS cstring
    AS 'libpg_kc' , 'kc_row_out'
    LANGUAGE C IMMUTABLE STRICT;

CREATE TYPE kc_row (
   INTERNALLENGTH = variable,    
   INPUT = kc_row_in,
   OUTPUT = kc_row_out,
   ALIGNMENT = double,
   STORAGE = plain
);

DROP FUNCTION kc_key(kc_row);
CREATE FUNCTION kc_key(kc_row)
    RETURNS text
    AS 'libpg_kc', 'kc_key'
    LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION kc_val(kc_row)
    RETURNS bigint
    AS 'libpg_kc', 'kc_val'
    LANGUAGE C IMMUTABLE STRICT;

DROP FUNCTION kc_class(kc_row);
CREATE FUNCTION kc_class(kc_row)
    RETURNS text
    AS 'libpg_kc', 'kc_class'
    LANGUAGE C IMMUTABLE STRICT;

DROP FUNCTION kc_doctype(kc_row);
CREATE FUNCTION kc_doctype(kc_row)
    RETURNS text
    AS 'libpg_kc', 'kc_doctype'
    LANGUAGE C IMMUTABLE STRICT;

DROP FUNCTION kc_pop(kc_row);
CREATE FUNCTION kc_pop(kc_row)
    RETURNS text
    AS 'libpg_kc', 'kc_pop'
    LANGUAGE C IMMUTABLE STRICT;

DROP FUNCTION kc_psource(kc_row);
CREATE FUNCTION kc_psource(kc_row)
    RETURNS text
    AS 'libpg_kc', 'kc_psource'
    LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION kc_sum(bigint, kc_row)
    RETURNS bigint
    AS 'libpg_kc', 'kc_sum'
    LANGUAGE C IMMUTABLE;

CREATE AGGREGATE sum ( kc_row ) (
    SFUNC = kc_sum,
    INITCOND = 0,
    STYPE = bigint
);

CREATE OR REPLACE FUNCTION kcts(__start_time TIMESTAMPTZ, __oid BIGINT) RETURNS text AS $_$
    BEGIN
        RETURN to_char(__start_time, 'YYYY-MM-DD') || '/' || __oid;
    END;
$_$ Language 'plpgsql' IMMUTABLE STRICT;

/**
    Returns multiple rows, being the expanded rows for every map found by the passed in inputs.
    Indexed by: result_id:map_name:doctype:pop:psource

    Call with -- result_id, map_name, doctype, pop, psource
    Everything from map_name down can be null.

    Return map_name doctype pop psource key value list
*/

DROP FUNCTION kcx(text, text, text);
CREATE OR REPLACE FUNCTION kcx(text, text, text)
    RETURNS SETOF kc_row
    AS 'libpg_kc', 'kc_expand'
    LANGUAGE C STABLE STRICT;

DROP FUNCTION kcx(text, text, text, text);
CREATE OR REPLACE FUNCTION kcx(IN text, IN text, IN text, IN text)
    RETURNS SETOF kc_row
    AS 'libpg_kc', 'kc_expand'
    LANGUAGE C STABLE STRICT;

DROP FUNCTION kcx(text, text, text, text, text);
CREATE OR REPLACE FUNCTION kcx(IN text, IN text, IN text, IN text, IN text)
    RETURNS SETOF kc_row
    AS 'libpg_kc', 'kc_expand'
    LANGUAGE C STABLE STRICT;

DROP FUNCTION kcx(text, text, text, text, text, text);
CREATE OR REPLACE FUNCTION kcx(IN text, IN text, IN text, IN text, IN text, IN text)
    RETURNS SETOF kc_row
    AS 'libpg_kc', 'kc_expand'
    LANGUAGE C STABLE STRICT;

DROP FUNCTION kcx(text, text, text, text, text, text, text);
CREATE OR REPLACE FUNCTION kcx(IN text, IN text, IN text, IN text, IN text, IN text, IN text)
    RETURNS SETOF kc_row
    AS 'libpg_kc', 'kc_expand'
    LANGUAGE C STABLE STRICT;

/**
    Given the fixed values, writes (overrites) the kc entry.
    map_name, start_time, new rid, array of rids, class, doctype, pop, psource
    returns new kcid.
*/
DROP FUNCTION kcs (text, text, text, text array, text, text, text, text);
CREATE FUNCTION kcs (text, text, text, text array, text, text, text, text)
    RETURNS text
    AS 'libpg_kc', 'kc_shrink'
    LANGUAGE C STABLE STRICT;

/**
    Also need a way to delete values in the kcdb
*/
DROP FUNCTION kcd (text, text, text array);
CREATE FUNCTION kcd (text, text, text array)
    RETURNS bigint
    AS 'libpg_kc', 'kc_delete'
    LANGUAGE C STABLE STRICT;

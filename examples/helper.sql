CREATE OR REPLACE FUNCTION get_page_views(IN __hours INT, IN __zid INT, OUT __st timestamp, OUT __val bigint) RETURNS setof RECORD AS $_$
    DECLARE
    r                  RECORD;
    BEGIN
        FOR r IN select start_time, sum(val) as val from ( select date_trunc('hour', start_time) as start_time, kc_key(kc) as key, kc_val(kc) as val, kc_doctype(kc) as doc from (select start_time, kcx('basic', kcts(start_time, oid), result_id) as kc from aggregate_meta_data where zid = __zid and start_time >= now() - (__hours || ' hour')::interval and period = '15 min' ) a ) b where key = 'num_rows' and doc = 'HTML' group by start_time order by start_time asc
        LOOP
            __st := r.start_time;
            __val := r.val;
            RETURN NEXT;
        END LOOP; 
    END;
$_$ Language 'plpgsql' IMMUTABLE STRICT;

CREATE TABLE aggregate_meta_data (
  result_id     VARCHAR(128) PRIMARY KEY,
  zid           BIGINT NOT NULL,
  oid           BIGINT NOT NULL,
  start_time    TIMESTAMPTZ NOT NULL,
  period        INTERVAL NOT NULL,
  zone_name     TEXT NOT NULL,
  CONSTRAINT cf_logs_aggregate_uq UNIQUE(zid, start_time, period)
);
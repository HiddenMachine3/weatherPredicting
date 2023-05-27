create database weatherdb;

use weatherdb;

CREATE TABLE weatherdata (
    time TIMESTAMP,
    temperature FLOAT,
    pressure FLOAT,
    humidity FLOAT
);

-- example insert:
-- INSERT INTO weatherdata (time, temperature, pressure, humidity) VALUES ('2023-05-28 10:00:00', 25.5, 1013.2, 60.2);
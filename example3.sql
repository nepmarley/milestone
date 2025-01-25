CREATE DATABASE players;

USE players;

CREATE TABLE players (
    name TEXT,
    height INT,
    jersey INT,
    position TEXT,
    idol TEXT,
    birthstate TEXT,
    country TEXT
);

INSERT INTO players (name, height, jersey, position, idol, birthstate, country)
VALUES ('Kyrie', 186, 11, 'PG', 'Iverson', 'Kentucky', 'USA');

INSERT INTO players (name, height, jersey, position, idol, birthstate, country)
VALUES ('Curry', 186, 30, 'PG', 'Bird', 'California', 'USA');

INSERT INTO players (name, height, jersey, position, idol, birthstate, country)
VALUES ('LeBron', 205, 23, 'SF', 'Jordan', 'Ohio', 'USA');

INSERT INTO players (name, height, jersey, position, idol, birthstate, country)
VALUES ('Jordan', 198, 23, 'SG', 'Wilt', 'Illinois', 'USA');

INSERT INTO players (name, height, jersey, position, idol, birthstate, country)
VALUES ('Kawhi', 198, 2, 'PF', 'Dirk', 'Toronto', 'Canada');

INSERT INTO players (name, height, jersey, position, idol, birthstate, country)
VALUES ('Paul', 196, 13, 'SG', 'Kobe', 'Oklahoma', 'USA');

INSERT INTO players (name, height, jersey, position, idol, birthstate, country)
VALUES ('Tatum', 199, 11, 'SG', 'LeBron', 'Massachusetts', 'USA');

INSERT INTO players (name, height, jersey, position, idol, birthstate, country)
VALUES ('Doncic', 196, 3, 'SG', 'Dirk', 'Texas', 'USA');

INSERT INTO players (name, height, jersey, position, idol, birthstate, country)
VALUES ('Draymond', 197, 4, 'PF', 'Khabib', 'California', 'USA');

INSERT INTO players (name, height, jersey, position, idol, birthstate, country)
VALUES ('Rose', 187, 4, 'PG', 'Iverson', 'Illinois', 'USA');

INSERT INTO players (name, height, jersey, position, idol, birthstate, country)
VALUES ('Kobe', 202, 24, 'SG', 'None', 'California', 'USA');

INSERT INTO players (name, height, jersey, position, idol, birthstate, country)
VALUES ('Harden', 194, 11, 'PG', 'Allen', 'Texas', 'USA');

INSERT INTO players (name, height, jersey, position, idol, birthstate, country)
VALUES ('Jaylen', 191, 11, 'SF', 'LeBron', 'Massachusetts', 'USA');

INSERT INTO players (name, height, jersey, position, idol, birthstate, country)
VALUES ('Tyrese', 197, 19, 'PG', 'Magic', 'Indiana', 'USA');

INSERT INTO players (name, height, jersey, position, idol, birthstate, country)
VALUES ('LaMelo', 201, 3, 'PG', 'Carmelo', 'North Carolina', 'USA');

INSERT INTO players (name, height, jersey, position, idol, birthstate, country)
VALUES ('Isaiah', 180, 11, 'PG', 'Bird', 'Arizona', 'USA');

SELECT * FROM players;

UPDATE players SET country = 'USA' WHERE country = 'Canada';

SELECT * FROM players;

DELETE FROM players WHERE height = 180;

SELECT * FROM players;

SELECT COUNT(*) FROM players;

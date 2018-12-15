DROP TABLE users, msgs, baned_ip;
CREATE TABLE users (
    id      SERIAL PRIMARY KEY,
    fname   varchar(20) NOT NULL,
    lname   varchar(20) NULL,
    sname   varchar(20) NULL,
    phone   varchar(12) NOT NULL,
    email   varchar(39) NOT NULL,
    driver  varchar(1) NULL,
    pass    bytea NOT NULL,
    wLat    float NULL,
    wLong   float NULL,
    rating  float NULL,
    registered  BIGINT NULL,
    lastlogin  BIGINT NULL
);
CREATE TABLE chat (
    id      SERIAL PRIMARY KEY,
    userid BIGINT NOT NULL,
    tgtuid BIGINT NULL,
    tgtgid BIGINT NULL,
    msg TEXT NOT NULL,
    ptime BIGINT NOT NULL
);
CREATE TABLE usergrp (
    id SERIAL PRIMARY KEY,
    ownerid BIGINT NOT NULL,
    ctime BIGINT NOT NULL,
    grpname varchar(50) NOT NULL
);
CREATE TABLE usersin (
    userid BIGINT NOT NULL,
    grpid BIGINT NOT NULL,
    etime BIGINT NOT NULL
);
INSERT INTO users (fname,lname,sname,phone,email,pass,rating)
VALUES ('Алексей','','','+79097915006','felexxx67@gmail.com','\xffffffffffffffff','4.5');
SELECT (id,fname,email,pass) FROM users WHERE phone='+79097915006';
INSERT INTO usergrp (ownerid,ctime,grpname)
VALUES ('1','0','Test');
INSERT INTO usersin (userid,grpid,etime)
VALUES ('1','1','0');

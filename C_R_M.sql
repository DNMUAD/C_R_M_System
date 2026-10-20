/*
 Navicat Premium Data Transfer

 Source Server         : C_R_M
 Source Server Type    : SQLite
 Source Server Version : 3035005 (3.35.5)
 Source Schema         : main

 Target Server Type    : SQLite
 Target Server Version : 3035005 (3.35.5)
 File Encoding         : 65001

 Date: 25/01/2025 15:50:13
*/

PRAGMA foreign_keys = false;

-- ----------------------------
-- Table structure for db_goods_inf
-- ----------------------------
DROP TABLE IF EXISTS "db_goods_inf";
CREATE TABLE "db_goods_inf" (
  "ID" integer NOT NULL ON CONFLICT ABORT COLLATE BINARY PRIMARY KEY AUTOINCREMENT,
  "goodsName" text NOT NULL,
  "manufacturer" text,
  "image" blob,
  "price" integer,
  "desc" text
);

-- ----------------------------
-- Table structure for db_shopping_inf
-- ----------------------------
DROP TABLE IF EXISTS "db_shopping_inf";
CREATE TABLE "db_shopping_inf" (
  "ID" INTEGER NOT NULL ON CONFLICT ABORT COLLATE BINARY PRIMARY KEY AUTOINCREMENT,
  "userId" INTEGER NOT NULL,
  "goosId" INTEGER NOT NULL,
  "date" DATE,
  "sellingPrice" integer,
  "desc" TEXT
);

-- ----------------------------
-- Table structure for db_tag_inf
-- ----------------------------
DROP TABLE IF EXISTS "db_tag_inf";
CREATE TABLE "db_tag_inf" (
  "ID" INTEGER NOT NULL ON CONFLICT ABORT PRIMARY KEY AUTOINCREMENT,
  "tagName" TEXT NOT NULL
);

-- ----------------------------
-- Table structure for db_user_inf
-- ----------------------------
DROP TABLE IF EXISTS "db_user_inf";
CREATE TABLE "db_user_inf" (
  "ID" integer NOT NULL ON CONFLICT ABORT PRIMARY KEY AUTOINCREMENT,
  "name" text NOT NULL,
  "tel" text NOT NULL,
  "company" TEXT,
  "construction" TEXT
);

-- ----------------------------
-- Table structure for db_user_tag
-- ----------------------------
DROP TABLE IF EXISTS "db_user_tag";
CREATE TABLE "db_user_tag" (
  "ID" INTEGER NOT NULL ON CONFLICT ABORT PRIMARY KEY AUTOINCREMENT,
  "userId" INTEGER NOT NULL,
  "tagId" INTEGER NOT NULL
);

-- ----------------------------
-- Table structure for sqlite_sequence
-- ----------------------------
DROP TABLE IF EXISTS "sqlite_sequence";
CREATE TABLE "sqlite_sequence" (
  "name",
  "seq"
);

-- ----------------------------
-- Auto increment value for db_goods_inf
-- ----------------------------
UPDATE "sqlite_sequence" SET seq = 1 WHERE name = 'db_goods_inf';

-- ----------------------------
-- Indexes structure for table db_goods_inf
-- ----------------------------
CREATE UNIQUE INDEX "goodsId"
ON "db_goods_inf" (
  "ID" COLLATE BINARY ASC
);

-- ----------------------------
-- Auto increment value for db_shopping_inf
-- ----------------------------
UPDATE "sqlite_sequence" SET seq = 1 WHERE name = 'db_shopping_inf';

-- ----------------------------
-- Auto increment value for db_tag_inf
-- ----------------------------
UPDATE "sqlite_sequence" SET seq = 3 WHERE name = 'db_tag_inf';

-- ----------------------------
-- Indexes structure for table db_tag_inf
-- ----------------------------
CREATE UNIQUE INDEX "tagId"
ON "db_tag_inf" (
  "ID" COLLATE BINARY ASC
);

-- ----------------------------
-- Auto increment value for db_user_inf
-- ----------------------------
UPDATE "sqlite_sequence" SET seq = 2 WHERE name = 'db_user_inf';

-- ----------------------------
-- Indexes structure for table db_user_inf
-- ----------------------------
CREATE UNIQUE INDEX "userId"
ON "db_user_inf" (
  "ID" COLLATE BINARY ASC
);

-- ----------------------------
-- Auto increment value for db_user_tag
-- ----------------------------
UPDATE "sqlite_sequence" SET seq = 1 WHERE name = 'db_user_tag';

-- ----------------------------
-- Indexes structure for table db_user_tag
-- ----------------------------
CREATE UNIQUE INDEX "user_tag_Id"
ON "db_user_tag" (
  "ID" COLLATE BINARY ASC
);

PRAGMA foreign_keys = true;

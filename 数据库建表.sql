
-- ----------------------------
-- Table structure for Account
-- ----------------------------
DROP TABLE IF EXISTS "Account";
CREATE TABLE "Account" (
  "user_id" text NOT NULL,
  "user_name" text NOT NULL,
  "user_pwd" text,
  "user_type" text,
  "citizen_id" text,
  "sex" text,
  PRIMARY KEY ("user_id")
);

-- ----------------------------
-- Table structure for Class_
-- ----------------------------
DROP TABLE IF EXISTS "Class_";
CREATE TABLE "Class_" (
  "class_id" text NOT NULL,
  "college_id" integer,
  "major_id" integer,
  "id" integer,
  "entry_year" integer,
  PRIMARY KEY ("class_id"),
  CONSTRAINT "college" FOREIGN KEY ("college_id") REFERENCES "College" ("college_id") ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT "major" FOREIGN KEY ("major_id") REFERENCES "Major" ("major_id") ON DELETE CASCADE ON UPDATE CASCADE
);

-- ----------------------------
-- Table structure for College
-- ----------------------------
DROP TABLE IF EXISTS "College";
CREATE TABLE "College" (
  "college_id" INTEGER NOT NULL,
  "college_name" TEXT,
  PRIMARY KEY ("college_id")
);

-- ----------------------------
-- Table structure for Course
-- ----------------------------
DROP TABLE IF EXISTS "Course";
CREATE TABLE "Course" (
  "course_id" text NOT NULL,
  "course_name" text,
  "grade_daily_percent" real,
  PRIMARY KEY ("course_id")
);

-- ----------------------------
-- Table structure for Major
-- ----------------------------
DROP TABLE IF EXISTS "Major";
CREATE TABLE "Major" (
  "major_id" INTEGER NOT NULL,
  "major_name" TEXT,
  PRIMARY KEY ("major_id")
);

-- ----------------------------
-- Table structure for Student
-- ----------------------------
DROP TABLE IF EXISTS "Student";
CREATE TABLE "Student" (
  "user_id" text NOT NULL,
  "entry_year" integer,
  "semester" integer,
  "college_id" integer,
  "major_id" integer,
  "class_id" integer,
  PRIMARY KEY ("user_id"),
  CONSTRAINT "user" FOREIGN KEY ("user_id") REFERENCES "Account" ("user_id") ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT "college" FOREIGN KEY ("college_id") REFERENCES "College" ("college_id") ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT "major" FOREIGN KEY ("major_id") REFERENCES "Major" ("major_id") ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT "class" FOREIGN KEY ("class_id") REFERENCES "Class_" ("class_id") ON DELETE CASCADE ON UPDATE CASCADE
);

-- ----------------------------
-- Table structure for Teacher
-- ----------------------------
DROP TABLE IF EXISTS "Teacher";
CREATE TABLE "Teacher" (
  "user_id" text NOT NULL,
  PRIMARY KEY ("user_id"),
  CONSTRAINT "user" FOREIGN KEY ("user_id") REFERENCES "Account" ("user_id") ON DELETE CASCADE ON UPDATE CASCADE
);

-- ----------------------------
-- Table structure for grade
-- ----------------------------
DROP TABLE IF EXISTS "grade";
CREATE TABLE "grade" (
  "course_id" text NOT NULL,
  "student_id" text NOT NULL,
  "grade_daily" real,
  "grade_final" real,
  "grade_total" real,
  PRIMARY KEY ("course_id", "student_id"),
  CONSTRAINT "course" FOREIGN KEY ("course_id") REFERENCES "Course" ("course_id") ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT "student" FOREIGN KEY ("student_id") REFERENCES "Student" ("user_id") ON DELETE CASCADE ON UPDATE CASCADE
);

-- ----------------------------
-- Table structure for lesson
-- ----------------------------
DROP TABLE IF EXISTS "lesson";
CREATE TABLE "lesson" (
  "class_id" text NOT NULL,
  "course_id" text NOT NULL,
  "teacher_id" text NOT NULL,
  "semester" text,
  PRIMARY KEY ("class_id", "course_id", "teacher_id"),
  CONSTRAINT "class" FOREIGN KEY ("class_id") REFERENCES "Class_" ("class_id") ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT "course" FOREIGN KEY ("course_id") REFERENCES "Course" ("course_id") ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT "teacher" FOREIGN KEY ("teacher_id") REFERENCES "Teacher" ("user_id") ON DELETE CASCADE ON UPDATE CASCADE
);



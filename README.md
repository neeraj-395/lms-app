# library-management-app
A library management system using Qt framework with C++ and MySQL database.

# application file structure

```
lms-app/
├── build
├── src/
│   ├── app/
│   │   ├── main.cpp
│   │   ├── mainwindow.h
│   │   ├── mainwindow.cpp
│   │   └── mainwindow.ui
│   ├── database/
│   │   ├── mysqldb.h
│   │   ├── mysqldb.cpp
│   │   ├── mysqlquery.h
│   │   └── mysqlquery.cp
│   ├── gui/
│   │   ├── loginportal.h
│   │   ├── loginportal.cpp
│   │   ├── loginportal.ui
│   │   ├── userportal.h
│   │   ├── userportal.cpp
│   │   ├── userportal.ui
│   │   ├── librarianportal.h
│   │   ├── librarianportal.cpp
│   │   └── librarianportal.ui
│   ├── managers/
│   │   ├── bookmanager.h
│   │   ├── bookmanager.cpp
│   │   ├── bookmanager.ui
│   │   ├── transacmanager.h
│   │   ├── transacmanager.cpp
│   │   ├── returnbook.ui
│   │   └── checkoutbook.ui
│   ├── utils/
│   │   ├── qutils.cpp
│   │   └── qutils.h
│   └── CMakeLists.txt
├── test/
│   ├── mysqltest.cpp
│   └── CMakeLists.txt
├── res/
│   ├── create.db.sql
│   ├── insert.db.sql
│   └── config.db.ini
├── CMakeLists.txt
├── LICENSE
└── README.md
```

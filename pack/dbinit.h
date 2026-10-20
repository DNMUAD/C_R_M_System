#ifndef DBINIT_H
#define DBINIT_H

#include <QSqlDatabase>
#include <QSharedPointer>

class DBInit {
public:
    DBInit(QSqlDatabase &db);
    ~DBInit();

private:
    void init_DB();

private:
    QSqlDatabase  &DB;
};

class DBObject {
public:
    static DBObject &getInstance();

    QSqlDatabase &getDBObj();

    void deleteLater();

private:
    explicit DBObject();
    ~DBObject();

    QSqlDatabase m_db;

    QSharedPointer<DBInit> m_pHandle;
};

#define g_getDBObject() (DBObject::getInstance())
#define deleteDBObject() (DBObject::getInstance().deleteLater())

#endif // DBINIT_H

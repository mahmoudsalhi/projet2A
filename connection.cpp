#include "connection.h"
Connection::Connection()
{
}
bool Connection::createconnect()
{bool test=false;
QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
db.setDatabaseName("database");
db.setUserName("anwar");// nom d'utilisateur
db.setPassword("0000");// mot de passe d'utilisateur
if (db.open())
test=true;
    return  test;
}

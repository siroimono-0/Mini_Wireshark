#include "Open_List.h"

Open_List::Open_List(QObject *parent) :  QAbstractListModel{parent}
{
    this->create_db_lite();
    this->init_vec();
}

void Open_List::create_db_table()
{
    QSqlQuery query(this->db_lite);
    QString sql = "CREATE TABLE open_list ("
                  "path TEXT PRIMARY KEY "
                  "NOT NULL);";
    bool ok_prepare = query.prepare(sql);

    if (!this->check_query_prepare(ok_prepare, query))
    {
        return;
    }

    bool ok_exec = query.exec();

    if (!this->check_query_exec(ok_exec, query))
    {
        return;
    }
    return;
}

void Open_List::create_db_lite()
{
    QString s_data_dir;
    QString s_db_path;
    QDir qd_dir;

    // s_data_dir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    s_data_dir = "/home/gray/gray/git_shark/Mini_Wireshark/SQLITE";
    s_db_path = s_data_dir + "/db_lite.sqlite3";
    qDebug() << s_db_path;
    qd_dir.mkpath(s_data_dir);

    this->db_lite = QSqlDatabase::addDatabase("QSQLITE", "DB_THREAD_CONN_SQLITE");
    this->db_lite.setDatabaseName(s_db_path);

    this->cur_path = s_data_dir;
    if (this->db_lite.open())
    {
        qDebug() << "db_light open";
    }
    else
    {
        // qDebug() << "db false";
        qDebug() << this->db_lite.lastError().text();
    }

    this->create_db_table();

    return;
}

bool Open_List::check_query_prepare(bool ok, QSqlQuery &query)
{
    if (!ok)
    {
        QSqlError e = query.lastError();
        qDebug() << "DB prepare failed";
        qDebug() << "driverText =" << e.driverText();
        qDebug() << "databaseText =" << e.databaseText();
        qDebug() << "lastQuery =" << query.lastQuery();
        return false;
    }

    return true;
}

bool Open_List::check_query_exec(bool ok, QSqlQuery &query)
{
    if (!ok)
    {
        QSqlError e = query.lastError();
        qDebug() << "DB exec failed";
        qDebug() << "driverText =" << e.driverText();
        qDebug() << "databaseText =" << e.databaseText();
        qDebug() << "type =" << e.type();
        qDebug() << "lastQuery =" << query.lastQuery();
        qDebug() << "boundValues =" << query.boundValues();
        return false;
    }
    return true;
}

int Open_List::rowCount(const QModelIndex &parent) const
{
    if(parent.isValid())
    {
        return 0;
    }
    return vec.size();
}

QHash<int, QByteArray> Open_List::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[pathRole] = "path";
    return roles;
}

QVariant Open_List::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= vec.size())
    {
        return QVariant();
    }


    if(role == pathRole)
    {
        return QVariant(vec.at(index.row()));
    }

    return QVariant();
}

void Open_List::add_db(const QString path)
{
    qDebug() << path << "   ?? ?";
    QSqlQuery query(this->db_lite);

    QString sql = "INSERT INTO open_list ("
                  "path) VALUES (:path)";

    bool ok_prepare = query.prepare(sql);

    if (!this->check_query_prepare(ok_prepare, query))
    {
        // return;
    }

    query.bindValue(":path", path);
    bool ok_exec = query.exec();

    if (!this->check_query_exec(ok_exec, query))
    {
        // return;
    }

    qDebug() <<  "   111111111111111111111111111111111111111111111";

    QSqlQuery query_2(this->db_lite);

    QString sql_2 = "SELECT * FROM open_list";
    bool ok_prepare_2 = query_2.prepare(sql_2);

    if (!this->check_query_prepare(ok_prepare_2, query_2))
    {
        return;
    }

    bool ok_exec_2 = query_2.exec();

    if (!this->check_query_exec(ok_exec_2, query_2))
    {
        return;
    }
    qDebug() <<  "   222222222222222222222222222222222222222222222222";

    QVector<QString> vec_tmp;
    while(query_2.next())
    {
        vec_tmp.push_back(query_2.value(0).toString());
    }

    this->update_vec(vec_tmp);

    return;
}

void  Open_List::update_vec(const QVector<QString> vec_tmp)
{
    beginResetModel();
    this->vec.clear();
    this->vec = vec_tmp;
    endResetModel();
    return;
}

void   Open_List::init_vec()
{
    QSqlQuery query_2(this->db_lite);
    QString sql_2 = "SELECT * FROM open_list";
    bool ok_prepare_2 = query_2.prepare(sql_2);

    if (!this->check_query_prepare(ok_prepare_2, query_2))
    {
        return;
    }

    bool ok_exec_2 = query_2.exec();

    if (!this->check_query_exec(ok_exec_2, query_2))
    {
        return;
    }

    QVector<QString> vec_tmp;
    while(query_2.next())
    {
        vec_tmp.push_back(query_2.value(0).toString());
    }

    this->update_vec(vec_tmp);

    return;

}

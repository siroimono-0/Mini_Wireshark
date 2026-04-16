#ifndef OPEN_LIST_H
#define OPEN_LIST_H
#include <QObject>
#include <QAbstractListModel>
#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QNetworkInterface>
#include <QtSql>

//==============================Cpp==============================
#include <pcap.h>
//==============================Cpp==============================

class Open_List : public QAbstractListModel
{
    Q_OBJECT

public:

    //==============================Default==============================
    // QML에서 접근할 "역할(role)" 이름 정의
    enum Role {
        pathRole = Qt::UserRole + 1  // 사용자 정의 역할은 UserRole + 1부터 시작
    };
    Q_ENUM(Role)  // QML에서도 enum을 인식할 수 있게 함

    explicit Open_List(QObject *parent = nullptr);

    // 모델의 총 행 개수 반환 (QML의 model.count 등에서 사용)
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    // 특정 인덱스의 데이터 반환 (QML에서 model.text 요청 시 호출됨)
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // QML에서 model.text로 접근할 수 있도록 role과 이름 매핑
    QHash<int, QByteArray> roleNames() const override;
    //==============================Default==============================

    void create_db_table();
    void create_db_lite();
    bool check_query_prepare(bool ok, QSqlQuery &query);
    bool check_query_exec(bool ok, QSqlQuery &query);

    Q_INVOKABLE void add_db(const QString path);

    void update_vec(const QVector<QString> vec_tmp);

    void init_vec();
public slots:

private:
    QVector<QString> vec;
    QSqlDatabase db_lite;
    QString cur_path;



signals:
};


#endif // OPEN_LIST_H

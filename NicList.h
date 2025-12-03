#ifndef NICLIST_H
#define NICLIST_H

#include <QObject>
#include <QAbstractListModel>

//==============================Cpp==============================
#include <pcap.h>
//==============================Cpp==============================

class NicList : public QAbstractListModel
{
    Q_OBJECT

public:

    //==============================Default==============================
    // QML에서 접근할 "역할(role)" 이름 정의
    enum Role {
        TextRole = Qt::UserRole + 1,  // 사용자 정의 역할은 UserRole + 1부터 시작
        TypeRole = Qt::UserRole + 2
    };
    Q_ENUM(Role)  // QML에서도 enum을 인식할 수 있게 함

    enum nic_op{
        OP_Loop = 1,
        OP_Wlo = 2,
        OP_Ethernet = 3,
        OP_Virtual = 4,
        OP_Any = 5
    };

    explicit NicList(QObject *parent = nullptr);

    // 모델의 총 행 개수 반환 (QML의 model.count 등에서 사용)
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    // 특정 인덱스의 데이터 반환 (QML에서 model.text 요청 시 호출됨)
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // QML에서 model.text로 접근할 수 있도록 role과 이름 매핑
    QHash<int, QByteArray> roleNames() const override;
    //==============================Default==============================

    Q_INVOKABLE void set_Nic(int set_flag = OP_Any);
    Q_INVOKABLE QString get_FirstNic();

    int compare_Nic(const QString& name, int flags);

public slots:

private:
    QVector<std::pair<QString, int>> vec;

signals:
};

#endif // NICLIST_H

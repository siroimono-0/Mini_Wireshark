#ifndef CAPTURE_H
#define CAPTURE_H
#include <QObject>
#include <QAbstractListModel>

//==============================Cpp==============================
#include "Struct_in.h"
#include <pcap.h>
#include <net/ethernet.h>   // struct ether_header
#include <netinet/if_ether.h>
#include <netinet/ip.h>
//==============================Cpp==============================


class Capture  : public QAbstractListModel
{
    Q_OBJECT

public:

    //==============================Default==============================
    // QML에서 접근할 "역할(role)" 이름 정의
    enum Role {
        Num_Role = Qt::UserRole + 1,  // 사용자 정의 역할은 UserRole + 1부터 시작
        Src_Role = Qt::UserRole + 2,  // 사용자 정의 역할은 UserRole + 1부터 시작
        Dst_Role = Qt::UserRole + 3,  // 사용자 정의 역할은 UserRole + 1부터 시작
        Proto_Role = Qt::UserRole + 4,  // 사용자 정의 역할은 UserRole + 1부터 시작
        Len_Role = Qt::UserRole + 5,  // 사용자 정의 역할은 UserRole + 1부터 시작
        Info_Role = Qt::UserRole + 6,  // 사용자 정의 역할은 UserRole + 1부터 시작
    };
    Q_ENUM(Role)  // QML에서도 enum을 인식할 수 있게 함

    explicit  Capture(QObject *parent = nullptr);

    // 모델의 총 행 개수 반환 (QML의 model.count 등에서 사용)
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    // 특정 인덱스의 데이터 반환 (QML에서 model.text 요청 시 호출됨)
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // QML에서 model.text로 접근할 수 있도록 role과 이름 매핑
    QHash<int, QByteArray> roleNames() const override;
    //==============================Default==============================

    void update_md(st_pkt& pkt);

public slots:

private:
    QVector<st_pkt> vec;
    pcap_t* pp = nullptr;
    int num = 0;

signals:
};

#endif //

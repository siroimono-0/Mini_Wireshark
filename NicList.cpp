#include "NicList.h"
#include "Support.h"

NicList::NicList(QObject *parent) :  QAbstractListModel{parent}
{
    set_Nic();
}

int NicList::rowCount(const QModelIndex &parent) const
{
    if(parent.isValid())
    {
        return 0;
    }
    return vec.size();
}

QHash<int, QByteArray> NicList::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[TextRole] = "text";
    roles[TypeRole] = "type";
    return roles;
}

QVariant NicList::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= vec.size())
    {
        return QVariant();
    }

    QString str = vec.at(index.row()).first;
    int type = vec.at(index.row()).second;
    if (role == TextRole)
    {
        return QVariant(str);  // QML에서 model.text로 접근 시 사용됨
    }
    else if(role == TypeRole)
    {
        return QVariant(type);  // QML에서 model.text로 접근 시 사용됨
    }

    return QVariant();
}

void NicList::set_Nic(int set_flag)
{
    vec.clear();

    pcap_if_t* pp;
    char errBuf[PCAP_ERRBUF_SIZE];
    memset(errBuf, 0, PCAP_ERRBUF_SIZE);

    pcap_findalldevs(&pp, errBuf);
    // check::ck(string(Q_FUNC_INFO) + "pcap_findalldevs", ret_p, -1);

    // pcap_if_t 리스트 순회
    for (pcap_if_t* p = pp; p != nullptr; p = p->next)
    {
        if (p->name)
        {
            int flag = compare_Nic(QString(p->name), p->flags);
            if(set_flag != OP_Any && flag == set_flag)
            {
                vec.push_back({QString(p->name), flag});
            }
            else if(set_flag == OP_Any && flag)
            {
                vec.push_back({QString(p->name), flag});
            }

            // 장치명 저장
        }
    }

    // QAbstractListModel 새로고침
    beginResetModel();
    endResetModel();

    // 메모리 해제
    pcap_freealldevs(pp);
    return;
}

int NicList::compare_Nic(const QString& name, int flags)
{
    if (flags & PCAP_IF_LOOPBACK)
    {
        return OP_Loop;
    }
    else if(name.startsWith("wl"))
    {
        return OP_Wlo;
    }
    else if (name.startsWith("en") || name.startsWith("eth"))
    {
        return OP_Ethernet;
    }
    else if(name.startsWith("vmnet") || name.startsWith("vbox"))
    {
        return OP_Virtual;
    }

    return 0;

}

Q_INVOKABLE QString NicList::get_FirstNic()
{
    return this->vec.at(0).first;
}

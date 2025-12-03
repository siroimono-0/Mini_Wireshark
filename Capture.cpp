#include "Capture.h"
#include "Support.h"

Capture::Capture(QObject *parent) : QAbstractListModel{parent}
{
    // st_pkt pkt = {0};
    // pkt.src = "hello";
    // this->update_md(pkt);
    // qDebug() << "update_md";
}

int Capture::rowCount(const QModelIndex &parent) const
{
    if(parent.isValid())
    {
        return 0;
    }
    return vec.size();
}

QHash<int, QByteArray> Capture::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Num_Role] = "number";
    roles[Src_Role] = "source";
    roles[Dst_Role] = "destination";
    roles[Proto_Role] = "protocol";
    roles[Len_Role] = "length";
    roles[Info_Role] = "info"; return roles;
}

QVariant Capture::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= vec.size())
    {
        return QVariant();
    }


    if(role == Num_Role)
    {
        return QVariant(vec.at(index.row()).num);
    }
    else if(role == Src_Role)
    {
        return QVariant(vec.at(index.row()).src);
    }
    else if(role == Dst_Role)
    {
        return QVariant(vec.at(index.row()).dst);
    }
    else if(role == Proto_Role)
    {
        return QVariant(vec.at(index.row()).proto);
    }
    else if(role == Len_Role)
    {
        return QVariant(vec.at(index.row()).len);
    }
    else if(role == Info_Role)
    {
        return QVariant(vec.at(index.row()).info);
    }

    return QVariant();
}

void Capture::update_md(st_pkt& pkt)
{
    int row = this->vec.size();

    beginInsertRows(QModelIndex(), row, row);
    vec.push_back(pkt);
    endInsertRows();

    return;
}

void Capture::reset()
{
    beginResetModel();
    vec.clear();
    endResetModel();
    return;
}

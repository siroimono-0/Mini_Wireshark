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

Q_INVOKABLE void Capture::sort_num()
{
    std::sort(this->vec.begin(), this->vec.end(),
              [&] (st_pkt pkt_1, st_pkt pkt_2){
        if(this->num_flag == false)
        {
            return pkt_1.num > pkt_2.num;
        }
        else
        {
            return pkt_1.num < pkt_2.num;
        }
    });

    if(num_flag == true) {
        num_flag = false;
    }
    else
    {
        num_flag = true;
    }


    beginResetModel();
    endResetModel();
    return;
}

Q_INVOKABLE void Capture::sort_src()
{
    paser_and_sort(SRC);
}

Q_INVOKABLE void  Capture::sort_dst()
{
    paser_and_sort(DST);
}

void Capture::paser_and_sort(int target)
{
    if(target == SRC)
    {
        std::sort(this->vec.begin(), this->vec.end(),
                  [&] (st_pkt pkt_1, st_pkt pkt_2){
            QString qs_1 = pkt_1.src;
            QString qs_2 = pkt_2.src;
            QStringList qs_1_arr = qs_1.split(".");
            QStringList qs_2_arr = qs_2.split(".");

            if(this->src_flag == false)
            {
                for(int i = 0; i < 4; i++)
                {
                    if(qs_1_arr[i].toInt() != qs_2_arr[i].toInt())
                    {
                        return qs_1_arr[i].toInt() > qs_2_arr[i].toInt();
                    }
                }
            }
            else
            {
                for(int i = 0; i < 4; i++)
                {
                    if(qs_1_arr[i].toInt() != qs_2_arr[i].toInt())
                    {
                        return qs_1_arr[i].toInt() < qs_2_arr[i].toInt();
                    }
                }
            }
            return false;
        });
        if(src_flag == true)
        {
            src_flag = false;
        }
        else
        {
            src_flag = true;
        }
    }
    else if(target == DST)
    {
        std::sort(this->vec.begin(), this->vec.end(),
                  [&] (st_pkt pkt_1, st_pkt pkt_2){
            QString qs_1 = pkt_1.dst;
            QString qs_2 = pkt_2.dst;
            QStringList qs_1_arr = qs_1.split(".");
            QStringList qs_2_arr = qs_2.split(".");

            if(this->dst_flag == false)
            {
                for(int i = 0; i < 4; i++)
                {
                    if(qs_1_arr[i].toInt() != qs_2_arr[i].toInt())
                    {
                        return qs_1_arr[i].toInt() > qs_2_arr[i].toInt();
                    }
                }
            }
            else
            {
                for(int i = 0; i < 4; i++)
                {
                    if(qs_1_arr[i].toInt() != qs_2_arr[i].toInt())
                    {
                        return qs_1_arr[i].toInt() < qs_2_arr[i].toInt();
                    }
                }
            }
            return false;
        });
        if(dst_flag == true)
        {
            dst_flag = false;
        }
        else
        {
            dst_flag = true;
        }
    }

    beginResetModel();
    endResetModel();
    return;
}

Q_INVOKABLE void Capture::sort_proto()
{
    this->proto_num = (this->proto_num + 1) % 3;

    std::sort(this->vec.begin(), this->vec.end(),
              [&] (st_pkt pkt_1, st_pkt pkt_2){

        int proto_1 = get_proto_num(pkt_1.proto);
        int proto_2 = get_proto_num(pkt_2.proto);

        proto_1 = (proto_1 + 3 - this->proto_num) % 3;
        proto_2 = (proto_2 + 3 - this->proto_num) % 3;

        return  proto_1 < proto_2;
    });

    beginResetModel();
    endResetModel();
    return;
}

int Capture::get_proto_num(QString qs)
{
    if(qs == "TCP")
    {
        return 0;
    }
    else if(qs == "UDP")
    {
        return 1;
    }
    else if(qs == "ICMP")
    {
        return 2;
    }
    return 0;
}

Q_INVOKABLE void Capture::sort_len()
{
    if(this->len_flag == false)
    {
        this->len_flag = true;
        std::sort(this->vec.begin(), this->vec.end(),
                  [&] (st_pkt pkt_1, st_pkt pkt_2){

            return pkt_1.len < pkt_2.len;
        });
    }
    else
    {
        this->len_flag = false;
        std::sort(this->vec.begin(), this->vec.end(),
                  [&] (st_pkt pkt_1, st_pkt pkt_2){

            return pkt_1.len > pkt_2.len;
        });
    }

    beginResetModel();
    endResetModel();
    return;
}


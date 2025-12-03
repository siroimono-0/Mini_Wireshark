#include "Pcap.h"
#include "NicList.h"
#include "Capture.h"

using namespace std;

Pcap::Pcap(QObject *parent): QObject{parent}
{
    this->md = new Capture(this);
}


//=============================================================================
//=============================================================================
Worker::Worker(QObject *parent): QObject{parent}
{}

void Worker::my_packet_handler( u_char *user,
                               const struct pcap_pkthdr *header,
                               const u_char * packet)
{
    Worker* p_this = (Worker *) user;
    st_pkt pkt = {0};

    e_H* e_header;
    ip_H* ip_header = (ip_H*)(packet + sizeof(e_H));

    if(ip_header->protocol == 6)
    {
        pkt.proto = "TCP";
        tcp_H* tcp_header = (tcp_H*)((u_char*)ip_header + ip_header->ihl * 4);
        QString qs_ret = "";

        int port_src = ntohs(tcp_header->source);
        QString qs_src = QString::number(port_src);

        int port_dst = ntohs(tcp_header->dest);
        QString qs_dst = QString::number(port_dst);

        qs_ret += qs_src;
        qs_ret += " ---> ";
        qs_ret += qs_dst;

        QString qs_flag = "   [ ";
        qs_flag += (tcp_header->syn == 1) ? "SYN " : "";
        qs_flag += (tcp_header->ack == 1) ? "ACK " : "";
        qs_flag += (tcp_header->fin == 1) ? "FIN " : "";
        qs_flag += (tcp_header->rst == 1) ? "RST " : "";
        qs_flag += (tcp_header->psh == 1) ? "PSH " : "";
        qs_flag += (tcp_header->urg == 1) ? "URG " : "";
        qs_flag += "]   ";

        qs_ret += qs_flag;

        int seq = ntohl(tcp_header->seq);
        QString qs_seq = QString::number(seq);

        qs_ret += "Seq=";
        qs_ret += qs_seq;  int ack = ntohl(tcp_header->ack_seq);
        QString qs_ack = QString::number(ack);

        qs_ret += "   Ack=";
        qs_ret += qs_ack;

        int winSize = ntohs(tcp_header->window);
        QString qs_win = QString::number(winSize);

        qs_ret += "   Win=";
        qs_ret += qs_win;

        int len = (ntohs(ip_header->tot_len)) -
                  (ip_header->ihl * 4) -
                  (tcp_header->doff * 4);
        QString qs_len = QString::number(len);

        qs_ret += "   Len=";
        qs_ret += qs_len;

        pkt.info = qs_ret;
    }
    else if(ip_header->protocol == 17)
    {
        pkt.proto = "UDP";
        udp_H* udp_header = (udp_H*)((u_char*)ip_header + ip_header->ihl * 4);
        QString qs_ret = "";

        int port_src = ntohs(udp_header->source);
        QString qs_src = QString::number(port_src);

        int port_dst = ntohs(udp_header->dest);
        QString qs_dst = QString::number(port_dst);

        int len = ntohs(udp_header->len);
        QString qs_len = QString::number(len);

        qs_ret += qs_src;
        qs_ret += " ---> ";
        qs_ret += qs_dst;
        qs_ret += "   Len=";
        qs_ret += qs_len;

        pkt.info = qs_ret;

    }
    else if(ip_header->protocol == 1)
    {
        pkt.proto = "ICMP";
        icmp_H* icmp_header = (icmp_H*)((u_char*)ip_header + ip_header->ihl * 4);
        QString qs_ret = "";

        qs_ret += (icmp_header->type == 0) ? "Echo Reply" : "";
        qs_ret += (icmp_header->type == 3) ? "Destination Unreachable" : "";
        qs_ret += (icmp_header->type == 5) ? "Redirect" : "";
        qs_ret += (icmp_header->type == 8) ? "Echo Request" : "";
        qs_ret += (icmp_header->type == 11) ? "Time Exceeded" : "";
        qs_ret += "   ";

        if(icmp_header->type == 3)
        {
            qs_ret += (icmp_header->code == 0) ? "Network Unreachable" : "";
            qs_ret += (icmp_header->code == 1) ? "Host Unreachable" : "";
            qs_ret += (icmp_header->code == 3) ? "Port Unreachable" : "";
        }
        else if(icmp_header->type == 11)
        {
            qs_ret += (icmp_header->code == 0) ? "TTL exceeded" : "";
            qs_ret += (icmp_header->code == 1)
                          ? "fragment reassembly time exceeded" : "";
        }
        else if(icmp_header->type == 0 || icmp_header->type == 8)
        {
            qs_ret += "id=";
            uint16_t id = ntohs(icmp_header->un.echo.id);
            qs_ret += QString::number(id);
            qs_ret += "   ";

            qs_ret += "seq=";
            uint16_t seq = ntohs(icmp_header->un.echo.sequence);
            qs_ret += QString::number(seq);
            qs_ret += "   ";

            qs_ret += "ttl=";
            qs_ret += QString::number(ip_header->ttl);
        }

        pkt.info = qs_ret;
    }
    else
    {
        return;
    }

    pkt.src = inet_ntoa(*(struct in_addr*)&ip_header->saddr);
    pkt.dst = inet_ntoa(*(struct in_addr*)&ip_header->daddr);
    pkt.num = p_this->num++;

    pkt.len = header->len;


    bool ret_invo =
        QMetaObject::invokeMethod(p_this->p_Pcap,
                                              &Pcap::update_md,
                                              Qt::QueuedConnection,
                                              pkt);


    if(ret_invo == false)
    {
        qDebug() << "Err invo";
    }

    // qDebug() << p_this->num;
}

int  Worker::set_Capture()
{
    char errbuf[PCAP_ERRBUF_SIZE];
    memset(errbuf, 0, PCAP_ERRBUF_SIZE);

    this->pp = pcap_open_live(this->nicName.c_str(), 65535, 0, 0, errbuf);
    // this->pp = pcap_open_live("wlo1", 65535, 0, 0, errbuf);
    bool ret_invo =
        QMetaObject::invokeMethod(this->p_Pcap,
                                              &Pcap::set_wk_pp,
                                              Qt::QueuedConnection,
                                              this->pp);

    if(ret_invo == false)
    {
        qDebug() << "Err invo";
    }

    bool set = true;
    QMetaObject::invokeMethod(this->p_Pcap,
                              &Pcap::set_wk_life,
                              Qt::QueuedConnection,
                              set);

    qDebug() << Q_FUNC_INFO;

    if(this->BFS_ft.empty())
    {
        pcap_loop(this->pp, -1, my_packet_handler ,(u_char*)this);
        qDebug() << Q_FUNC_INFO;
    }
    else if(!this->BFS_ft.empty())
    {
        struct bpf_program st_bpf = {};
        bpf_u_int32 net = 0xffffff00;
        int ret_pcap = pcap_compile(this->pp,
                                    &st_bpf,
                                    BFS_ft.c_str(),
                                    1, net);
        if(ret_pcap == -1)
        {
            qDebug() << "Err pcap_compile";
            return 0;
        }

        pcap_setfilter(this->pp, &st_bpf);

        pcap_loop(this->pp, -1, my_packet_handler ,(u_char*)this);
        qDebug() << Q_FUNC_INFO;
    }

    pcap_close(this->pp);

    bool set_2 = false;
    QMetaObject::invokeMethod(this->p_Pcap,
                              &Pcap::set_wk_life,
                              Qt::QueuedConnection,
                              set_2);

    emit sig_end();
    return 0;
}

void Worker::set_nicName(string set)
{
    this->nicName = set;
}

void Worker::set_p_Pcap(Pcap* set)
{
    this->p_Pcap = set;
}

void Worker::set_BFS_ft(std::string set)
{
    this->BFS_ft = set;
}

//=============================================================================
//=============================================================================

Q_INVOKABLE void Pcap::create_Th(QVariant qba, QVariant ft)
{
    QString qs = qba.toString();
    string str = qs.toStdString();

    QString qs_2 = ft.toString();
    string str_2 = qs_2.toStdString();

    QThread* th = new QThread;
    Worker* wk = new Worker();
    this->p_Wk = wk;
    wk->set_nicName(str);
    wk->set_p_Pcap(this);
    wk->set_BFS_ft(str_2);

    wk->moveToThread(th);
    connect(th, &QThread::started, wk, &Worker::set_Capture);
    connect(wk, &Worker::sig_end, th, &QThread::quit);
    connect(th, &QThread::finished, wk, &QObject::deleteLater);
    connect(th, &QThread::finished, th, &QObject::deleteLater);

    th->start();
}

Q_INVOKABLE void Pcap::stop_Th()
{
    if(this->wk_pp == nullptr)
    {
        qDebug() << "Current Stoped";
        return;
    }

    pcap_breakloop(this->wk_pp);
    qDebug() << "Stop";
    return;
}

void Pcap::set_wk_pp(pcap_t* set)
{
    this->wk_pp = set;
    qDebug() << Q_FUNC_INFO;
    return;
}

void Pcap::update_md(st_pkt pkt)
{
    this->md->update_md(pkt);
    // qDebug() << "update";
    return;
}

Capture* Pcap::get_md()
{
    return this->md;
}


Q_INVOKABLE void  Pcap::reset_md()
{
    if(wk_life == false)
    {
        md->reset();
    }
    else
    {
        qDebug() << "Current Running...";
    }
    return;
}

void Pcap::set_wk_life(bool set)
{
    this->wk_life = set;
    return;
}

Q_INVOKABLE void Pcap::pcapFile_Read(QString path)
{
    string localPath = QUrl(path).toLocalFile().toStdString();
    char errbuf[PCAP_ERRBUF_SIZE];
    memset(errbuf, 0, PCAP_ERRBUF_SIZE);

    this->p_pcap = pcap_open_offline(localPath.c_str(), errbuf);
    // this->pp = pcap_open_live("wlo1", 65535, 0, 0, errbuf);

    qDebug() << Q_FUNC_INFO;


    const u_char *pkt_data = nullptr;
    struct pcap_pkthdr* pkt_h = nullptr;

    while(pcap_next_ex(this->p_pcap, &pkt_h, &pkt_data) == 1)
    {
        this->packet_func((u_char*)this, pkt_h, pkt_data);
    }


    pcap_close(this->p_pcap);
    this->num = 0;
    return ;
}

Q_INVOKABLE void Pcap::save_md(QString path)
{

    string localPath = QUrl(path).toLocalFile().toStdString();

    char errbuf[PCAP_ERRBUF_SIZE];
    memset(errbuf, 0, PCAP_ERRBUF_SIZE);

    this->p_pcap = pcap_open_offline(localPath.c_str(), errbuf);
    // this->pp = pcap_open_live("wlo1", 65535, 0, 0, errbuf);

    qDebug() << Q_FUNC_INFO;


    const u_char *pkt_data = nullptr;
    struct pcap_pkthdr* pkt_h = nullptr;

    while(pcap_next_ex(this->p_pcap, &pkt_h, &pkt_data) == 1)
    {
        this->packet_func((u_char*)this, pkt_h, pkt_data);
    }


    pcap_close(this->p_pcap);
    // this->num = 0;
    return ;

}

void Pcap::packet_func( u_char *user,
                 const struct pcap_pkthdr *header,
                 const u_char * packet)
{
    Pcap* p_this = (Pcap *) user;
    st_pkt pkt = {0};

    e_H* e_header;
    ip_H* ip_header = (ip_H*)(packet + sizeof(e_H));

    if(ip_header->protocol == 6)
    {
        pkt.proto = "TCP";
        tcp_H* tcp_header = (tcp_H*)((u_char*)ip_header + ip_header->ihl * 4);
        QString qs_ret = "";

        int port_src = ntohs(tcp_header->source);
        QString qs_src = QString::number(port_src);

        int port_dst = ntohs(tcp_header->dest);
        QString qs_dst = QString::number(port_dst);

        qs_ret += qs_src;
        qs_ret += " ---> ";
        qs_ret += qs_dst;

        QString qs_flag = "   [ ";
        qs_flag += (tcp_header->syn == 1) ? "SYN " : "";
        qs_flag += (tcp_header->ack == 1) ? "ACK " : "";
        qs_flag += (tcp_header->fin == 1) ? "FIN " : "";
        qs_flag += (tcp_header->rst == 1) ? "RST " : "";
        qs_flag += (tcp_header->psh == 1) ? "PSH " : "";
        qs_flag += (tcp_header->urg == 1) ? "URG " : "";
        qs_flag += "]   ";

        qs_ret += qs_flag;

        int seq = ntohl(tcp_header->seq);
        QString qs_seq = QString::number(seq);

        qs_ret += "Seq=";
        qs_ret += qs_seq;  int ack = ntohl(tcp_header->ack_seq);
        QString qs_ack = QString::number(ack);

        qs_ret += "   Ack=";
        qs_ret += qs_ack;

        int winSize = ntohs(tcp_header->window);
        QString qs_win = QString::number(winSize);

        qs_ret += "   Win=";
        qs_ret += qs_win;

        int len = (ntohs(ip_header->tot_len)) -
                  (ip_header->ihl * 4) -
                  (tcp_header->doff * 4);
        QString qs_len = QString::number(len);

        qs_ret += "   Len=";
        qs_ret += qs_len;

        pkt.info = qs_ret;
    }
    else if(ip_header->protocol == 17)
    {
        pkt.proto = "UDP";
        udp_H* udp_header = (udp_H*)((u_char*)ip_header + ip_header->ihl * 4);
        QString qs_ret = "";

        int port_src = ntohs(udp_header->source);
        QString qs_src = QString::number(port_src);

        int port_dst = ntohs(udp_header->dest);
        QString qs_dst = QString::number(port_dst);

        int len = ntohs(udp_header->len);
        QString qs_len = QString::number(len);

        qs_ret += qs_src;
        qs_ret += " ---> ";
        qs_ret += qs_dst;
        qs_ret += "   Len=";
        qs_ret += qs_len;

        pkt.info = qs_ret;

    }
    else if(ip_header->protocol == 1)
    {
        pkt.proto = "ICMP";
        icmp_H* icmp_header = (icmp_H*)((u_char*)ip_header + ip_header->ihl * 4);
        QString qs_ret = "";

        qs_ret += (icmp_header->type == 0) ? "Echo Reply" : "";
        qs_ret += (icmp_header->type == 3) ? "Destination Unreachable" : "";
        qs_ret += (icmp_header->type == 5) ? "Redirect" : "";
        qs_ret += (icmp_header->type == 8) ? "Echo Request" : "";
        qs_ret += (icmp_header->type == 11) ? "Time Exceeded" : "";
        qs_ret += "   ";

        if(icmp_header->type == 3)
        {
            qs_ret += (icmp_header->code == 0) ? "Network Unreachable" : "";
            qs_ret += (icmp_header->code == 1) ? "Host Unreachable" : "";
            qs_ret += (icmp_header->code == 3) ? "Port Unreachable" : "";
        }
        else if(icmp_header->type == 11)
        {
            qs_ret += (icmp_header->code == 0) ? "TTL exceeded" : "";
            qs_ret += (icmp_header->code == 1)
                          ? "fragment reassembly time exceeded" : "";
        }
        else if(icmp_header->type == 0 || icmp_header->type == 8)
        {
            qs_ret += "id=";
            uint16_t id = ntohs(icmp_header->un.echo.id);
            qs_ret += QString::number(id);
            qs_ret += "   ";

            qs_ret += "seq=";
            uint16_t seq = ntohs(icmp_header->un.echo.sequence);
            qs_ret += QString::number(seq);
            qs_ret += "   ";

            qs_ret += "ttl=";
            qs_ret += QString::number(ip_header->ttl);
        }

        pkt.info = qs_ret;
    }
    else
    {
        return;
    }

    pkt.src = inet_ntoa(*(struct in_addr*)&ip_header->saddr);
    pkt.dst = inet_ntoa(*(struct in_addr*)&ip_header->daddr);
    pkt.num = p_this->num++;

    pkt.len = header->len;

    Pcap::update_md(pkt);

}

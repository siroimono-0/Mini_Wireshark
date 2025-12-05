#include "Pcap.h"
#include "NicList.h"
#include "Capture.h"

using namespace std;

Pcap::Pcap(QObject *parent): QObject{parent}
{
    this->md = new Capture(this);
    this->tree_md = new TreeModel(this);
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

    QByteArray qba ((const char*)packet, header->len);
    dump_data st_dump = {};
    st_dump.data = qba;
    st_dump.header = *header;

     bool ret_invo_2 =
        QMetaObject::invokeMethod(p_this->p_Pcap,
                                              &Pcap::dump_push_back,
                                              Qt::QueuedConnection,
                                              st_dump);


    if(ret_invo_2 == false)
    {
        qDebug() << "Err invo_2";
    }

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
    this->set_stop_flag(false);
    this->set_wk_flag(true);

    this->md->reset();
    vec_dump_file.clear();
}

Q_INVOKABLE void Pcap::stop_Th()
{
    if(this->wk_pp == nullptr)
    {
        qDebug() << "Current Stoped";
        return;
    }

    pcap_breakloop(this->wk_pp);
    this->set_stop_flag(true);
    this->set_wk_flag(false);

    qDebug() << "Stop";
    if(this->stop_flag == true && this->wk_flag == false)
    {

    }
    else if(this->stop_flag == false && this->wk_flag == true)
    {

    }
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
    if(this->wk_pp == nullptr ||
        this->vec_dump.empty() || this->stop_flag == false)
    {
        return;
    }

    string localPath = QUrl(path).toLocalFile().toStdString();

    char errbuf[PCAP_ERRBUF_SIZE];
    memset(errbuf, 0, PCAP_ERRBUF_SIZE);

    pcap_dumper_t* p_dump_handle =
        pcap_dump_open(this->wk_pp, localPath.c_str());

    if(p_dump_handle == NULL)
    {
        qDebug() << "Err pcap_dump_open";
    }

    for(auto& v : this->vec_dump)
    {
        pcap_dump((u_char*)p_dump_handle,
                  &v.header,
                  (const u_char*)v.data.constData());
    }

    pcap_dump_close(p_dump_handle);

    return;
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

    QByteArray qba ((const char*)packet, header->len);
    dump_data st_dump = {};
    st_dump.data = qba;
    st_dump.header = *header;

    this->update_md(pkt);
    this->vec_dump.push_back(st_dump);
}

void Pcap::dump_push_back(dump_data st_dump)
{
    this->vec_dump.push_back(st_dump);
}

void Pcap::set_stop_flag(bool set)
{
    this->stop_flag = set;
    emit sig_stop_flag();
}

bool Pcap::get_stop_flag()
{
    return this->stop_flag;
}

bool Pcap::get_wk_flag ()
{
    return this->wk_flag;
}

void Pcap::set_wk_flag (bool set)
{
    this->wk_flag = set;
    emit sig_wk_flag();
}

QString Pcap::start_hax(int idx)
{
    dump_data d_data;
    if (vec_dump_file.empty())
        d_data = vec_dump.at(idx);
    else
        d_data = vec_dump_file.at(idx);

    int len = d_data.header.caplen;
    const u_char* data = (const u_char*)d_data.data.constData();

    QString out;

    for (int i = 0; i < len; i += 16)
    {
        // Offset (0000, 0010, ...)
        out += QString("%1   ").arg(i, 4, 16, QLatin1Char('0'));

        QString hexPart;
        QString asciiPart;

        // Build hex + ascii separately
        for (int j = 0; j < 16; j++)
        {
            if (i + j < len)
            {
                unsigned char c = data[i + j];

                // hex
                hexPart += QString("%1 ").arg(c, 2, 16, QLatin1Char('0'));

                // ascii
                if (c >= 32 && c <= 126)
                    asciiPart += QChar(c);
                else
                    asciiPart += ".";
            }
            else
            {
                // padding for missing bytes
                hexPart += "   ";
                asciiPart += " ";
            }
        }

        // hex는 16 * 3 = 48칸 → 고정 너비
        hexPart = hexPart.leftJustified(48, ' ');

        // hexPart + 공백 5칸 + asciiPart
        out += hexPart + "     " + asciiPart + "\n";
    }

    return out;
}

TreeModel* Pcap::get_tree_md()
{
    return this->tree_md;
}

Q_INVOKABLE void Pcap::start_tree_md(int idx)
{
    TreeItem* root = new TreeItem("root");
    this->tree_md->setPacketTree(root);

    dump_data data;
    if(!this->vec_dump_file.empty())
    {
        data = this->vec_dump_file.at(idx);
    }
    else
    {
        data = this->vec_dump.at(idx);
    }

    e_H* e_h = (e_H*)data.data.constData();

    QString dst = this->macToString((const u_char*)e_h->ether_dhost);
    QString src = this->macToString((const u_char*)e_h->ether_shost);
    QString e_tmp = QString("Ethernet II,     Src: %1,     Dst : %2").arg(dst).arg(src);

    TreeItem* ether = new TreeItem(e_tmp, root);
    root->addChild(ether);

    QString ch_dst = QString("Destination : (%1)").arg(dst);
    QString ch_src = QString("Source : (%1)").arg(src);
    QString ch_type = "";
    if(ntohs(e_h->ether_type) == 0x0800)
    {
        ch_type = "Type: IPv4 (0x0800)";
    }
    else if(ntohs(e_h->ether_type) == 0x086DD)
    {
        ch_type = "Type: IPv6 (0x086DD)";
    }
    else if(ntohs(e_h->ether_type) == 0x0806)
    {
        ch_type = "Type: ARP (0x0806)";
    }
    TreeItem* ether_ch_dst = new TreeItem(ch_dst, ether);
    TreeItem* ether_ch_src = new TreeItem(ch_src, ether);
    TreeItem* ether_ch_type = new TreeItem(ch_type, ether);

    ether->addChild(ether_ch_dst);
    ether->addChild(ether_ch_src);
    ether->addChild(ether_ch_type);

    return;
}

QString Pcap::macToString(const u_char* mac)
{
    return QString("%1:%2:%3:%4:%5:%6")
        .arg(mac[0], 2, 16, QLatin1Char('0'))
        .arg(mac[1], 2, 16, QLatin1Char('0'))
        .arg(mac[2], 2, 16, QLatin1Char('0'))
        .arg(mac[3], 2, 16, QLatin1Char('0'))
        .arg(mac[4], 2, 16, QLatin1Char('0'))
        .arg(mac[5], 2, 16, QLatin1Char('0'))
        .toUpper();
}


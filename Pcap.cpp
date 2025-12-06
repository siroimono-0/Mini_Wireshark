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

   e_H* e_header = (e_H*)packet;
    if (ntohs(e_header->ether_type) != 0x0800) {
        return; // IPv4 아니면 무시
    }

    ip_H* ip_header = (ip_H*)(packet + sizeof(e_H));


    if(ip_header->protocol == 6)
    {
        pkt.proto = "TCP";

        tcp_H* tcp_header =
            (tcp_H*)((u_char*)ip_header + ip_header->ihl * 4);

        QString qs_ret = "";

        // 🔹 포트
        int port_src = ntohs(tcp_header->source);
        int port_dst = ntohs(tcp_header->dest);

        qs_ret += QString::number(port_src);
        qs_ret += " ---> ";
        qs_ret += QString::number(port_dst);

        // 🔹 TCP 헤더 길이 (옵션 포함)
        uint8_t tcp_doff = tcp_header->doff;  // 4bit = "words"
        uint32_t tcp_header_len = tcp_doff * 4;

        // 🔹 Flags
        bool fin = tcp_header->fin;
        bool syn = tcp_header->syn;
        bool rst = tcp_header->rst;
        bool psh = tcp_header->psh;
        bool ack = tcp_header->ack;
        bool urg = tcp_header->urg;
        bool ece = tcp_header->ece;
        bool cwr = tcp_header->cwr;

        QString qs_flag = "   [ ";
        qs_flag += syn ? "SYN " : "";
        qs_flag += ack ? "ACK " : "";
        qs_flag += fin ? "FIN " : "";
        qs_flag += rst ? "RST " : "";
        qs_flag += psh ? "PSH " : "";
        qs_flag += urg ? "URG " : "";
        qs_flag += "]   ";

        qs_ret += qs_flag;

        // 🔹 Seq / Ack
        qs_ret += "Seq=";
        qs_ret += QString::number(ntohl(tcp_header->seq));

        qs_ret += "   Ack=";
        qs_ret += QString::number(ntohl(tcp_header->ack_seq));

        // 🔹 Window Size
        qs_ret += "   Win=";
        qs_ret += QString::number(ntohs(tcp_header->window));

        // 🔹 Payload Length (옵션 포함 계산)
        int payload_len =
            ntohs(ip_header->tot_len) -
                          (ip_header->ihl * 4) -
                          tcp_header_len;

        qs_ret += "   Len=";
        qs_ret += QString::number(payload_len);

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
        QString qs_len = QString::number(len - 8);

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

    int totalLen = ntohs(ip_header->tot_len) + sizeof(e_H);
    pkt.len = totalLen;


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

    e_H* e_header = (e_H*)packet;
    if (ntohs(e_header->ether_type) != 0x0800) {
        return; // IPv4 아니면 무시
    }

    ip_H* ip_header = (ip_H*)(packet + sizeof(e_H));

    if(ip_header->protocol == 6)
    {
        pkt.proto = "TCP";

        tcp_H* tcp_header =
            (tcp_H*)((u_char*)ip_header + ip_header->ihl * 4);

        QString qs_ret = "";

        // 🔹 포트
        int port_src = ntohs(tcp_header->source);
        int port_dst = ntohs(tcp_header->dest);

        qs_ret += QString::number(port_src);
        qs_ret += " ---> ";
        qs_ret += QString::number(port_dst);

        // 🔹 TCP 헤더 길이 (옵션 포함)
        uint8_t tcp_doff = tcp_header->doff;  // 4bit = "words"
        uint32_t tcp_header_len = tcp_doff * 4;

        // 🔹 Flags
        bool fin = tcp_header->fin;
        bool syn = tcp_header->syn;
        bool rst = tcp_header->rst;
        bool psh = tcp_header->psh;
        bool ack = tcp_header->ack;
        bool urg = tcp_header->urg;
        bool ece = tcp_header->ece;
        bool cwr = tcp_header->cwr;

        QString qs_flag = "   [ ";
        qs_flag += syn ? "SYN " : "";
        qs_flag += ack ? "ACK " : "";
        qs_flag += fin ? "FIN " : "";
        qs_flag += rst ? "RST " : "";
        qs_flag += psh ? "PSH " : "";
        qs_flag += urg ? "URG " : "";
        qs_flag += "]   ";

        qs_ret += qs_flag;

        // 🔹 Seq / Ack
        qs_ret += "Seq=";
        qs_ret += QString::number(ntohl(tcp_header->seq));

        qs_ret += "   Ack=";
        qs_ret += QString::number(ntohl(tcp_header->ack_seq));

        // 🔹 Window Size
        qs_ret += "   Win=";
        qs_ret += QString::number(ntohs(tcp_header->window));

        // 🔹 Payload Length (옵션 포함 계산)
        int payload_len =
            ntohs(ip_header->tot_len) -
                          (ip_header->ihl * 4) -
                          tcp_header_len;

        qs_ret += "   Len=";
        qs_ret += QString::number(payload_len);

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
        QString qs_len = QString::number(len - 8);

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


    QByteArray qba ((const char*)packet, header->len);
    dump_data st_dump = {};
    st_dump.data = qba;
    st_dump.header = *header;

    int totalLen = ntohs(ip_header->tot_len) + sizeof(e_H);
    pkt.len = totalLen;
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

    //==============================Ether==============================
    //==============================Ether==============================
    u_char* pkt = (u_char*)data.data.constData();
    e_H* e_h = (e_H*)pkt;

    QString dst = this->macToString((const u_char*)e_h->ether_dhost);
    QString src = this->macToString((const u_char*)e_h->ether_shost);
    QString e_tmp = QString("Ethernet II,    Src: %1,    Dst : %2").arg(dst).arg(src);

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
    //==============================Ether==============================
    //==============================Ether==============================

    //==============================ip==============================
    //==============================ip==============================
    ip_H* ip_h = (ip_H*)(pkt + sizeof(e_H));
    QString src_ip = inet_ntoa(*(struct in_addr*)&ip_h->saddr);
    QString dst_ip = inet_ntoa(*(struct in_addr*)&ip_h->daddr);

    QString ip_tmp = QString("Internet Protocol Version %1,    Src: %2,    Dst: %3")
                         .arg(ip_h->version).arg(src_ip).arg(dst_ip);

    TreeItem* ip_root = new TreeItem(ip_tmp , root);
    root->addChild(ip_root);
    //==============================================================
    QString qs_ipVersion = QString("Version: %1").arg(ip_h->version);
    TreeItem* ip_version = new TreeItem(qs_ipVersion, ip_root);
    ip_root->addChild(ip_version);

    //==============================================================
    QString qs_ipHeaderLen = QString("Header Length: %1    bytes (%2)")
                                 .arg(ip_h->ihl * 4).arg(ip_h->ihl);

    TreeItem* ip_ipHeaderLen = new TreeItem(qs_ipHeaderLen, ip_root);
    ip_root->addChild(ip_ipHeaderLen);
    //==============================================================
    uint8_t tos = ip_h->tos;
    uint8_t dscp = tos >> 2;
    uint8_t ecn = tos & 0x03;
    QString qs_tmp_sdcp = "";
    QString qs_tmp_ecn = "";

    if(dscp >= 0 && dscp <= 7)
    {
        qs_tmp_sdcp = "CS0";
    }
    else if(dscp >= 8 && dscp <= 15)
    {
        qs_tmp_sdcp = "CS1";
    }
    else if(dscp >= 16 && dscp <= 23)
    {
        qs_tmp_sdcp = "CS2";
    }
    else if(dscp >= 24 && dscp <= 31)
    {
        qs_tmp_sdcp = "CS3";
    }
    else if(dscp >= 32 && dscp <= 39)
    {
        qs_tmp_sdcp = "CS4";
    }
    else if(dscp >= 40 && dscp <= 47)
    {
        qs_tmp_sdcp = "CS5";
    }
    else if(dscp >= 48 && dscp <= 55)
    {
        qs_tmp_sdcp = "CS6";
    }
    else if(dscp >= 56 && dscp <= 63)
    {
        qs_tmp_sdcp = "CS7";
    }

    if(ecn == 0)
    {
        qs_tmp_ecn  = "Not-ECT";
    }
    else if(ecn == 2)
    {
        qs_tmp_ecn  = "ECT(0)";
    }
    else if(ecn == 1)
    {
        qs_tmp_ecn  = "ECT(1)";
    }
    else if(ecn == 3)
    {
        qs_tmp_ecn  = "CE";
    }

    QString qs_hex_tos = QString("0x%1")
                             .arg(QString::number(tos, 16)
                                      .toUpper()
                                      .rightJustified(2, '0'));

    QString qs_tos =
        QString("Differentiated Services Field: %1 (DSCP: %2, ECN: %3)")
                         .arg(qs_hex_tos)
                         .arg(qs_tmp_sdcp)
                         .arg(qs_tmp_ecn);

    /*
    QString qs_tos =
        QString("Differentiated Services Field: %1    (DSCP: %2,    ECN: %3)")
                         .arg(ip_h->version).arg(qs_tmp_sdcp).arg(qs_tmp_ecn);
    */

    TreeItem* ip_tos = new TreeItem(qs_tos, ip_root);
    ip_root->addChild(ip_tos);

    QString  qs_tos_ch1 =
        QString("Differentiated Services Codepoint: %1 (%2)")
                             .arg(qs_tmp_sdcp).arg(dscp);
    TreeItem* ip_tos_ch1 = new TreeItem(qs_tos_ch1, ip_tos);

    ip_tos->addChild(ip_tos_ch1);

    QString  qs_tos_ch2 =
        QString("Explicit Congestion Notification: %1 (%2)")
                             .arg(qs_tmp_ecn).arg(ecn);
    TreeItem* ip_tos_ch2 = new TreeItem(qs_tos_ch2, ip_tos);

    ip_tos->addChild(ip_tos_ch2);
    //==============================================================

    QString qs_totalLen =
        QString("Total Length: %1").arg(ntohs(ip_h->tot_len));
    TreeItem* ip_totalLen = new TreeItem(qs_totalLen, ip_root);
    ip_root->addChild(ip_totalLen);
    //==============================================================

    uint16_t id = ntohs(ip_h->id);

    QString qs_id = QString("Identification: 0x%1 (%2)")
                        .arg(id, 4, 16, QLatin1Char('0'))  // hex 4자리, zero-padding
                        .arg(id);                         // decimal

    TreeItem* ip_id = new TreeItem(qs_id, ip_root);
    ip_root->addChild(ip_id);
    //==============================================================

    uint16_t frag = ntohs(ip_h->frag_off);
    uint8_t flags = (frag & 0xE000) >> 13;
    uint8_t offset = frag & 0x1FFF;

    QString flagMeaning;

    if(flags & 0x2)
    {
        flagMeaning = "Don't fragment";
    }
    else if(flags & 0x1)
    {
        flagMeaning = "More fragments";
    }
    else
    {
        flagMeaning = "None";
    }

    QString qs_flag = QString("Flags: 0x%1, %2")
                          .arg(flags, 0, 16)        // hex
                          .arg(flagMeaning);

    TreeItem* ip_flag = new TreeItem(qs_flag, ip_root);
    ip_root->addChild(ip_flag);

    QString tmp_flag_1 = "";
    if((flags & 0x04) == 0)
    {
        tmp_flag_1 = "Not Set";
    }
    else
    {
        tmp_flag_1 = "Set";
    }

    QString tmp_flag_2 = "";
    if((flags & 0x02) == 0)
    {
        tmp_flag_2 = "Not Set";
    }
    else
    {
        tmp_flag_2 = "Set";
    }

    QString tmp_flag_3 = "";
    if((flags & 0x01) == 0)
    {
        tmp_flag_3 = "Not Set";
    }
    else
    {
        tmp_flag_3 = "Set";
    }

    QString RF = QString("Reserved bit: %1").arg(tmp_flag_1);
    QString DF = QString("Don't fragment: %1").arg(tmp_flag_2);
    QString MF = QString("More fragments: %1").arg(tmp_flag_3);

    TreeItem* ip_flag_RF = new TreeItem(RF, ip_flag);
    TreeItem* ip_flag_DF = new TreeItem(DF, ip_flag);
    TreeItem* ip_flag_MF = new TreeItem(MF, ip_flag);

    ip_flag->addChild(ip_flag_RF);
    ip_flag->addChild(ip_flag_DF);
    ip_flag->addChild(ip_flag_MF);
    //==============================================================
    QString qs_ttl =
        QString("Time to Live: %1").arg(ip_h->ttl);
    TreeItem* ip_ttl = new TreeItem(qs_ttl, ip_root);
    ip_root->addChild(ip_ttl);
    //==============================================================
    // QString qs_check_stat = QString("[Header checksum status: %1]").arg(qs_ret_check);
    // TreeItem* ip_check_stat = new TreeItem(qs_check_stat, ip_root);
    // ip_root->addChild(ip_check_stat );

    uint8_t proto = ip_h->protocol;
    QString qs_proto = "";

    if(proto == 6)
    {
        qs_proto = "TCP (6)";
    }
    else if(proto == 17)
    {
        qs_proto = "UDP (17)";
    }
    else if(proto == 1)
    {
        qs_proto = "ICMP (1)";
    }

    QString qs_proto_2 = QString("Protocol: %1").arg(qs_proto);

    TreeItem* ip_proto = new TreeItem(qs_proto_2, ip_root);
    ip_root->addChild(ip_proto);
    //==============================================================

    uint16_t raw_checksum = ntohs(ip_h->check);
    QString qs_ret_check = "";

    int ret_check = compute_ip_checksum((const uint8_t*)ip_h, ip_h->ihl * 4);

    if(ret_check == 0)
    {
        qs_ret_check = "Good";
    }
    else
    {
        qs_ret_check = "Bad";
    }

    QString qs_checksum = QString("Header Checksum: 0x%1 [%2]")
                              .arg(raw_checksum, 4, 16, QLatin1Char('0')).arg(qs_ret_check);

    TreeItem* ip_check = new TreeItem(qs_checksum, ip_root);
    ip_root->addChild(ip_check);

    QString qs_check_stat = QString("[Header checksum status: %1]").arg(qs_ret_check);
    TreeItem* ip_check_stat = new TreeItem(qs_check_stat, ip_root);
    ip_root->addChild(ip_check_stat );
    //==============================================================
    QString tmp_ipSrc = inet_ntoa(*(struct in_addr*)&ip_h->saddr);
    QString qs_ipSrc = QString("Source Address: %1").arg(tmp_ipSrc);
    TreeItem* ip_src = new TreeItem(qs_ipSrc, ip_root);
    ip_root->addChild(ip_src);

    QString tmp_ipDst = inet_ntoa(*(struct in_addr*)&ip_h->daddr);
    QString qs_ipDst = QString("Destination Address: %1").arg(tmp_ipDst);
    TreeItem* ip_Dst = new TreeItem(qs_ipDst, ip_root);
    ip_root->addChild(ip_Dst);
    //==============================ip==============================
    //==============================ip==============================

    //==============================tcp==============================
    //==============================tcp==============================

    int e_h_size = sizeof(e_H);
    if(proto == 6)
    {
        tcp_H* tcp_h = (tcp_H*)(pkt + e_h_size + (ip_h->ihl * 4));

        int ip_total = ntohs(ip_h->tot_len);
        int ip_header_len = ip_h->ihl * 4;
        int tcp_header_len = tcp_h->doff* 4;
        int tcp_payload = ip_total - ip_header_len - tcp_header_len;

        QString tmp_tcpSrc = QString::number(ntohs(tcp_h->source));
        QString tmp_tcpDst = QString::number(ntohs(tcp_h->dest));
        QString tmp_seq = QString::number(ntohl(tcp_h->seq));
        QString tmp_ack = QString::number(ntohl(tcp_h->ack_seq));
        QString tmp_tcpLen = QString::number(tcp_payload);

        QString qs_tcp_root =
            QString("Transmission Control Protocol, Src Port: %1,"
                                      " Dst Port: %2, Seq: %3, Ack: %4, Len: %5")
                                  .arg(tmp_tcpSrc).arg(tmp_tcpDst).arg(tmp_seq)
                                  .arg(tmp_ack).arg(tmp_tcpLen);
        TreeItem* tcp_root = new TreeItem(qs_tcp_root, root);
        root->addChild(tcp_root);

        //=============================================================
        QString qs_tcpSrc = QString("Source Port: %1").arg(tmp_tcpSrc);
        TreeItem* tcp_src = new TreeItem(qs_tcpSrc, tcp_root);
        tcp_root->addChild(tcp_src);
        //=============================================================

        //=============================================================
        QString qs_tcpDst = QString("Destination Port: %1").arg(tmp_tcpDst);
        TreeItem* tcp_dst = new TreeItem(qs_tcpDst, tcp_root);
        tcp_root->addChild(tcp_dst);
        //=============================================================

        //=============================================================
        QString qs_tcpLen = QString("[Tcp Segment Len: %1]").arg(tmp_tcpLen);
        TreeItem* tcp_len = new TreeItem(qs_tcpLen, tcp_root);
        tcp_root->addChild(tcp_len);
        //=============================================================

        //=============================================================
        QString qs_tcpSeq = QString("Sequence Number: %1").arg(tmp_seq);
        TreeItem* tcp_seq = new TreeItem(qs_tcpSeq, tcp_root);
        tcp_root->addChild(tcp_seq);
        //=============================================================

        //=============================================================
        QString qs_ackseq = QString("Acknowledgment Number: %1").arg(tmp_ack);
        TreeItem* tcp_ack = new TreeItem(qs_ackseq, tcp_root);
        tcp_root->addChild(tcp_ack);
        //=============================================================


    }
    else if(proto == 17)
    {
    }
    else if(proto == 1)
    {
    }


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

uint16_t Pcap::compute_ip_checksum(const uint8_t* data, int header_len)
{
    uint32_t sum = 0;

    // 16비트씩 더함
    for(int i = 0; i < header_len; i += 2)
    {
        uint16_t word = (data[i] << 8) | data[i + 1];
        sum += word;
    }

    // carry 처리
    while(sum >> 16)
        sum = (sum & 0xFFFF) + (sum >> 16);

    return ~sum & 0xFFFF;
}


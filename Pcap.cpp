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
    }
    else if(ip_header->protocol == 17)
    {
        pkt.proto = "UDP";
    }
    else if(ip_header->protocol == 1)
    {
        pkt.proto = "ICMP";
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

    qDebug() << Q_FUNC_INFO;


    pcap_loop(this->pp, -1, my_packet_handler ,(u_char*)this);
    qDebug() << Q_FUNC_INFO;

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

//=============================================================================
//=============================================================================

Q_INVOKABLE void Pcap::create_Th(QVariant qba)
{
    QString qs = qba.toString();
    string str = qs.toStdString();

    QThread* th = new QThread;
    Worker* wk = new Worker();
    this->p_Wk = wk;
    wk->set_nicName(str);
    wk->set_p_Pcap(this);


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


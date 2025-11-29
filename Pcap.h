#ifndef PCAP_H
#define PCAP_H

#include <QObject>
#include <QVariant>
#include <QThread>
//=====================================================================
#include "Struct_in.h"
#include <pcap.h>
#include <net/ethernet.h>   // struct ether_header
#include <netinet/if_ether.h>
#include <netinet/ip.h>
#include <string>



class Pcap;
class Capture;

class Worker: public QObject
{
    Q_OBJECT

public:
    explicit  Worker(QObject *parent = nullptr);

    static void my_packet_handler( u_char *user,
                                  const struct pcap_pkthdr *header,
                                  const u_char * packet);

    int set_Capture();

    void set_nicName(std::string set);
    void set_p_Pcap(Pcap* set);

public slots:

private:
    pcap_t* pp;
    int num = 0;
    std::string nicName;

    Pcap* p_Pcap = nullptr;
signals:
    void sig_end();
};


//========================================================================
//========================================================================
class Pcap : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Capture* md READ get_md NOTIFY sig_md FINAL)

public:
    explicit Pcap(QObject *parent = nullptr);

    Q_INVOKABLE void create_Th(QVariant qba);
    Q_INVOKABLE void stop_Th();

    void set_wk_pp(pcap_t* set);
    void update_md(st_pkt pkt);

//========================================================================
    Capture* get_md();
//========================================================================

public slots:

private:
    bool wk_life = false;
    Worker* p_Wk = nullptr;
    pcap_t* wk_pp = nullptr;

    Capture* md = nullptr;

signals:
    void sig_md();
};

#endif // PCAP_H


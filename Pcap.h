#ifndef PCAP_H
#define PCAP_H

#include <QObject>
#include <QVariant>
#include <QThread>
#include <QUrl>
//=====================================================================
#include "Struct_in.h"
#include "TreeModel.h"
#include <pcap.h>
#include <net/ethernet.h>   // struct ether_header
#include <netinet/if_ether.h>
#include <netinet/ip.h>
#include <string>


typedef struct dump_data
{
    pcap_pkthdr header;
    QByteArray data;

}dump_data;

class Pcap;
class Capture;
class TreeModel;

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
    void set_BFS_ft(std::string set);

public slots:

private:
    pcap_t* pp;
    int num = 0;
    std::string nicName;
    std::string BFS_ft;

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
    Q_PROPERTY(TreeModel* tree_md READ get_tree_md
                   NOTIFY sig_tree_md  FINAL)

    Q_PROPERTY(bool stop_flag READ get_stop_flag WRITE set_stop_flag
               NOTIFY sig_stop_flag FINAL)
    Q_PROPERTY(bool wk_flag READ get_wk_flag WRITE set_wk_flag
               NOTIFY sig_wk_flag FINAL)

public:
    explicit Pcap(QObject *parent = nullptr);

    Q_INVOKABLE void create_Th(QVariant qba, QVariant ft);
    Q_INVOKABLE void stop_Th();
    Q_INVOKABLE void reset_md();

    Q_INVOKABLE void pcapFile_Read(QString path);

    Q_INVOKABLE void save_md(QString path);
    Q_INVOKABLE QString start_hax(int idx);

    void set_wk_pp(pcap_t* set);
    void set_wk_life(bool set);
    void dump_push_back(dump_data st_dump);

    //========================================================================
    bool get_stop_flag();
    void set_stop_flag(bool set);
    //========================================================================
    bool get_wk_flag ();
    void set_wk_flag (bool set);
    //========================================================================
    TreeModel* get_tree_md();
    Q_INVOKABLE void start_tree_md(int idx);
    QString macToString(const u_char* mac);
    uint16_t compute_ip_checksum(const uint8_t* data, int header_len);

    //========================================================================
    Capture* get_md();
    void update_md(st_pkt pkt);
    //========================================================================
    void packet_func( u_char *user,
                     const struct pcap_pkthdr *header,
                     const u_char * packet);
    //========================================================================

    //========================================================================
public slots:

private:
    bool wk_life = false;
    Worker* p_Wk = nullptr;
    pcap_t* wk_pp = nullptr;

    Capture* md = nullptr;
    TreeModel* tree_md = nullptr;

    //========================================================================
    pcap_t* p_pcap = nullptr;
    int num = 0;
    //========================================================================
    QVector<dump_data> vec_dump;
    QVector<dump_data> vec_dump_file;
    bool stop_flag = false;
    bool wk_flag = false;
    //========================================================================
signals:
    void sig_md();
    void sig_tree_md();
    void sig_stop_flag();
    void sig_wk_flag();

};

#endif // PCAP_H


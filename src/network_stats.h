#ifndef NETWORK_STATS_H
#define NETWORK_STATS_H

#include <string>
#include <map>

struct InterfaceStats {
    unsigned long long rx_bytes;
    unsigned long long tx_bytes;
    unsigned long long rx_packets;
    unsigned long long tx_packets;
};

class NetworkStats {
public:
    NetworkStats();
    bool update();
    std::map<std::string, InterfaceStats> getStats() const;
    std::string formatStats() const;

private:
    std::map<std::string, InterfaceStats> stats_;
    bool parseNetDev();
};

#endif

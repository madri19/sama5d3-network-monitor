/*network_stats.cpp/h
Reads /proc/net/dev (kernel network statistics file)
Parses it to extract RX/TX bytes and packets for each interface
Stores in a map: interface_name → stats
*/

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
    bool update(); // refresh stats from /proc
    std::map<std::string, InterfaceStats> getStats() const;
    std::string formatStats() const; // format as readable text

private:
    std::map<std::string, InterfaceStats> stats_; // storage
    bool parseNetDev(); // parse it
};

#endif

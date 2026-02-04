#include "network_stats.h"
#include <fstream>
#include <sstream>
#include <iomanip>

NetworkStats::NetworkStats() {
    update();
}

bool NetworkStats::update() {
    return parseNetDev();
}

bool NetworkStats::parseNetDev() {
    std::ifstream file("/proc/net/dev");
    if (!file.is_open()) {
        return false;
    }

    stats_.clear();
    std::string line;
    
    // Skip header lines
    std::getline(file, line);
    std::getline(file, line);

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string iface;
        InterfaceStats stats;

        // Parse interface name (format: "eth0:")
        iss >> iface;
        if (iface.empty()) continue;
        
        // Remove trailing colon
        if (iface.back() == ':') {
            iface.pop_back();
        }

        // Parse RX bytes, packets, and skip other RX fields
        iss >> stats.rx_bytes >> stats.rx_packets;
        
        // Skip remaining RX fields (errors, drop, fifo, frame, compressed, multicast)
        for (int i = 0; i < 6; i++) {
            unsigned long long dummy;
            iss >> dummy;
        }
        
        // Parse TX bytes and packets
        iss >> stats.tx_bytes >> stats.tx_packets;

        stats_[iface] = stats;
    }

    return !stats_.empty();
}

std::map<std::string, InterfaceStats> NetworkStats::getStats() const {
    return stats_;
}

std::string NetworkStats::formatStats() const {
    std::ostringstream oss;
    oss << "Network Interface Statistics\n";
    oss << "=============================\n\n";

    for (const auto& entry : stats_) {
        oss << "Interface: " << entry.first << "\n";
        oss << "  RX: " << std::setw(12) << entry.second.rx_bytes << " bytes, "
            << std::setw(8) << entry.second.rx_packets << " packets\n";
        oss << "  TX: " << std::setw(12) << entry.second.tx_bytes << " bytes, "
            << std::setw(8) << entry.second.tx_packets << " packets\n";
        oss << "\n";
    }

    return oss.str();
}

// control.h

#ifndef CONTROL_H
#define CONTROL_H

#include <vector>
#include <string>
#include <map>
#include <ns3/net-device.h>

struct Node {
    int id;
    double energy;
    bool isMalicious;
    double trustScore;
};

struct Packet {
    int src;
    int dest;
    std::vector<int> path;
};

class SDNController {
public:
    SDNController();
    ~SDNController();

    void initializeNetwork(int numNodes);
    void evaluateTrust();
    void updateRouting();
    void handlePacket(Packet &pkt);
    void logParameters(const std::string &filename);

    // New function to enable PCAP tracing
    void EnablePcapTracing();

private:
    std::vector<Node> nodes;
    std::map<int, std::vector<int>> routingTable;
    std::vector<ns3::Ptr<ns3::NetDevice>> devices;  // List of network devices

    double computeCommunicationTrust(const Node &node);
    double computeNodeTrust(const Node &node);
    double computeEnvironmentTrust(const Node &node);
    std::vector<int> getOptimalPath(int src, int dest);
    double runLSTMModel(const std::vector<double> &trustMetrics);
};

#endif  // CONTROL_H

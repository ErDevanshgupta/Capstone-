// control.h

#ifndef CONTROL_H
#define CONTROL_H

#include <vector>
#include <string>
#include <map>

// Define Node structure
struct Node {
    int id;
    double energy;
    double x, y, z; // Coordinates
    bool isMalicious;
    double trustScore;
};

// Define Packet structure
struct Packet {
    int src;
    int dest;
    std::vector<int> path;
};

// SDN Controller Class
class SDNController {
public:
    SDNController();
    ~SDNController();

    void initializeNetwork(int numNodes);
    void evaluateTrust();
    void updateRouting();
    void handlePacket(Packet &pkt);
    void logToPCAP(const std::string &filename, const Packet &pkt);

private:
    std::vector<Node> nodes;
    std::map<int, std::vector<int>> routingTable; // Node ID -> List of neighbor IDs

    double computeCommunicationTrust(const Node &node);
    double computeNodeTrust(const Node &node);
    double computeEnvironmentTrust(const Node &node);
    std::vector<int> getOptimalPath(int src, int dest);
    double runLSTMModel(const std::vector<double> &trustMetrics);
    double computeReward(int src, int dest, const std::vector<int> &path);
};

#endif // CONTROL_H

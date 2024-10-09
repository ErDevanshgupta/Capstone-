#ifndef CONTROL_H
#define CONTROL_H

#include <vector>
#include <string>
#include <map>
#include <ns3/net-device.h>
#include <ns3/net-device-container.h>
#include <ns3/csma-helper.h>
#include <ns3/ptr.h>
#include <ns3/node-container.h>
#include <ns3/ipv4-address-helper.h>
#include <ns3/internet-stack-helper.h>
#include <ns3/v4ping-helper.h>

NetDeviceContainer initializeNetwork(int numNodes);

// Node structure representing individual network nodes
struct Node {
    int id;
    double energy;
    double trustScore;
    bool isMalicious;
};

// Packet structure for simulating network packets
struct Packet {
    int src;
    int dest;
    std::vector<int> path;
};

// SDNController class for managing trust and routing decisions
class SDNController {
public:
    SDNController();
    ~SDNController();

    // Initialize the network with a given number of nodes
    void initializeNetwork(int numNodes);



    // Evaluate trust for all nodes
    void evaluateTrust();

    // Update routing table based on trust values
    void updateRouting();

    // Handle packet forwarding and logging
    void handlePacket(Packet &pkt);

    // Enable PCAP tracing for network devices
    void EnablePcapTracing();

private:
    std::vector<Node> customNodes;  // Custom nodes for trust calculation
    std::map<int, std::vector<int>> routingTable; // Node ID -> list of neighbors
    std::vector<ns3::Ptr<ns3::NetDevice>> devices; // Store network devices

    // Trust evaluation functions
    double computeCommunicationTrust(const Node &node);
    double computeNodeTrust(const Node &node);
    double computeEnvironmentTrust(const Node &node);
    double computeBehaviorTrust(const Node &node); // Add this declaration

    // Routing function to find the optimal path
    std::vector<int> getOptimalPath(int src, int dest);

    // Placeholder for LSTM model implementation
    double runLSTMModel(const std::vector<double> &trustMetrics);
};

#endif // CONTROL_H

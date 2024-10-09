#ifndef CONTROL_H
#define CONTROL_H

#include <vector>
#include <string>
#include <map>
#include <ns3/net-device.h>

// Node structure representing individual network nodes
struct Node {
    int id;
    double energy;
    double trustScore;
    bool isMalicious;
};

// SDNController class for managing trust and routing decisions
class SDNController {
public:
    SDNController();
    ~SDNController();

    // Initialize the network with a given number of nodes
    void initializeNetwork(int numNodes);

    // Enable PCAP tracing for network devices
    void EnablePcapTracing();

    // Evaluate trust for all nodes
    void evaluateTrust();

private:
    std::vector<Node> nodes;
    std::vector<ns3::Ptr<ns3::NetDevice>> devices; // Store network devices

    // Trust evaluation functions
    double computeNodeTrust(const Node &node);
    double computeCommunicationTrust(const Node &node);
    double computeBehaviorTrust(const Node &node);
    double computeEnvironmentTrust(const Node &node);
};

#endif // CONTROL_H

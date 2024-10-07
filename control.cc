#include "control.h"
#include <ns3/core-module.h>
#include <ns3/csma-helper.h>
#include <ns3/internet-module.h>
#include <iostream>
#include <fstream>

SDNController::SDNController() {
    // Constructor implementation
}

SDNController::~SDNController() {
    // Destructor implementation
}

void SDNController::initializeNetwork(int numNodes) {
    NodeContainer nodes;
    nodes.Create(numNodes);

    // Use CsmaHelper for wired connections (or AquaSimHelper for UASNs)
    ns3::CsmaHelper csmaHelper;
    csmaHelper.SetChannelAttribute("DataRate", DataRateValue(DataRate("100Mbps")));
    csmaHelper.SetChannelAttribute("Delay", TimeValue(MilliSeconds(2)));

    // Install devices on all nodes
    NetDeviceContainer netDevices = csmaHelper.Install(nodes);

    // Add devices to the vector for PCAP tracing
    for (uint32_t i = 0; i < netDevices.GetN(); ++i) {
        devices.push_back(netDevices.Get(i));
    }

    // Install Internet stack (IP, routing, etc.) on the nodes
    InternetStackHelper internet;
    internet.Install(nodes);

    // Assign IP addresses to devices
    Ipv4AddressHelper ipv4;
    ipv4.SetBase("10.1.1.0", "255.255.255.0");
    ipv4.Assign(netDevices);
}

    // Set up basic routing (neighbor discovery)
    for (auto &node : nodes) {
        routingTable[node.id] = { (node.id + 1) % numNodes }; // Simple circular routing
    }
}

void SDNController::evaluateTrust() {
    for (auto &node : nodes) {
        if (!node.isMalicious) {
            double commTrust = computeCommunicationTrust(node);
            double nodeTrust = computeNodeTrust(node);
            double envTrust = computeEnvironmentTrust(node);
            std::vector<double> trustMetrics = { commTrust, nodeTrust, envTrust };
            node.trustScore = runLSTMModel(trustMetrics); // Using LSTM to compute trust score
        } else {
            node.trustScore = 0.0;
        }
    }
}

double SDNController::computeCommunicationTrust(const Node &node) {
    return 0.8; // Placeholder: Simulate communication trust
}

double SDNController::computeNodeTrust(const Node &node) {
    return node.energy / 1000.0; // Trust based on remaining energy
}

double SDNController::computeEnvironmentTrust(const Node &node) {
    return 0.9; // Placeholder: Simulate environment trust
}

double SDNController::runLSTMModel(const std::vector<double> &trustMetrics) {
    return 0.5 * trustMetrics[0] + 0.3 * trustMetrics[1] + 0.2 * trustMetrics[2]; // Placeholder for LSTM
}

std::vector<int> SDNController::getOptimalPath(int src, int dest) {
    return { src, dest }; // Simplified direct routing
}

void SDNController::handlePacket(Packet &pkt) {
    std::cout << "Handling packet from " << pkt.src << " to " << pkt.dest << std::endl;
}

void SDNController::EnablePcapTracing() {
    ns3::CsmaHelper csmaHelper;

    if (devices.empty()) {
        std::cerr << "No devices initialized for PCAP tracing!" << std::endl;
        return;
    }

    for (size_t i = 0; i < devices.size(); ++i) {
        std::string filename = "/home/capstone/trace/devansh/trace/node" + std::to_string(i) + ".pcap";
        csmaHelper.EnablePcap(filename, devices[i], true);
        std::cout << "PCAP logging enabled for node " << i << " -> " << filename << std::endl;
    }
}


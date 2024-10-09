#include "control.h"
#include <ns3/csma-helper.h>
#include <ns3/internet-module.h>
#include <ns3/net-device.h>
#include <ns3/ipv4-address-helper.h>
#include <iostream>
#include <cmath> // For trust calculations

SDNController::SDNController() {
    // Constructor implementation
}

SDNController::~SDNController() {
    // Destructor implementation
}

void SDNController::initializeNetwork(int numNodes) {
    ns3::NodeContainer nodes;
    nodes.Create(numNodes);

    // Use CsmaHelper for wired connections (or AquaSimHelper for UASNs)
    ns3::CsmaHelper csmaHelper;
    csmaHelper.SetChannelAttribute("DataRate", ns3::DataRateValue(ns3::DataRate("100Mbps")));
    csmaHelper.SetChannelAttribute("Delay", ns3::TimeValue(ns3::MilliSeconds(2)));

    // Install devices on all nodes
    ns3::NetDeviceContainer netDevices = csmaHelper.Install(nodes);

    // Add devices to the vector for PCAP tracing
    for (uint32_t i = 0; i < netDevices.GetN(); ++i) {
        devices.push_back(netDevices.Get(i));
    }

    // Install Internet stack (IP, routing, etc.) on the nodes
    ns3::InternetStackHelper internet;
    internet.Install(nodes);

    // Assign IP addresses to devices
    ns3::Ipv4AddressHelper ipv4;
    ipv4.SetBase("10.1.1.0", "255.255.255.0");
    ipv4.Assign(netDevices);

    // Initialize nodes for trust evaluation
    for (int i = 0; i < numNodes; ++i) {
        Node node;
        node.id = i;
        node.energy = 100.0; // Initial energy value (could be dynamic)
        node.trustScore = 0.5; // Initialize trust score to neutral value
        node.isMalicious = false;
        nodes.push_back(node);
    }
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

void SDNController::evaluateTrust() {
    for (Node &node : nodes) {
        node.trustScore = computeNodeTrust(node);
        std::cout << "Node " << node.id << " trust score: " << node.trustScore << std::endl;
    }
}

double SDNController::computeNodeTrust(const Node &node) {
    // Weighting factors
    double alpha = 0.4, beta = 0.3, gamma = 0.3;

    double T_comm = computeCommunicationTrust(node); // Communication trust
    double T_behave = computeBehaviorTrust(node); // Node behavior trust
    double T_env = computeEnvironmentTrust(node); // Environmental trust

    // Total trust score as weighted sum of all factors
    return alpha * T_comm + beta * T_behave + gamma * T_env;
}

double SDNController::computeCommunicationTrust(const Node &node) {
    // Example: Based on packet delivery success (simulated as a random value)
    double deliverySuccessRate = 0.9; // Example fixed value, this should come from the network stats
    return deliverySuccessRate; // Return value between 0 and 1
}

double SDNController::computeBehaviorTrust(const Node &node) {
    // Example: Based on packet drops or malicious activity (simulated here)
    if (node.isMalicious) {
        return 0.1; // Low trust for malicious nodes
    }
    return 0.8; // Example for well-behaved nodes
}

double SDNController::computeEnvironmentTrust(const Node &node) {
    // Example: Based on node energy
    double maxEnergy = 100.0;
    return node.energy / maxEnergy; // Return value between 0 and 1
}

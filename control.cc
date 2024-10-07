#include "control.h"
#include <ns3/csma-helper.h>
#include <ns3/internet-module.h>
#include <ns3/net-device.h>
#include <ns3/ipv4-address-helper.h>
#include <ns3/ofswitch13-helper.h>  // Include OFSwitch13Helper header
#include <iostream>

// Constructor
SDNController::SDNController() {
    // Constructor implementation
}

// Destructor
SDNController::~SDNController() {
    // Destructor implementation
}

void SDNController::initializeNetwork(int numNodes) {
    // Create nodes container
    ns3::NodeContainer nodes;
    nodes.Create(numNodes);

    // Use OFSwitch13Helper for OpenFlow switches
    ns3::OFSwitch13Helper ofHelper;

    // Create controllers and switches
    ns3::NodeContainer controllerNodes;
    ns3::NodeContainer switchNodes;
    controllerNodes.Create(1);  // Assuming 1 controller for now
    switchNodes.Create(1);  // Assuming 1 switch for now

    // Create the switch and controller devices
    ns3::NetDeviceContainer switchDevices;
    ns3::NetDeviceContainer controllerDevices;

    // Use CsmaHelper for wired connections between switches and controller
    ns3::CsmaHelper csmaHelper;
    csmaHelper.SetChannelAttribute("DataRate", ns3::DataRateValue(ns3::DataRate("100Mbps")));
    csmaHelper.SetChannelAttribute("Delay", ns3::TimeValue(ns3::MilliSeconds(2)));

    // Install devices on the switch and controller nodes
    switchDevices = csmaHelper.Install(switchNodes);
    controllerDevices = csmaHelper.Install(controllerNodes);

    // Populate devices vector with installed devices
    for (uint32_t i = 0; i < switchNodes.GetN(); ++i) {
        devices.push_back(switchDevices.Get(i));
    }

    // Install Internet stack on the nodes
    ns3::InternetStackHelper internet;
    internet.Install(nodes);

    // Assign IP addresses to devices
    ns3::Ipv4AddressHelper ipv4;
    ipv4.SetBase("10.1.1.0", "255.255.255.0");
    ipv4.Assign(switchDevices);

    // Create OpenFlow switch and controller
    Ptr<ns3::OFSwitch13InternalHelper> ofSwitchHelper = CreateObject<ns3::OFSwitch13InternalHelper>();
    Ptr<ns3::OFSwitch13LearningController> controller = CreateObject<ns3::OFSwitch13LearningController>();

    // Install controller on controller node
    ofSwitchHelper->InstallController(controllerNodes.Get(0), controller);

    // Install OpenFlow switch on switch node
    ofSwitchHelper->InstallSwitch(switchNodes.Get(0), switchDevices.Get(0));

    // Create the OpenFlow channel between the switch and controller
    ofSwitchHelper->CreateOpenFlowChannels();
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

double SDNController::computeCommunicationTrust(const Node &node) {
    // Placeholder: Implement communication trust computation
    return 0.8;  // Example value
}

double SDNController::computeNodeTrust(const Node &node) {
    // Placeholder: Implement node trust computation
    return 0.9;  // Example value
}

double SDNController::computeEnvironmentTrust(const Node &node) {
    // Placeholder: Implement environment trust computation
    return 0.85;  // Example value
}

double SDNController::runLSTMModel(const std::vector<double> &trustMetrics) {
    // Placeholder: Implement LSTM model for decision-making
    // Combine trust metrics using LSTM model
    double result = 0.5 * trustMetrics[0] + 0.3 * trustMetrics[1] + 0.2 * trustMetrics[2];
    return result;
}

void SDNController::evaluateTrust() {
    for (auto &node : nodes) {
        double commTrust = computeCommunicationTrust(node);
        double nodeTrust = computeNodeTrust(node);
        double envTrust = computeEnvironmentTrust(node);

        std::vector<double> trustMetrics = { commTrust, nodeTrust, envTrust };
        node.trustScore = runLSTMModel(trustMetrics);  // Using LSTM to compute trust score
    }
}

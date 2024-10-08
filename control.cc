#include "control.h"
#include <ns3/applications-module.h>
#include <ns3/core-module.h>
#include <ns3/csma-module.h>
#include <ns3/internet-module.h>
#include <ns3/network-module.h>
#include <ns3/ofswitch13-module.h>
#include <iostream>
#include <vector>

using namespace ns3;

void SDNController::initializeNetwork(int numNodes)
{
    NodeContainer nodes;
    nodes.Create(numNodes);

    // Configure the CsmaHelper for network communication
    CsmaHelper csmaHelper;
    csmaHelper.SetChannelAttribute("DataRate", DataRateValue(DataRate("100Mbps")));
    csmaHelper.SetChannelAttribute("Delay", TimeValue(MilliSeconds(2)));

    NetDeviceContainer netDevices = csmaHelper.Install(nodes);

    // Set up OpenFlow switch using OFSwitch13InternalHelper
    Ptr<OFSwitch13InternalHelper> ofHelper = CreateObject<OFSwitch13InternalHelper>();
    Ptr<OFSwitch13LearningController> controller = CreateObject<OFSwitch13LearningController>();
    ofHelper->InstallController(nodes.Get(0), controller); // Set controller on node 0

    // Install switches
    ofHelper->InstallSwitch(nodes.Get(1), netDevices);
    ofHelper->CreateOpenFlowChannels();

    // Install the Internet stack
    InternetStackHelper internet;
    internet.Install(nodes);

    // Assign IP addresses
    Ipv4AddressHelper ipv4;
    ipv4.SetBase("10.1.1.0", "255.255.255.0");
    ipv4.Assign(netDevices);

    // Enable tracing (PCAP)
    EnablePcapTracing(netDevices); // Pass netDevices to EnablePcapTracing

    // Add trust-based decision logic here
    calculateNodeTrust(nodes);

    // Call the LSTM-based prediction logic
    lstmTrafficPrediction();
}

// Trust calculation for each node
void SDNController::calculateNodeTrust(NodeContainer& nodes)
{
    for (uint32_t i = 0; i < nodes.GetN(); ++i)
    {
        Ptr<Node> node = nodes.Get(i);
        double trustValue = computeNodeTrust(node);
        std::cout << "Node " << i << " Trust Value: " << trustValue << std::endl;
    }
}

// Trust computation based on network parameters (dummy logic for now)
double SDNController::computeNodeTrust(Ptr<Node> node)
{
    // In real implementation, this would be based on network stats like packet delivery, latency, etc.
    double trustValue = 0.9;  // Example static value
    trustValue = trustValue - 0.1 * (rand() % 5);  // Dummy computation
    return trustValue;
}

// LSTM Traffic Prediction function
void SDNController::lstmTrafficPrediction()
{
    // Placeholder for LSTM model loading and traffic prediction logic
    vector<double> trafficData = loadTrafficData();
    vector<double> prediction = runLstmModel(trafficData);

    // Output the prediction results
    for (size_t i = 0; i < prediction.size(); ++i)
    {
        std::cout << "Predicted Traffic for Node " << i << ": " << prediction[i] << std::endl;
    }
}

// Function to load traffic data for LSTM model (dummy data here)
vector<double> SDNController::loadTrafficData()
{
    vector<double> trafficData = {0.2, 0.3, 0.5, 0.7, 0.6, 0.8}; // Example data
    return trafficData;
}

// Dummy LSTM model to predict traffic (this should load a real LSTM model)
vector<double> SDNController::runLstmModel(const vector<double>& inputData)
{
    vector<double> predictedTraffic(inputData.size());
    for (size_t i = 0; i < inputData.size(); ++i)
    {
        predictedTraffic[i] = inputData[i] + 0.1;  // Dummy prediction logic
    }
    return predictedTraffic;
}

void SDNController::EnablePcapTracing(NetDeviceContainer devices) // Pass devices here
{
    CsmaHelper csmaHelper;
    for (size_t i = 0; i < devices.GetN(); ++i)
    {
        csmaHelper.EnablePcap("/home/capstone/trace/devansh/trace", devices.Get(i), true);
    }
}

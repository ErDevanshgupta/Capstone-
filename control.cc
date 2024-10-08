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

NetDeviceContainer SDNController::initializeNetwork(int numNodes) {
    NodeContainer nodes;
    nodes.Create(numNodes);

    PointToPointHelper p2p;
    p2p.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
    p2p.SetChannelAttribute("Delay", StringValue("2ms"));

    NetDeviceContainer devices = p2p.Install(nodes);

    InternetStackHelper internet;
    internet.Install(nodes);

    Ipv4AddressHelper ipv4;
    ipv4.SetBase("10.1.1.0", "255.255.255.0");
    ipv4.Assign(devices);

    // Return the NetDeviceContainer with the devices
    return devices;
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

void SDNController::EnablePcapTracing(NetDeviceContainer devices) 
{
    CsmaHelper csmaHelper;
    for (size_t i = 0; i < devices.GetN(); ++i)
    {
        csmaHelper.EnablePcap("/home/capstone/trace/devansh/trace", devices.Get(i), true);
    }
}

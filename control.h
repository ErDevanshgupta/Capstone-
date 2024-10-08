#ifndef SDN_CONTROLLER_H
#define SDN_CONTROLLER_H

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include <vector>

using namespace ns3;
using namespace std;

class SDNController {
public:
    NetDeviceContainer initializeNetwork(int numNodes);
    void EnablePcapTracing(ns3::NetDeviceContainer devices);

    // Add these missing function declarations
    void calculateNodeTrust(ns3::NodeContainer& nodes);
    double computeNodeTrust(ns3::Ptr<Node> node);  // Adjust if required
    void lstmTrafficPrediction();
    vector<double> loadTrafficData();  // Adjust the return type and parameters as needed
    vector<double> runLstmModel(const vector<double>& inputData);  // Adjust if required
};

#endif // SDN_CONTROLLER_H

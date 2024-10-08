#ifndef SDNCONTROLLER_H
#define SDNCONTROLLER_H

#include <ns3/core-module.h>
#include <ns3/network-module.h>
#include <ns3/csma-module.h>
#include <ns3/internet-module.h>
#include <ns3/ofswitch13-module.h>

using namespace ns3;
using namespace std;

class SDNController {
public:
    void initializeNetwork(int numNodes);
    void calculateNodeTrust(ns3::NodeContainer& nodes);
    double computeNodeTrust(ns3::Ptr<ns3::Node> node);
    void lstmTrafficPrediction();
    std::vector<double> loadTrafficData();
    std::vector<double> runLstmModel(const std::vector<double>& inputData);

    // Update this declaration to accept NetDeviceContainer
    void EnablePcapTracing(ns3::NetDeviceContainer devices);
};


#endif // SDNCONTROLLER_H

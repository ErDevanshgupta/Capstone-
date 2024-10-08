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
    void EnablePcapTracing();

    // Trust calculation and LSTM prediction functions
    void calculateNodeTrust(NodeContainer& nodes);
    double computeNodeTrust(Ptr<Node> node);

    void lstmTrafficPrediction();
    vector<double> loadTrafficData();
    vector<double> runLstmModel(const vector<double>& inputData);
};

#endif // SDNCONTROLLER_H

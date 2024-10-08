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
    NetDeviceContainer initializeNetwork(int numNodes);  // Change void to NetDeviceContainer
    void EnablePcapTracing(ns3::NetDeviceContainer devices);
};



#endif // SDNCONTROLLER_H

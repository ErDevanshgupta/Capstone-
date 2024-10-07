// main.cc

#include "control.h"
#include <ns3/core-module.h>
#include <ns3/csma-module.h>
#include <ns3/network-module.h>
#include <ns3/internet-module.h>
#include <ns3/ofswitch13-module.h>

using namespace ns3;

int main(int argc, char* argv[])
{
    // Initialize the SDN controller
    SDNController controller;
    int numNodes = 100;  // Example number of nodes

    // Initialize Network
    controller.initializeNetwork(numNodes);

    // Enable tracing (PCAP files)
    bool trace = true;  // Enable PCAP traces
    if (trace)
    {
        // Assuming controller has access to nodes and devices
        controller.EnablePcapTracing();
    }

    // Run the simulation
    Simulator::Run();
    Simulator::Destroy();

    return 0;
}

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/pcap-file-wrapper.h"
#include "control.h"

using namespace ns3;
int main(int argc, char *argv[]) {
    // Set up the network and nodes
    SDNController controller;
    NodeContainer nodes;
    nodes.Create(3); // Example: Create 3 nodes

    // Initialize the network and get the NetDeviceContainer
    NetDeviceContainer netDevices = controller.initializeNetwork(nodes.GetN());

    // Enable PCAP tracing
    controller.EnablePcapTracing(netDevices);

    // Run the simulation
    Simulator::Run();
    Simulator::Destroy();
    return 0;
}


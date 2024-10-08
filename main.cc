#include "control.h"
#include <ns3/core-module.h>
#include <ns3/csma-module.h>
#include <ns3/internet-module.h>
#include <ns3/network-module.h>
#include <ns3/ofswitch13-module.h>
#include <ns3/aqua-sim-helper.h>

using namespace ns3;

int main(int argc, char *argv[]) {
    // Set up the network and nodes
    SDNController controller;
    NodeContainer nodes;
    nodes.Create(3); // Example: Create 3 nodes

    // Initialize the network
    controller.initializeNetwork(nodes.GetN());

    // Obtain the NetDeviceContainer (assuming you have it from controller.initializeNetwork or another source)
    NetDeviceContainer netDevices; // This should be the container of your network devices
    // Typically, netDevices would be filled with the devices during initialization

    // Pass the NetDeviceContainer to EnablePcapTracing
    controller.EnablePcapTracing(netDevices);

    // Run the simulation
    Simulator::Run();
    Simulator::Destroy();
    return 0;
}


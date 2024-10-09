#include "control.h"
#include <ns3/core-module.h>
#include <ns3/csma-module.h>
#include <ns3/internet-module.h>
#include <ns3/network-module.h>
#include <ns3/ofswitch13-module.h>
#include <ns3/aqua-sim-helper.h>
#include <ns3/ipv4-static-routing-helper.h>
#include <ns3/ipv4-routing-helper.h>
#include <ns3/ipv4-static-routing.h>
#include <ns3/ipv4-routing-table-entry.h>
#include <ns3/applications-module.h>
#include <ns3/v4ping-helper.h> // Include the v4ping module for pinging

using namespace ns3;

int main(int argc, char *argv[])
{
    uint16_t simTime = 10; // Simulation time in seconds
    bool verbose = false;
    bool trace = true; // Enable PCAP traces

    // Configure command line parameters
    CommandLine cmd;
    cmd.AddValue("simTime", "Simulation time (seconds)", simTime);
    cmd.AddValue("verbose", "Enable verbose output", verbose);
    cmd.AddValue("trace", "Enable PCAP tracing", trace);
    cmd.Parse(argc, argv);

    // Initialize the SDN controller
    SDNController controller;
    int numNodes = 50; // Number of nodes in the network

    // Initialize network in SDN controller and get the devices
    NetDeviceContainer netDevices = controller.initializeNetwork(numNodes);

    // Setup ICMP ping between nodes
    uint32_t sourceNodeIndex = 0;
    uint32_t destNodeIndex = numNodes - 1;

    Ptr<Node> sourceNode = NodeList::GetNode(sourceNodeIndex); // Correct use of Ptr<Node>
    Ptr<Node> destNode = NodeList::GetNode(destNodeIndex);

    Ipv4Address destAddress = destNode->GetObject<Ipv4>()->GetAddress(1, 0).GetLocal(); // Get the IP of the destination node

    // Install Ping Application on the source node
    V4PingHelper ping(destAddress); // Correct usage of V4PingHelper
    ping.SetAttribute("Verbose", BooleanValue(true));
    ApplicationContainer app = ping.Install(sourceNode);
    app.Start(Seconds(1.0));  // Start pinging at time = 1s
    app.Stop(Seconds(simTime - 1));

    if (verbose) {
        LogComponentEnable("OFSwitch13Device", LOG_LEVEL_ALL);
        LogComponentEnable("OFSwitch13Controller", LOG_LEVEL_ALL);
        LogComponentEnable("SDNController", LOG_LEVEL_ALL);
    }

    // Enable tracing (PCAP format)
    if (trace) {
        controller.EnablePcapTracing();
    }

    // Run the simulation
    Simulator::Stop(Seconds(simTime));
    Simulator::Run();
    Simulator::Destroy();

    return 0;
}

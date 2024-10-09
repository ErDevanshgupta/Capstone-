#include "control.h"
#include <ns3/core-module.h>
#include <ns3/internet-module.h>
#include <ns3/csma-module.h>
#include <ns3/v4ping-helper.h>#include <ns3/net-device-container.h>
#include <ns3/csma-helper.h>
#include <ns3/ptr.h>
#include <ns3/node-container.h>
#include <ns3/ipv4-address-helper.h>
#include <ns3/internet-stack-helper.h>
#include <ns3/v4ping-helper.h>

using namespace ns3;

int main(int argc, char *argv[]) {
    uint16_t numNodes = 50;
    uint16_t simTime = 10; // Simulation time in seconds

    // Command line parameters
    CommandLine cmd;
    cmd.AddValue("simTime", "Simulation time (seconds)", simTime);
    cmd.Parse(argc, argv);

    // Initialize the SDN controller and network
    SDNController controller;
    NetDeviceContainer netDevices = controller.initializeNetwork(numNodes);

    // Pinging from node 0 to node 49
    Ptr<Node> sourceNode = NodeList::GetNode(0);
    Ptr<Node> destNode = NodeList::GetNode(numNodes - 1);

    // Get the IP address of the destination node
    Ipv4Address destAddress = destNode->GetObject<Ipv4>()->GetAddress(1, 0).GetLocal();

    // Install Ping application on the source node
    V4PingHelper ping(destAddress);
    ping.SetAttribute("Verbose", BooleanValue(true));
    ApplicationContainer app = ping.Install(sourceNode);

    app.Start(Seconds(1.0));
    app.Stop(Seconds(simTime - 1));

    // Enable PCAP tracing
    csma.EnablePcapAll("ping-trace");

    // Run the simulation
    Simulator::Stop(Seconds(simTime));
    Simulator::Run();
    Simulator::Destroy();

    return 0;
}

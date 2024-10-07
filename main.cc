// main.cc

#include "control.h"
#include <iostream>
#include <ns3/aqua-sim-ng.h>  // Include Aqua-Sim-NG headers
#include <ns3/core-module.h>
#include <ns3/network-module.h>
#include <ns3/aqua-sim-helper.h>  // Aqua-Sim helper for node creation
#include <ns3/pcap-file.h>

using namespace ns3;

int main(int argc, char *argv[]) {
    SDNController controller;
    int numNodes = 100;

    // Initialize NS-3 core components
    CommandLine cmd;
    cmd.Parse(argc, argv);

    // Create Aqua-Sim nodes
    NodeContainer nodes;
    nodes.Create(numNodes);

    // Initialize Aqua-Sim-NG (configure channel and physical layer for underwater nodes)
    AquaSimHelper aquaSimHelper;
    aquaSimHelper.SetChannel("ns3::AquaSimChannel");
    aquaSimHelper.SetPhy("ns3::AquaSimPhyCmn");
    aquaSimHelper.SetMac("ns3::AquaSimMacCmn");
    aquaSimHelper.SetRouting("ns3::AquaSimRoutingVbf");

    // Assign AquaSim-NG devices to nodes
    AquaSimNetDeviceContainer devices = aquaSimHelper.Install(nodes);

    // Assign positions to nodes using mobility model
    MobilityHelper mobility;
    mobility.SetPositionAllocator("ns3::RandomBoxPositionAllocator");
    mobility.Install(nodes);

    // Use switch 13: assign as a device to handle switching/routing
    Ptr<AquaSimSwitch> switch13 = CreateObject<AquaSimSwitch>();
    Ptr<Node> switchNode = nodes.Get(13);  // Node 13 as the switch
    switch13->Install(switchNode);  // Install switch on Node 13

    // Initialize the SDN controller
    controller.initializeNetwork(numNodes);
    std::cout << "Network initialized with " << numNodes << " nodes." << std::endl;

    // Simulation loop with SDN controller handling packet transmission
    for (int step = 0; step < 100; ++step) {
        std::cout << "Simulation Step: " << step + 1 << std::endl;

        // Trust evaluation
        controller.evaluateTrust();

        // Update routing using switch 13 for packet routing decisions
        controller.updateRouting();

        // Handle packet transmission using Aqua-Sim devices and Switch 13
        Packet pkt;
        pkt.src = 0;  // Example source
        pkt.dest = numNodes - 1;  // Example destination
        controller.handlePacket(pkt);  // Log packets using Aqua-Sim's pcap mechanism

        Simulator::Run();  // Run Aqua-Sim simulation for this step
        Simulator::Stop(Seconds(10));  // Set simulation stop time (adjust as necessary)

        // Trace output to .pcap file
        devices.EnablePcap("aqua-sim-ng-trace", nodes);
    }

    Simulator::Destroy();
    std::cout << "Simulation completed." << std::endl;
    return 0;
}

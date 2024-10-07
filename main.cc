#include "control.h"
#include <ns3/core-module.h>
#include <ns3/csma-module.h>
#include <ns3/internet-module.h>
#include <ns3/network-module.h>
#include <ns3/ofswitch13-module.h>
#include <ns3/aqua-sim-helper.h>

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

    // Initialize network in SDN controller
    controller.initializeNetwork(numNodes);

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

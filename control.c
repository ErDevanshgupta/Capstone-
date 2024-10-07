// control.c

#include "control.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <ns3/packet.h>
#include <ns3/aqua-sim-header.h>
#include <ns3/aqua-sim-routing.h>


// Constructor
SDNController::SDNController() {
    std::srand(std::time(0));
}

// Destructor
SDNController::~SDNController() {}

// Initialize Network with Nodes
void SDNController::initializeNetwork(int numNodes) {
    nodes.reserve(numNodes);
    for (int i = 0; i < numNodes; ++i) {
        Node node;
        node.id = i;
        node.energy = 1000.0; // Initial energy
        node.x = static_cast<double>(std::rand() % 1000);
        node.y = static_cast<double>(std::rand() % 1000);
        node.z = static_cast<double>(std::rand() % 1000);
        node.isMalicious = (std::rand() % 100) < 10;  // 10% chance to be malicious
        node.trustScore = 1.0;  // Initial trust score
        nodes.push_back(node);
    }

    double communicationRange = 100.0;
    for (auto &srcNode : nodes) {
        for (auto &destNode : nodes) {
            if (srcNode.id != destNode.id) {
                double distance = std::sqrt(
                    pow(srcNode.x - destNode.x, 2) +
                    pow(srcNode.y - destNode.y, 2) +
                    pow(srcNode.z - destNode.z, 2)
                );
                if (distance <= communicationRange) {
                    routingTable[srcNode.id].push_back(destNode.id);
                }
            }
        }
    }
}

// Trust Evaluation
void SDNController::evaluateTrust() {
    for (auto &node : nodes) {
        if (!node.isMalicious) {
            double commTrust = computeCommunicationTrust(node);
            double nodeTrust = computeNodeTrust(node);
            double envTrust = computeEnvironmentTrust(node);

            std::vector<double> trustMetrics = { commTrust, nodeTrust, envTrust };
            node.trustScore = runLSTMModel(trustMetrics);
        } else {
            node.trustScore = 0.0;
        }
    }
}

// Handle Packet Transmission
void SDNController::handlePacket(Packet &pkt) {
    if (!routingTable[pkt.src].empty()) {
        pkt.path = routingTable[pkt.src];
        std::cout << "Packet from " << pkt.src << " to " << pkt.dest << " via path: ";
        for (auto nodeId : pkt.path) {
            std::cout << nodeId << " ";
        }
        std::cout << std::endl;

        // Log packet to .pcap file using AquaSim-NG's pcap mechanism
        logToPCAP("aqua_sim_trace.pcap", pkt);
    }
}

// Log Packet to PCAP file
void SDNController::logToPCAP(const std::string &filename, const Packet &pkt) {
    pcap_t *pcap;
    pcap_dumper_t *dumper;
    char errbuf[PCAP_ERRBUF_SIZE];

    // Open the file for writing
    pcap = pcap_open_dead(DLT_RAW, 65535);
    dumper = pcap_dump_open(pcap, filename.c_str());

    if (dumper == nullptr) {
        std::cerr << "Error opening pcap file: " << errbuf << std::endl;
        return;
    }

    struct pcap_pkthdr header;
    header.ts.tv_sec = std::time(nullptr);
    header.ts.tv_usec = 0;
    header.caplen = sizeof(pkt);  // Simulating packet capture length
    header.len = sizeof(pkt);     // Actual length of the packet

    pcap_dump((u_char*)dumper, &header, (const u_char*)&pkt);

    pcap_dump_close(dumper);
    pcap_close(pcap);
}

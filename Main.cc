// main.cc

#include "control.h"
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    SDNController controller;
    int numNodes = 100; // Example number of nodes

    // Initialize Network
    controller.initializeNetwork(numNodes);
    std::cout << "Network initialized with " << numNodes << " nodes." << std::endl;

    // Simulation Loop
    int simulationSteps = 100; // Example number of simulation steps
    for(int step = 0; step < simulationSteps; ++step) {
        std::cout << "Simulation Step: " << step + 1 << std::endl;

        // Evaluate Trust
        controller.evaluateTrust();

        // Update Routing
        controller.updateRouting();

        // Handle Packet Transmission (Example)
        Packet pkt;
        pkt.src = 0; // Example source
        pkt.dest = numNodes - 1; // Example destination
        controller.handlePacket(pkt);

        // Integrate with AquaSim-NG (using its APIs)

        // Pause for a short duration to simulate real-time (optional)
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::cout << "Simulation completed." << std::endl;
    return 0;
}

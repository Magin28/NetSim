#include <iostream>
#include "package.hxx"
#include "types.hxx"
#include "storage_types.hxx"
#include "gtest/gtest.h"
<<<<<<< Updated upstream
=======
#include "simulation.hxx"
#include "raports.hxx"

int main() {
    const std::string project_root = "../";
    const std::string filename = project_root + "dane/dane.txt";
    const std::string output_dir = project_root + "Wyniki";
    const std::string struct_out_file = output_dir + "/struct-report.txt";
    const std::string sim_out_file = output_dir + "/sim-report.txt";
    const TimeOffset simulation_steps = 30;
    std::cout << "--- START: NetSim ---" << std::endl;

    std::ifstream input_file(filename);
    Factory factory;

    factory = load_factory_structure(input_file);
    std::ofstream struct_os(struct_out_file);
    struct_os.is_open();
    generate_structure_report(factory, struct_os);
    struct_os.close();

    std::ofstream sim_os(sim_out_file);
    simulate(factory, simulation_steps, [&sim_os](Factory& f, Time t) {
        generate_simulation_turn_report(f, sim_os, t);
    });
    sim_os.close();
    std::cout << "--- KONIEC: Symulacja zakonczona sukcesem ---" << std::endl;

    return 0;
}
>>>>>>> Stashed changes

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
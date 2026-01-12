#include <iostream>
#include "package.hxx"
#include "types.hxx"
#include "storage_types.hxx"
#include "gtest/gtest.h"
#include "simulation.hxx"
#include "raports.hxx"

int main() {
    const std::string filename = "dane.txt";

    // Nazwy plików wyjściowych
    const std::string struct_out_file = "struct-report.txt";
    const std::string sim_out_file = "sim-report.txt";
    const TimeOffset simulation_steps = 30;

    std::cout << "--- START: NetSim ---" << std::endl;

    // 1. Otwarcie pliku
    std::ifstream input_file(filename);
    if (!input_file.is_open()) {
        std::cerr << "BLAD KRYTYCZNY: Nie mozna otworzyc pliku '" << filename << "'!" << std::endl;
        std::cerr << "Upewnij sie, ze 'simulation.txt' jest w folderze projektu i odswiezyles CMake." << std::endl;
        return 1;
    }

    Factory factory;

    // 2. Wczytywanie struktury (Parsowanie)
    try {
        std::cout << "Wczytywanie konfiguracji z pliku..." << std::endl;
        factory = load_factory_structure(input_file);
        std::cout << "Konfiguracja wczytana poprawnie." << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "BLAD PARSOWANIA: " << e.what() << std::endl;
        std::cerr << "Sprawdz, czy w pliku txt nie ma pustych linii lub dziwnych spacji." << std::endl;
        return 1;
    }
    input_file.close();

    // 3. Raport struktury (przed symulacją)
    std::ofstream struct_os(struct_out_file);
    if (struct_os.is_open()) {
        generate_structure_report(factory, struct_os);
        struct_os.close();
        std::cout << "Zapisano raport struktury: " << struct_out_file << std::endl;
    } else {
        std::cerr << "Nie udalo sie zapisac raportu struktury." << std::endl;
    }

    // 4. Uruchomienie Symulacji
    std::cout << "Rozpoczynam symulacje na " << simulation_steps << " tur..." << std::endl;

    std::ofstream sim_os(sim_out_file);
    if (!sim_os.is_open()) {
        std::cerr << "Nie mozna utworzyc pliku raportu symulacji!" << std::endl;
        return 1;
    }

    // Wywołanie symulacji z lambda-funkcją do raportowania
    simulate(factory, simulation_steps, [&sim_os](Factory& f, Time t) {
        generate_simulation_turn_report(f, sim_os, t);
    });

    sim_os.close();
    std::cout << "--- KONIEC: Symulacja zakonczona sukcesem ---" << std::endl;

    return 0;
}


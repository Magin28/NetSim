//
// Created by User on 9.01.2026.
//

#ifndef RAPORTS_HXX
#define RAPORTS_HXX

#include "factory.hxx"

void generate_structure_report(const Factory& f, std::ostream& os);
void generate_simulation_turn_report(const Factory& f, std::ostream& os, Time t);

#endif //RAPORTS_HXX

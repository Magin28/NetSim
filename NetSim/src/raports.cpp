//
// Created by User on 9.01.2026.
//
#include "raports.hxx"

void generate_structure_report(const Factory& f, std::ostream& os) {
    os << "\n== LOADING RAMPS ==\n\n";

    std::set<ElementID> workers;
    std::set<ElementID> storehouses;

    for (auto it = f.ramp_cbegin(); it != f.ramp_cend(); it++) {
        os << "LOADING RAMP #" << std::to_string(it->get_id()) << "\n  Delivery interval: "
           << std::to_string(it->get_delivery_interval()) << "\n  Receivers:\n";
        for (auto iterator = it->receiver_preferences_.cbegin();
             iterator != it->receiver_preferences_.cend(); iterator++) {
            if (iterator->first->get_receiver_type() == ReceiverType::WORKER) {
                workers.insert(iterator->first->get_id());
            } else if (iterator->first->get_receiver_type() == ReceiverType::STOREHOUSE) {
                storehouses.insert(iterator->first->get_id());
            }
             }
        for (auto i: storehouses) {
            os << "    storehouse #" << std::to_string(i) << "\n";
        }
        for (auto i: workers) {
            os << "    worker #" << std::to_string(i) << "\n";
        }
        os << "\n";
    }
    workers.clear();
    storehouses.clear();

    os << "\n== WORKERS ==\n\n";

    std::string queue_type;
};
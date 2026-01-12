#include "raports.hxx"

#include <iostream>
#include <iomanip>
#include <set>
#include <string>
#include <vector>


void print_header_line(std::ostream& os, std::string title) {
    os << "+---------------------------------------------------------------+\n";
    os << "| " << std::left << std::setw(61) << title << " |\n";
    os << "+---------------------------------------------------------------+\n";
}

void generate_structure_report(const Factory& f, std::ostream& os) {
    os << "\n";
    print_header_line(os, "STRUKTURA FABRYKI (MAPA POLACZEN)");


    os << "\n[ NADAJNIKI (Ramps) ]\n";
    for (auto it = f.ramp_cbegin(); it != f.ramp_cend(); it++) {
        os << "  + Rampa [" << it->get_id() << "]\n";
        os << "    |-- Interval: " << it->get_delivery_interval() << " tury\n";
        os << "    `-- Wysyla do:\n";

        for (const auto& receiver : it->receiver_preferences_) {
            std::string type = (receiver.first->get_receiver_type() == ReceiverType::WORKER) ? "Worker" : "Store";
            os << "        -> " << type << " #" << receiver.first->get_id() << "\n";
        }
        os << "\n";
    }


    os << "[ PRZETWORCY (Workers) ]\n";
    for (auto it = f.worker_cbegin(); it != f.worker_cend(); it++) {
        std::string q_type = (it->get_queue()->get_queue_type() == PackageQueueType::LIFO) ? "LIFO" : "FIFO";

        os << "  + Worker [" << it->get_id() << "]\n";
        os << "    |-- Czas pracy: " << it->get_processing_duration() << "\n";
        os << "    |-- Kolejka:    " << q_type << "\n";
        os << "    `-- Wysyla do:\n";

        for (const auto& receiver : it->receiver_preferences_) {
            std::string type = (receiver.first->get_receiver_type() == ReceiverType::WORKER) ? "Worker" : "Store";
            os << "        -> " << type << " #" << receiver.first->get_id() << "\n";
        }
        os << "\n";
    }


    os << "[ MAGAZYNY (Storehouses) ]\n";
    for (auto it = f.storehouse_cbegin(); it != f.storehouse_cend(); it++) {
        os << "  + Storehouse [" << it->get_id() << "]\n";
    }
    os << "\n";
}

void generate_simulation_turn_report(const Factory& f, std::ostream& os, Time t) {
    std::string turn_title = "STAN SYMULACJI - TURA: " + std::to_string(t);
    print_header_line(os, turn_title);


    std::set<ElementID> worker_ids;
    for (auto it = f.worker_cbegin(); it != f.worker_cend(); it++) worker_ids.insert(it->get_id());

    if (!worker_ids.empty()) os << "| WORKERS STATUS:\n";

    for (auto id : worker_ids) {
        auto it = f.find_worker_by_id(id);

        os << "|   Worker #" << id << "\n";

        // 1. P-Buffer (Co jest teraz obrabiane?)
        os << "|     > P-Buffer (Przetwarzanie): ";
        if (it->get_processing_buffer().has_value()) {
            Time time_processed = t - it->get_package_processing_start_time() + 1;
            os << "[ Paczka #" << it->get_processing_buffer()->get_id() << " ]"
               << " (postep: " << time_processed << "/" << it->get_processing_duration() << ")";
        } else {
            os << "(pusty)";
        }
        os << "\n";


        os << "|     > Kolejka: ";
        if (it->get_queue()->empty()) {
            os << "(pusta)";
        } else {
            for (const auto& pkg : *it->get_queue()) {
                os << "#" << pkg.get_id() << " ";
            }
        }
        os << "\n";

        os << "|     > S-Buffer (Gotowe!):       ";
        if (it->get_sending_buffer().has_value()) {
            os << ">>> [ Paczka #" << it->get_sending_buffer()->get_id() << " ] >>>";
        } else {
            os << "(pusty)";
        }
        os << "\n|\n";
    }


    std::set<ElementID> store_ids;
    for (auto it = f.storehouse_cbegin(); it != f.storehouse_cend(); it++) store_ids.insert(it->get_id());

    if (!store_ids.empty()) os << "| STOREHOUSES STATUS:\n";

    for (auto id : store_ids) {
        auto it = f.find_storehouse_by_id(id);
        os << "|   Storehouse #" << id << " Stock: ";

        if (it->get_queue()->empty()) {
            os << "(pusty)";
        } else {

            os << "{ ";
            bool first = true;
            for (const auto& pkg : *it->get_queue()) {
                if (!first) os << ", ";
                os << "#" << pkg.get_id();
                first = false;
            }
            os << " }";
        }
        os << "\n";
    }

    os << "+---------------------------------------------------------------+\n\n";
}
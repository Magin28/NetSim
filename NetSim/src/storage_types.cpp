#include "storage_types.hxx"

Package PackageQueue::pop() {
    if (package_queue_type_ == PackageQueueType::LIFO) {
        Package p(std::move(package_list_.back()));
        package_list_.pop_back();
        return p;
    } else {
        Package p(std::move(package_list_.front()));
        package_list_.pop_front();
        return p;
    }
}
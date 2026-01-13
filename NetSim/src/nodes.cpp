#include "nodes.hxx"

void ReceiverPreferences::add_receiver(IPackageReceiver* r) {
    auto num_of_receivers = preferences_.size();
    if (num_of_receivers == 0) {
        preferences_[r] = 1.0;
    } else {
        auto denominator = static_cast<double>(num_of_receivers + 1);
        for (auto& pair : preferences_) {
            pair.second = 1.0 / denominator;
        }
        preferences_[r] = 1.0 / denominator;
    }
}

void ReceiverPreferences::remove_receiver(IPackageReceiver* r) {
    auto it = preferences_.find(r);
    if (it == preferences_.end()) return;

    preferences_.erase(it);

    auto num_of_receivers = preferences_.size();
    if (num_of_receivers == 0) return;

    auto new_probability = 1.0 / static_cast<double>(num_of_receivers);
    for (auto& pair : preferences_) {
        pair.second = new_probability;
    }
}

IPackageReceiver* ReceiverPreferences::choose_receiver() {
    auto prob = generate_probability_();
    double distribution = 0.0;
    for (const auto& pair : preferences_) {
        distribution += pair.second;
        if (prob <= distribution) {
            return pair.first;
        }
    }
    return nullptr;
}

void PackageSender::send_package() {
    if (buffer_) {
        IPackageReceiver* receiver = receiver_preferences_.choose_receiver();
        if (receiver) {
            receiver->receive_package(std::move(*buffer_));
            buffer_.reset();
        }
    }
}

void Ramp::deliver_goods(Time t) {
    if ((t - 1) % di_ == 0) {
        push_package(Package());
    }
}

void Worker::do_work(Time t) {
    if (!buffer_ && !q_->empty()) {
        buffer_.emplace(q_->pop());
        t_ = t;
    }

    if (buffer_) {
        if (t - t_ + 1 == pd_) {
            push_package(std::move(*buffer_));
            buffer_.reset();
        }
    }
}

void Worker::receive_package(Package&& p) {
    q_->push(std::move(p));
}

void Storehouse::receive_package(Package&& p) {
    d_->push(std::move(p));
}
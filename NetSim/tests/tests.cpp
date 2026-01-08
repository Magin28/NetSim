#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "package.hxx"
#include "storage_types.hxx"
#include <utility>
#include "nodes.hxx"

// main mocks

#define NODES_MOCKS_HPP_

class MockReceiver : public IPackageReceiver
{
public:
    MOCK_METHOD1(receive_package, void(Package &&));

    MOCK_CONST_METHOD0(begin, IPackageStockpile::const_iterator());

    MOCK_CONST_METHOD0(cbegin, IPackageStockpile::const_iterator());

    MOCK_CONST_METHOD0(end, IPackageStockpile::const_iterator());

    MOCK_CONST_METHOD0(cend, IPackageStockpile::const_iterator());

    MOCK_CONST_METHOD0(get_receiver_type, ReceiverType());

    MOCK_CONST_METHOD0(get_id, ElementID());

    MOCK_CONST_METHOD0(get_delivery_interval, TimeOffset());
};

// TEST połprodukty

TEST(WorkerTest, HasBuffer)
{
    Worker w(1, 2, std::make_unique<PackageQueue>(PackageQueueType::FIFO));
    Time t = 1;

    w.receive_package(Package(1));
    w.do_work(t);
    ++t;
    w.receive_package(Package(2));
    w.do_work(t);
    auto &buffer = w.get_sending_buffer();

    ASSERT_TRUE(buffer.has_value());
    EXPECT_EQ(buffer.value().get_id(), 1);
}

TEST(PackageTest, IsAssignedIdLowest)
{
    Package p1;
    Package p2;

    EXPECT_EQ(p1.get_id(), 1);
    EXPECT_EQ(p2.get_id(), 2);
}

TEST(PackageTest, IsIdReused)
{
    {
        Package p1;
    }
    Package p2;

    EXPECT_EQ(p2.get_id(), 1);
}

TEST(PackageTest, IsMoveConstructorCorrect)
{
    Package p1;
    Package p2(std::move(p1));

    EXPECT_EQ(p2.get_id(), 1);
}

TEST(PackageTest, IsAssignmentOperatorCorrect)
{
    Package p1;
    Package p2 = std::move(p1);

    EXPECT_EQ(p2.get_id(), 1);
}

TEST(PackageQueueTest, IsFifoCorrect)
{
    PackageQueue q(PackageQueueType::FIFO);
    q.push(Package(1));
    q.push(Package(2));

    Package p(std::move(q.pop()));
    EXPECT_EQ(p.get_id(), 1);

    p = q.pop();
    EXPECT_EQ(p.get_id(), 2);
}

TEST(PackageQueueTest, IsLifoCorrect)
{
    PackageQueue q(PackageQueueType::LIFO);
    q.push(Package(1));
    q.push(Package(2));

    Package p(std::move(q.pop()));
    EXPECT_EQ(p.get_id(), 2);

    p = q.pop();
    EXPECT_EQ(p.get_id(), 1);
}

// TEST węzły sieci

TEST(RampTest, IsDeliveryOnTime)
{

    Ramp r(1, 2);
    auto recv = std::make_unique<Storehouse>(1);

    r.receiver_preferences_.add_receiver(recv.get());

    r.deliver_goods(1);
    ASSERT_TRUE(r.get_sending_buffer().has_value());
    r.send_package();

    r.deliver_goods(2);
    ASSERT_FALSE(r.get_sending_buffer().has_value());

    r.deliver_goods(3);
    ASSERT_TRUE(r.get_sending_buffer().has_value());
}

TEST(ReceiverPreferencesTest, AddReceiversRescalesProbability) {
    ReceiverPreferences rp;

    MockReceiver r1;
    rp.add_receiver(&r1);
    ASSERT_NE(rp.get_preferences().find(&r1), rp.get_preferences().end());
    EXPECT_EQ(rp.get_preferences().at(&r1), 1.0);

    MockReceiver r2;
    rp.add_receiver(&r2);
    EXPECT_EQ(rp.get_preferences().at(&r1), 0.5);
    ASSERT_NE(rp.get_preferences().find(&r2), rp.get_preferences().end());
    EXPECT_EQ(rp.get_preferences().at(&r2), 0.5);
}

TEST(ReceiverPreferencesTest, RemoveReceiversRescalesProbability) {
    ReceiverPreferences rp;

    MockReceiver r1, r2;
    rp.add_receiver(&r1);
    rp.add_receiver(&r2);

    rp.remove_receiver(&r2);
    ASSERT_EQ(rp.get_preferences().find(&r2), rp.get_preferences().end());
    EXPECT_EQ(rp.get_preferences().at(&r1), 1.0);
}

void PrintTo(const IPackageStockpile::const_iterator& it, ::std::ostream* os) {
    *os << it->get_id();
}

class PackageSenderFixture : public PackageSender {
    // Nie sposób w teście wykorzystać prywetnej metody `PackageSender::push_package()`,
    // dlatego do celów testowych stworzona została implementacja zawierająca
    // metodę `push_package()` w sekcji publicznej.
public:
    void push_package(Package&& package) { PackageSender::push_package(std::move(package)); }
};

using ::testing::_;

TEST(PackageSenderTest, SendPackage) {
    MockReceiver mock_receiver;
    // Oczekujemy, że metoda `receive_package()` obiektu `mock_receiver` zostanie
    // wywołana dwukrotnie, z dowolnym argumentem (symbol `_`).
    EXPECT_CALL(mock_receiver, receive_package(_)).Times(1);

    PackageSenderFixture sender;
    sender.receiver_preferences_.add_receiver(&mock_receiver);
    // Zwróć uwagę, że poniższa instrukcja korzysta z semantyki referencji do r-wartości.
    sender.push_package(Package());

    sender.send_package();

    EXPECT_FALSE(sender.get_sending_buffer());

    // Upewnij się, że proces wysyłania zachodzi tylko wówczas, gdy w bufor jest pełny.
    sender.send_package();
}
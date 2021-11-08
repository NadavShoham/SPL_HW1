#include "Trainer.h"

Trainer::Trainer(int t_capacity): capacity(t_capacity), open(false) {
    //TODO initialize vectors?
}

int Trainer::getCapacity() const {
    return capacity;
}

void Trainer::addCustomer(Customer *customer) {
    //TODO add to costumerlist
    // add to orderlist
}

void Trainer::removeCustomer(int id) {
    // TODO reverse add
}

Customer* Trainer::getCustomer(int id) {
    return nullptr;
    //TODO decide order of customers then change return statement.
}

std::vector<Customer *>& Trainer::getCustomers() {
    // TODO check if user can change values in customerlist
    return customersList;
}

std::vector<OrderPair>& Trainer::getOrders() {
    // TODO like getcustomers
    return orderList;
}

void
Trainer::order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout> &workout_options) {
    // TODO initialize when writing order class in actions
}

void Trainer::openTrainer() {
    // TODO initialize when writing relevant action class
}

void Trainer::closeTrainer() {
    // TODO same as opentrainer
}

int Trainer::getSalary() {
    //TODO find out where the salary is saved and calculated
    return 0;
}

bool Trainer::isOpen() {
    return open;
}



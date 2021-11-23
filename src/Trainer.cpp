#include "Trainer.h"

Trainer::Trainer(int t_capacity): capacity(t_capacity), open(false), salary(0), id(0){
}

int Trainer::getCapacity() const {
    return capacity;
}

void Trainer::addCustomer(Customer *customer) {
    //TODO add to costumerlist
    // add to orderlist
}

void Trainer::removeCustomer(int c_id) {
    int index = 0;
    for (Customer* customer:customersList) {
        if (customer->getId() == c_id)
            break;
        index++;
    }
    if (index < customersList.size()) {
        customersList.erase(customersList.begin() + index);
        capacity++;
    }


    // copies all valid orders to temp vector.
    vector<OrderPair> temp;
    for (OrderPair & orderPair : orderList) {
        if (orderPair.first != c_id) {
            temp.emplace_back(orderPair.first, orderPair.second);
        }
    }

    // empty salary and orderlist vector
    decreaseSalary(salary);
    orderList.clear();

    // copy valid orders back.
    for (const OrderPair& orderPair: temp) {
        orderList.emplace_back(orderPair.first, orderPair.second);
        increaseSalary(orderPair.second.getPrice());
    }
}

Customer* Trainer::getCustomer(int c_id) {
    for (Customer* customer:customersList) {
        if (customer->getId() == c_id)
            return customer;
    }
    return nullptr;
}

std::vector<Customer *>& Trainer::getCustomers(){
    return customersList;
}

std::vector<OrderPair>& Trainer::getOrders() {
    return orderList;
}

void
Trainer::order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout> &workout_options) {
    for (int i: workout_ids) {
        increaseSalary(workout_options[i].getPrice());
        orderList.emplace_back(customer_id, workout_options[i]);
    }
}

void Trainer::openTrainer() {
    open = true;
}

void Trainer::closeTrainer() {
    if (salary > 0)
        salary = 0;
    open = false;
}

int Trainer::getSalary() const {
    return salary;
}

bool Trainer::isOpen() {
    return open;
}

//ours
void Trainer::setId(int t_id) {
    id = t_id;
}

int Trainer::getId() const {
    return id;
}

void Trainer::increaseSalary(int payCheck) {
    salary += payCheck;
}

void Trainer::decreaseSalary(int amount) {
    salary -= amount;
    if (salary < 0)
        salary = 0;
}

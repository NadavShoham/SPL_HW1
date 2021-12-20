#include "Trainer.h"
using namespace std;

Trainer::Trainer(int t_capacity): capacity(t_capacity), open(false), salary(0), id(0){
}

//copy constructor
Trainer::Trainer(const Trainer& other): capacity(other.capacity), open(other.open), salary(other.salary), id(other.id){
    deepCopy(other);
}

//copy assigment operator
Trainer& Trainer::operator=(const Trainer& other){
    if (this != &other) {
        deepDelete();
        capacity = other.capacity;
        open = other.open;
        salary = other.salary;
        id = other.id;
        deepCopy(other);
    }
    return *this;
}

 //move constructor
 Trainer::Trainer(Trainer&& other) {
     id = other.id;
     customersList = other.customersList;
     capacity = other.capacity;
     open = other.open;
     salary = other.salary;
     for (const OrderPair& orderPair: other.orderList) {
         orderList.emplace_back(orderPair.first, orderPair.second);
     }
     other.customersList.clear();

}



 // move assigment operator
Trainer& Trainer::operator=(Trainer&& other){
     deepDelete();
     capacity = other.capacity;
     open = other.open;
     salary = other.salary;
     id = other.id;
     customersList = other.customersList;
     for (const OrderPair& orderPair: other.orderList){
         orderList.emplace_back(orderPair.first, orderPair.second);
     }
     other.customersList.clear();
     return *this;
 }

 //destructor
 Trainer:: ~Trainer(){
    deepDelete();

 }

void Trainer::deepCopy (const Trainer& other){
    for (Customer *customer: other.customersList){
        customersList.push_back(customer->clone());
    }
    for (const OrderPair& orderPair: other.orderList){
        orderList.emplace_back(orderPair.first, orderPair.second);
    }
}

void Trainer::deepDelete() {
    for (Customer *customer: customersList){
        if (customer) delete customer;
    }
    customersList.clear();
    orderList.clear();
}

 int Trainer::getCapacity() const {
    return capacity;
}

void Trainer::addCustomer(Customer *customer) {
    customersList.push_back(customer);
    capacity--;
}

void Trainer::removeCustomer(int c_id) {
    unsigned int index = 0;
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

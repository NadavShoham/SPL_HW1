#include "Customer.h"

Customer::Customer(std::string c_name, int c_id): name(c_name), id(c_id){}

std::string Customer::getName() const {
    return name;
}

int Customer::getId() const {
    return id;
}

SweatyCustomer::SweatyCustomer(std::string name, int id) : Customer(name, id) {}

std::vector<int> SweatyCustomer::order(const std::vector<Workout> &workout_options) {
    // TODO initialize when we write order class.
    return std::vector<int>();
}

std::string SweatyCustomer::toString() const {
    //TODO decide later what to return
    return "";
}

Customer *SweatyCustomer::clone() const {
    return new SweatyCustomer(*this);
}

CheapCustomer::CheapCustomer(std::string name, int id) : Customer(name, id) {

}

std::string CheapCustomer::toString() const {
    return getName() + ",chp";
}

Customer *CheapCustomer::clone() const {
    return new CheapCustomer(*this);
}

HeavyMuscleCustomer::HeavyMuscleCustomer(std::string name, int id) : Customer(name, id) {

}

std::string HeavyMuscleCustomer::toString() const {
    return getName() + ",mcl";
}

Customer *HeavyMuscleCustomer::clone() const {
    return new HeavyMuscleCustomer(*this);
}

FullBodyCustomer::FullBodyCustomer(std::string name, int id) : Customer(name, id) {

}

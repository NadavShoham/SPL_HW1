#include "Customer.h"
#include "algorithm"
using namespace std;


Customer::Customer(std::string c_name, int c_id): name(c_name), id(c_id), ordered(false){}

Customer::~Customer() = default;

std::string Customer::getName() const {
    return name;
}

int Customer::getId() const {
    return id;
}

bool Customer::getOrdered() const{
    return ordered;
}

void Customer::setOrdered() {
    ordered = !ordered;
}


SweatyCustomer::SweatyCustomer(std::string name, int id) : Customer(name, id) {}

vector<int> SweatyCustomer::order(const vector<Workout> &workout_options) {
    vector<int> orders;

    // if an order already happened no need to add another order pair
    if (getOrdered())
        return orders;

    //searching for cardio workouts and adding them to his workout plan
    for (const Workout & workout_option : workout_options){
        if (workout_option.getType() == CARDIO)
            orders.push_back(workout_option.getId());
    }

    setOrdered();
    return orders;
    
}

string SweatyCustomer::toString() const {
    return getName() + ",swt";
}

Customer *SweatyCustomer::clone() const {
    return new SweatyCustomer(*this);
}

CheapCustomer::CheapCustomer(string name, int id) : Customer(name, id) {

}

vector<int> CheapCustomer::order(const vector<Workout> &workout_options){
    int cheapest = INT_MAX;
    int cheapestWorkout = INT_MAX;
    vector<int> orders;

    // if an order already happened no need to add another order pair
    if (getOrdered())
        return orders;

    // looking for cheapest workout
    for (const auto & workout_option : workout_options){
        int price = workout_option.getPrice();
        if(price <= cheapest) {
            if (price == cheapest && workout_option.getId() > cheapestWorkout){
            } else{
                cheapest = price;
                cheapestWorkout = workout_option.getId();
            }
        }
    }

    // if order was successful
    if (cheapestWorkout != INT_MAX) {
        orders.push_back(cheapestWorkout);
    }
    setOrdered();
    return orders;
    
}

std::string CheapCustomer::toString() const {
    return getName() + ",chp";
}

Customer *CheapCustomer::clone() const {
    return new CheapCustomer(*this);
}

HeavyMuscleCustomer::HeavyMuscleCustomer(string name, int id) : Customer(name, id) {

}

vector<int> HeavyMuscleCustomer::order(const vector<Workout> &workout_options) {
    vector<pair<int, int>> anaerobicWorkouts;
    vector<int> orders;

    // if an order already happened no need to add another order pair
    if (getOrdered())
        return orders;

    // looking for most expensive anaerobics workouts
    for (const auto & workout_option : workout_options){
        if(workout_option.getType() == ANAEROBIC)
            anaerobicWorkouts.emplace_back(workout_option.getPrice(),workout_option.getId());
    }
    sort(anaerobicWorkouts.rbegin(),anaerobicWorkouts.rend());
    orders.reserve(anaerobicWorkouts.size());
    for (auto pair: anaerobicWorkouts){
        orders.push_back(pair.second);
    }
    setOrdered();

    return orders;

}

std::string HeavyMuscleCustomer::toString() const {
    return getName() + ",mcl";
}

Customer *HeavyMuscleCustomer::clone() const {
    return new HeavyMuscleCustomer(*this);
}

FullBodyCustomer::FullBodyCustomer(std::string name, int id) : Customer(name, id) {

}

vector<int> FullBodyCustomer::order(const vector<Workout> &workout_options){
    int cheapestCardioPrice = INT_MAX;
    int cheapestCardio = INT_MAX;
    int expensive = 0;
    int expensiveMixType = INT_MAX;
    int cheapestAnaerobicPrice = INT_MAX;
    int cheapestAnaerobic = INT_MAX;
    vector<int> orders;

    // if an order already happened no need to add another order pair
    if (getOrdered())
        return orders;

    // looking for relevant workouts
    for (const auto & workout_option : workout_options){
        //finding the cheapest cardio workout:
        int price = workout_option.getPrice();
        if (workout_option.getType() == CARDIO && price <= cheapestCardioPrice) {
            if (price == cheapestCardioPrice && workout_option.getId() > cheapestCardio){
            } else{
                cheapestCardioPrice = price;
                cheapestCardio = workout_option.getId();
            }
        }
        //finding the most expensive mix type workout:
        else if (workout_option.getType() == MIXED && price >= expensive){
            if (price == expensive && workout_option.getId() > cheapestCardio){
            } else{
                expensive = price;
                expensiveMixType = workout_option.getId();
            }
        }
        //finding the cheapest anaerobic workout:
        else if (workout_option.getType() == ANAEROBIC && price < cheapestAnaerobicPrice){
            if (price == cheapestAnaerobicPrice && workout_option.getId() > cheapestAnaerobic){
            } else {
                cheapestAnaerobicPrice = price;
                cheapestAnaerobic = workout_option.getId();
            }
        }
    }

    // if order was successful
    if (cheapestCardio != INT_MAX ) {
        orders.push_back(cheapestCardio);
    }
    if (expensiveMixType != INT_MAX) {;
        orders.push_back(expensiveMixType);
    }
    if (cheapestAnaerobic != INT_MAX) {
        orders.push_back(cheapestAnaerobic);
    }
    setOrdered();
    return orders;
}

std::string FullBodyCustomer::toString() const {
    return getName() + ",fbd";
}

Customer *FullBodyCustomer::clone() const {
    return new FullBodyCustomer(*this);
}






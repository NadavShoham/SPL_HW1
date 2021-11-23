#ifndef TRAINER_H_
#define TRAINER_H_

#include <vector>
#include "Customer.h"
#include "Workout.h"

typedef std::pair<int, Workout> OrderPair;

class Trainer{
public:
    Trainer(int t_capacity);
    int getCapacity() const;
    void addCustomer(Customer* customer);
    void removeCustomer(int c_id);
    Customer* getCustomer(int c_id);
    std::vector<Customer*>& getCustomers();
    std::vector<OrderPair>& getOrders();
    void order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout>& workout_options);
    void openTrainer();
    void closeTrainer();
    int getSalary() const;
    bool isOpen();

    //ours
    void setId(int t_id);
    int getId() const;
    void increaseSalary(int payCheck);
    void decreaseSalary(int amount);

private:
    int capacity;
    bool open;
    std::vector<Customer*> customersList;
    std::vector<OrderPair> orderList; //A list of pairs for each order for the trainer - (customer_id, Workout)

    //ours
    int salary;
    int id;
    void deepCopy(const Trainer& other);
    void deepDelete();

};


#endif
#ifndef CUSTOMER_H_
#define CUSTOMER_H_

#include <vector>
#include <string>
#include "Workout.h"

class Customer{
public:
    Customer(std::string c_name, int c_id);
    virtual ~Customer();
    virtual Customer* clone() const = 0;
    virtual std::vector<int> order(const std::vector<Workout> &workout_options) = 0;
    virtual std::string toString() const = 0;
    std::string getName() const;
    int getId() const;
    //ours:
    bool getOrdered() const;
    void setOrdered();
protected:
    //ours:
private:
    const std::string name;
    const int id;
    //ours:
    bool ordered;

};


class SweatyCustomer : public Customer {
public:
	SweatyCustomer(std::string name, int id);
    Customer* clone() const;
    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;
private:
};

class CheapCustomer : public Customer {
public:
	CheapCustomer(std::string name, int id);
    Customer* clone() const;
    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;
private:
};


class HeavyMuscleCustomer : public Customer {
public:
	HeavyMuscleCustomer(std::string name, int id);
    Customer* clone() const;
    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;
private:
};

class FullBodyCustomer : public Customer {
public:
	FullBodyCustomer(std::string name, int id);
    Customer* clone() const;
    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;
private:
};


#endif
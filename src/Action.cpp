#include "Action.h"

#include <utility>
#include "Studio.h"
#include "Trainer.h"
#include "algorithm"

using namespace std;
extern Studio* backup;
// TODO kurin said to add destructor to base action
BaseAction::BaseAction(): errorMsg(""){}

ActionStatus BaseAction::getStatus() const {
    return status;
}

std::string BaseAction::getErrorMsg() const {
    return errorMsg;
}

void BaseAction::complete() {
    status = COMPLETED;
}

void BaseAction::error(std::string errormsg) {

    status = ERROR;
    this->errorMsg = std::move(errormsg);
}

BaseAction::~BaseAction() = default;

OpenTrainer::OpenTrainer(int id, std::vector<Customer *> &customersList):
BaseAction(), trainerId(id), customers(customersList){
    for (Customer* customer:customers) {
        customersToStrings.push_back(customer->toString());
    }
}

void OpenTrainer::act(Studio &studio) {
    Trainer* trainer = studio.getTrainer(trainerId);

    // input check
    if (trainer == nullptr || trainer->isOpen()){
        error("Workout session does not exist or is already open\n");
        cout << getErrorMsg();

    } else {
        // Will insert customers until capacity is full, if max limit reached it will delete the extra customers.
        unsigned int limit = customers.size() < trainer->getCapacity() ? customers.size(): trainer->getCapacity();

        for (int i = 0; i < limit; i++)
            trainer->addCustomer(customers[i]);
        for (unsigned int i = limit; i < customers.size(); ++i)
            delete customers[i];
        customers.clear();
        trainer->openTrainer();
        complete();
    }
    //TODO question about this class and rule of 5 in forum
}

std::string OpenTrainer::toString() const {

    // create argument part
    string argumentString = to_string(trainerId);
    for (const string& customer: customersToStrings) {
        argumentString += " " + customer;
    }

    // create status part
    string statusString;
    if (getStatus() == COMPLETED)
        statusString = " Completed";
    else
        statusString = " Error: " + getErrorMsg();

    // concat together
    return "open " + argumentString + statusString;
}

BaseAction *OpenTrainer::clone() const {
    return new OpenTrainer(*this);
}

Order::Order(int id) :BaseAction(), trainerId(id) {}

void Order::act(Studio &studio) {
    Trainer* trainer = studio.getTrainer(trainerId);

    // input check
    if (trainer == nullptr || !trainer->isOpen()) {
        error("Trainer does not exist or is not open\n");
        cout << getErrorMsg();
    } else {
        // preforms the orders of the customers and updates orderList.
        for (Customer* customer:trainer->getCustomers()) {
            trainer->order(customer->getId(), customer->order(studio.getWorkoutOptions()),studio.getWorkoutOptions());
        }

        // prints the orders.
        for (const OrderPair& pair: trainer->getOrders()) {
            string name = trainer->getCustomer(pair.first)->getName();
            string workout = pair.second.getName();
            cout << name << " Is Doing " << workout << endl;
        }
        complete();
    }
}

std::string Order::toString() const {
    // create argument part
    string argumentString = to_string(trainerId);

    // create status part
    string statusString;
    if (getStatus() == COMPLETED)
        statusString = " Completed";
    else
        statusString = " Error: " + getErrorMsg();

    // concat together
    return "order " + argumentString + statusString;
}

BaseAction *Order::clone() const {
    return new Order(*this);
}

MoveCustomer::MoveCustomer(int src, int dst, int customerId):BaseAction(), srcTrainer(src), dstTrainer(dst), id(customerId) {}
void MoveCustomer::act(Studio &studio) {
    Trainer* sTrainer = studio.getTrainer(srcTrainer);
    Trainer* dTrainer = studio.getTrainer(dstTrainer);

    //TODO what the fuck is this function supposed to take care of.

    // check that both trainers are open, check dTrainer capacity and check if customer is in sTrainer's customer list.
    if (sTrainer == nullptr || dTrainer == nullptr || !sTrainer->isOpen() || !dTrainer->isOpen() ||
    dTrainer->getCapacity() == 0 || sTrainer->getCustomers().empty()) {
        error("Cannot move customer\n");
        cout << getErrorMsg();
    } else{
        // Move customer and order.
        Customer* customer = sTrainer->getCustomer(id);
        dTrainer->addCustomer(customer);
        if (customer->getOrdered()) {
            customer->setOrdered();
            dTrainer->order(customer->getId(), customer->order(studio.getWorkoutOptions()), studio.getWorkoutOptions());
        }

        sTrainer->removeCustomer(id);
    }
    // closes trainer if he has no customers left.
    if (sTrainer->getCustomers().empty()){
        sTrainer->closeTrainer();
    }
    complete();
}

std::string MoveCustomer::toString() const {
    // create argument part
    string argumentString = to_string(srcTrainer) + " " + to_string(dstTrainer) + " " + to_string(id);

    // create status part
    string statusString;
    if (getStatus() == COMPLETED)
        statusString = " Completed";
    else
        statusString = " Error: " + getErrorMsg();

    // concat together
    return "move " + argumentString + statusString;
}

BaseAction *MoveCustomer::clone() const {
    return new MoveCustomer(*this);
}

Close::Close(int id):BaseAction(), trainerId(id) {}

void Close::act(Studio &studio) {
    Trainer* trainer = studio.getTrainer(trainerId);

    // input check
    if(trainer == nullptr || !(trainer->isOpen())){
        error("Trainer does not exist or is not open\n");
        cout << getErrorMsg();
    } else {
        // print salary
        int salary = trainer->getSalary();
        cout<< "Trainer " << trainerId << " closed. Salary " << salary << "NIS\n";

        // removes customers
        while (!trainer->getCustomers().empty()) {
            Customer* customer = trainer->getCustomers()[0];
            trainer->removeCustomer(customer->getId());
            delete customer;
        }
        trainer->closeTrainer();
        complete();
    }
    
}

std::string Close::toString() const {
    // create argument part
    string argumentString = to_string(trainerId);

    // create status part
    string statusString;
    if (getStatus() == COMPLETED)
        statusString = " Completed";
    else
        statusString = " Error: " + getErrorMsg();

    // concat together
    return "close " + argumentString + statusString;
}

BaseAction *Close::clone() const {
    return new Close(*this);
}

CloseAll::CloseAll():BaseAction() {}

void CloseAll::act(Studio &studio) {

    // closes all trainers if they are open.
    for (int i = 0; i < studio.getNumOfTrainers(); ++i) {
        if (studio.getTrainer(i)->isOpen()) {
            Close action(i);
            action.act(studio);
        }
    }
    complete();
}

std::string CloseAll::toString() const {

    // create status part
    string statusString;
    if (getStatus() == COMPLETED)
        statusString = " Completed";
    else
        statusString = " Error: " + getErrorMsg();

    // concat together
    return "closeall " + statusString;
}

BaseAction *CloseAll::clone() const {
    return new CloseAll(*this);
}

PrintWorkoutOptions::PrintWorkoutOptions():BaseAction() {}

void PrintWorkoutOptions::act(Studio &studio) {
    for (const Workout& workout: studio.getWorkoutOptions()) {
        string type;
        switch (workout.getType()) {
            case 0:
                type = "Anaerobic";
                break;
            case 1:
                type = "Mixed";
                break;
            case 2:
                type = "Cardio";
        }
        cout<< workout.getName() << ", " << type << ", " << workout.getPrice() << "\n";
    }
    complete();
}

std::string PrintWorkoutOptions::toString() const {

    // create status part
    string statusString;
    if (getStatus() == COMPLETED)
        statusString = " Completed";
    else
        statusString = " Error: " + getErrorMsg();

    // concat together
    return "workout_options " + statusString;
}

BaseAction *PrintWorkoutOptions::clone() const {
    return new PrintWorkoutOptions(*this);
}

PrintTrainerStatus::PrintTrainerStatus(int id): BaseAction(), trainerId(id) {}

void PrintTrainerStatus::act(Studio &studio) {
    Trainer* trainer = studio.getTrainer(trainerId);
    if (!trainer->isOpen()){
        cout << "Trainer " << trainerId << " status: closed\n";
    } else {
        cout << "Trainer " << trainerId << " status: open\n" <<
             "Customers:\n";
        for (Customer *customer: trainer->getCustomers()) {
            cout << customer->getId() << " " << customer->getName() << "\n";
        }
        cout << "Orders:\n";
        for (const OrderPair& pair: trainer->getOrders()) {
            cout << pair.second.getName() << " " << pair.second.getPrice() << "NIS " << pair.first << "\n";
        }
        cout << "Current Trainer's Salary: " << trainer->getSalary() << "NIS\n";
    }
    complete();
}

std::string PrintTrainerStatus::toString() const {

    // create argument part
    string argumentString = to_string(trainerId);

    // create status part
    string statusString;
    if (getStatus() == COMPLETED)
        statusString = " Completed";
    else
        statusString = " Error: " + getErrorMsg();

    // concat together
    return "status " + argumentString + statusString;
}

BaseAction *PrintTrainerStatus::clone() const {
    return new PrintTrainerStatus(*this);
}

PrintActionsLog::PrintActionsLog():BaseAction() {}

void PrintActionsLog::act(Studio &studio) {
    for (BaseAction* action: studio.getActionsLog()) {
        cout << action->toString() << "\n";
    }
    complete();
}

std::string PrintActionsLog::toString() const {

    // create status part
    string statusString;
    if (getStatus() == COMPLETED)
        statusString = " Completed";
    else
        statusString = " Error: " + getErrorMsg();

    // concat together
    return "log " + statusString;
}

BaseAction *PrintActionsLog::clone() const {
    return new PrintActionsLog(*this);
}

BackupStudio::BackupStudio():BaseAction() {}
void BackupStudio::act(Studio &studio) {
    delete backup;
    backup = new Studio(studio);
    complete();
}

std::string BackupStudio::toString() const {

    // create status part
    string statusString;
    if (getStatus() == COMPLETED)
        statusString = " Completed";
    else
        statusString = " Error: " + getErrorMsg();

    // concat together
    return "backup " + statusString;
}

BaseAction *BackupStudio::clone() const {
    return new BackupStudio(*this);
}

RestoreStudio::RestoreStudio(): BaseAction() {}

void RestoreStudio::act(Studio &studio) {
    if (!backup) {
        error("No backup available\n");
        cout << getErrorMsg();
    } else {
        studio = *backup;
        complete();
    }
}

std::string RestoreStudio::toString() const {
    // create status part
    string statusString;
    if (getStatus() == COMPLETED)
        statusString = " Completed";
    else
        statusString = " Error: " + getErrorMsg();

    // concat together
    return "restore " + statusString;
}

BaseAction *RestoreStudio::clone() const {
    return new RestoreStudio(*this);
}

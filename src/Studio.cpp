#include "Studio.h"
#include "Trainer.h"
#include "fstream"

using namespace std;

Studio::Studio(): open(false), customerLastId(0){
}

Studio::Studio(const string &configFilePath): open(false), customerLastId(0){

    // open and read config file
    ifstream config(configFilePath);

    string numOfTrainers;
    string capacityOfTrainers;
    while (numOfTrainers.empty() || numOfTrainers[0] == '#') {
        getline(config, numOfTrainers);
    }
    while (capacityOfTrainers.empty() || capacityOfTrainers[0] == '#') {
        getline(config, capacityOfTrainers);
    }



    // initializing trainers vector.
    for (int i = 0; i < stoi(numOfTrainers); ++i) {
        auto* trainer = new Trainer(static_cast<int>(capacityOfTrainers[i * 2]) - '0');
        trainer->setId(i);
        trainers.push_back(trainer);
    }

    // initializing workout vector.
    string workoutLine;
    while (workoutLine.empty() || workoutLine[0] == '#') {
        getline(config, workoutLine);
    }

    int i = 0;
    while (!workoutLine.empty()){
        unsigned int firstDelimiter = workoutLine.find_first_of(',');
        string name = workoutLine.substr(0, firstDelimiter);
        workoutLine = workoutLine.substr(firstDelimiter + 1, workoutLine.size());
        unsigned int secondDelimiter = workoutLine.find_first_of(',');
        string sType = workoutLine.substr(0, secondDelimiter);
        WorkoutType type;
        if (sType == " Anaerobic")
            type = ANAEROBIC;
        else if (sType == " Mixed")
            type = MIXED;
        else
            type = CARDIO;
        int price = stoi(workoutLine.substr(secondDelimiter + 1, workoutLine.length()));
        workout_options.emplace_back(i, name, price, type);
        i++;
        getline(config, workoutLine);
    }
    config.close();
}

//copy constructor
Studio::Studio(const Studio &other): open(other.open), customerLastId(other.customerLastId) {
    deepCopy(other);
}

//copy assigment operator
Studio &Studio::operator=(const Studio &other) {
    if (this != &other) {
        deepDelete();
        open = other.open;
        customerLastId = other.customerLastId;
        deepCopy(other);
    }
    return *this;
}

void Studio::deepCopy(const Studio &other) {
    for (Trainer* trainer: other.trainers){
        trainers.push_back(new Trainer(*trainer));
    }
    for (const Workout& workout: other.workout_options){
        workout_options.emplace_back(workout);
    }
    for (BaseAction* baseAction: other.actionsLog){
        actionsLog.push_back(baseAction->clone());
    }
}

void Studio::deepDelete() {
    for (Trainer* trainer: trainers){
        if (trainer) delete trainer;
    }
    trainers.clear();
    workout_options.clear();
    for (BaseAction* baseAction: actionsLog){
        if(baseAction) delete baseAction;
    }
    actionsLog.clear();
}

//move constructor
Studio::Studio(Studio &&other): open(other.open), customerLastId(other.customerLastId), trainers(other.trainers), actionsLog(other.actionsLog) {
    for (const Workout& workout: other.workout_options){
        workout_options.emplace_back(workout);
    }
    other.trainers.clear();
    other.actionsLog.clear();
}

//move assigment operator
Studio &Studio::operator=(Studio &&other) {
    deepDelete();
    open = other.open;
    customerLastId = other.customerLastId;
    trainers = other.trainers;
    actionsLog = other.actionsLog;
    for (const Workout& workout: other.workout_options){
        workout_options.emplace_back(workout);
    }
    other.trainers.clear();
    other.actionsLog.clear();
    return *this;
}

//destructor
Studio::~Studio() {
    deepDelete();
}

void Studio::start() {
    open = true;
    cout<< "Studio is now open!\n";

    // main loop, runs until user stops it.
    while (open){
        string request;
        getline(cin, request);
        BaseAction* action;
        // CloseAll action.
        if (request == "closeall") {
            action = new CloseAll();
            open = false;

            // PrintWorkoutOptions action
        } else if (request == "workout_options") {
            action = new PrintWorkoutOptions();

            // PrintActionsLog action
        } else if (request == "log") {
            action = new PrintActionsLog();

            // BackupStudio action
        } else if (request == "backup") {
            action = new BackupStudio();

            // RestoreStudio action
        } else if (request == "restore") {
            action = new RestoreStudio();
        }

        // parsing request data for further actions.
        unsigned int delimiter;
        string data;
        int id;
        delimiter = request.find_first_of(' ');
        string sAction = request.substr(0, delimiter);

        if (sAction != request){
            data = request.substr(delimiter + 1, request.length());
            delimiter = data.find_first_of(' ');
            id = stoi(data.substr(0, delimiter));
            data = data.substr(delimiter + 1, data.length());
        }

        // OpenTrainer action
        if (sAction == "open") {
            vector<Customer *> customerList;
            string customerData;
            delimiter = 0;

            // customerList initialization
            while (delimiter != string::npos) {
                //parsing data
                delimiter = data.find_first_of(' ');
                customerData = data.substr(0, delimiter);
                data = data.substr(delimiter + 1, data.length());
                unsigned int customerDelimiter = customerData.find_first_of(',');
                string name = customerData.substr(0, customerDelimiter);
                string strategy = customerData.substr(customerDelimiter+1, customerDelimiter + 4);

                // initializing customer
                Customer* customer;
                if (strategy == "swt"){
                    customer = new SweatyCustomer(name, chooseCustomerId());
                } else if (strategy == "chp") {
                    customer = new CheapCustomer(name, chooseCustomerId());
                } else if (strategy == "mcl") {
                    customer = new HeavyMuscleCustomer(name, chooseCustomerId());
                } else if (strategy == "fbd") {
                    customer = new FullBodyCustomer(name, chooseCustomerId());
                }

                // adding to list.
                customerList.push_back(customer);
            }

            // create action.
            action = new OpenTrainer(id, customerList);

        // Order action
        } else if (sAction == "order") {
            action = new Order(id);

        // MoveCustomer action
        } else if (sAction == "move") {
            delimiter = data.find_first_of(' ');
            int scdId = stoi(data.substr(0, delimiter));
            int c_id = stoi(data.substr(delimiter + 1));
            action = new MoveCustomer(id, scdId, c_id);

        // Close action
        } else if (sAction == "close") {
            action = new Close(id);

        // PrintTrainerStatus action
        } else if (sAction == "status") {
            action = new PrintTrainerStatus(id);
        }
        action->act(*this);
        actionsLog.push_back(action);
    }
}

int Studio::getNumOfTrainers() const {
    return trainers.size();
}

Trainer *Studio::getTrainer(int tid) {
    if (tid <= trainers.size())
        return trainers[tid];
    else
        return nullptr;
}

const std::vector<BaseAction *> &Studio::getActionsLog() const {
    return actionsLog;
}

std::vector<Workout> &Studio::getWorkoutOptions() {
    return workout_options;
}

//ours

int Studio::chooseCustomerId() {
    return customerLastId++;
}
/*
void Studio::clearId(unsigned int amount) {
    customerLastId -= amount;
}
*/





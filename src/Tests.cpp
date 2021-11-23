#include "Tests.h"
#include "Studio.h"
#include "Customer.h"
#include "Workout.h"
#include "Action.h"

using namespace std;

void currTest() {
    runningTest();
}
void runningTest() {
    string configPath = R"(C:\Users\shoha\CLionProjects\SPL_HW1\exampleInput2.txt)";
    Studio studio(configPath);
    studio.start();
}

// All tests:
void allTests() {
    testStudio();
    testAction();
    testTrainer();
    testCustomer();
}


// full .cpp tests:
void testStudio() {
    testStudioConstructor();
}

void testAction() {

}

void testTrainer() {

}

void testCustomer() {
    testGetCustomers();
    testSweatyCustomerOrder();

}


// studio tests:
void testStudioConstructor(){
    string configPath = R"(C:\Users\shoha\CLionProjects\SPL_HW1\ExmapleInput.txt)";
    Studio studio(configPath);
    cout<< "StudioConstructor --->SUCCESS!";
}

// action tests:



// trainer tests:



// customer tests:
void testGetCustomers() {
    Trainer t(2);
    vector<Customer*> i = t.getCustomers();
}

void testSweatyCustomerOrder(){
    Workout yoga = Workout(1, "Yoga", 90, ANAEROBIC);
    Workout pilates = Workout(2, "Pilates", 110, ANAEROBIC);
    Workout spinning = Workout(3, "Spinning", 120, MIXED);
    Workout zumba = Workout(4, "Zumba", 100, CARDIO);
    Workout rope = Workout(5, "Rope jumps", 70, CARDIO);
    Workout crossFit = Workout(6, "CrossFit", 140, MIXED);
    vector<Workout> workOutOptions;
    workOutOptions.push_back(yoga);
    workOutOptions.push_back(pilates);
    workOutOptions.push_back(spinning);
    workOutOptions.push_back(zumba);
    workOutOptions.push_back(rope);
    workOutOptions.push_back(crossFit);
    SweatyCustomer yosi = SweatyCustomer("Yosi", 1);
    vector<int> yosiOrder = yosi.order(workOutOptions);



}



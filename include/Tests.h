#ifndef SPL_HW1_TEST_H
#define SPL_HW1_TEST_H
#include "Studio.h"
#include "Customer.h"
#include "Workout.h"
#include "Trainer.h"
#include "Action.h"

void currTest();

void runningTest();

// All tests:
void allTests();

// full .cpp tests:
void testStudio();
void testAction();
void testTrainer();
void testCustomer();

// Studio tests:
void testStudioConstructor();

// Action tests:

// Trainer tests:

// Customer tests:
void testSweatyCustomerOrder();
void testGetCustomers();

#endif //SPL_HW1_TEST_H
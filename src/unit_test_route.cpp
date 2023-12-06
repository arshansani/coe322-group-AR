// Name: Arsh Dauwa
// uteid: ae26933
// TACC Username: ArshDauwa

// Name: Arshan Saniei-Sani
// uteid: AS97297
// TACC Username: as97297

#include "route.cpp"  // Include the file that contains the Address and AddressList classes
#include <cassert>
#include <iostream>

const char* red = "\033[31m";
const char* green = "\033[32m";
const char* white = "\033[0m";
const char* blue = "\033[34m";

// Function to check if floating point values are approximately equal
bool AreAlmostEqual(double a, double b, double epsilon = 0.001) {
    return std::abs(a - b) < epsilon;
}

void TestAddressDistance() {
    Address a1(0, 0, false);
    Address a2(3, 4, false);
    double expected_distance = 7; // Manhattan distance

    if (AreAlmostEqual(a1.Distance(a2), expected_distance)) {
        std::cout << "TestAddressDistance: " << green << "Passed\n" << white;
    } 
    else {
        std::cout << "TestAddressDistance: " << red << "Failed\n" << white;
    }
}

void TestAddressHasSameLocation() {
    Address a1(0, 0, false);
    Address a2(0, 0, false);
    Address a3(3, 4, false);

    if (a1.HasSameLocation(a2) && !a1.HasSameLocation(a3)) {
        std::cout << "TestAddressHasSameLocation: " << green << "Passed\n" << white;
    } 
    else {
        std::cout << "TestAddressHasSameLocation: " << red << "Failed\n" << white;
    }
}

void TestAddressListAddAddress() {
    AddressList list;
    list.AddAddress(Address(0, 0, false));
    list.AddAddress(Address(0, 0, false));
    list.AddAddress(Address(0, 0, false));
    list.AddAddress(Address(5, 2, false));
    int prime_status = false; // Earliest Delivery Date
    
    if (list.GetAddresses().at(0).GetPrimeStatus() == prime_status && list.GetAddresses().size() == 2) {
        std::cout << "TestAddressListAddAddress: " << green << "Passed\n" << white;
    } 
    else {
        std::cout << "TestAddressListAddAddress: " << red << "Failed\n" << white;
    }
}

void TestAddressListTotalDistance() {
    AddressList list;
    list.AddAddress(Address(0, 0, false));
    list.AddAddress(Address(3, 4, false));
    list.AddAddress(Address(6, 8, false));
    double expected_distance = 14.0; // Total Manhattan distance

    if (AreAlmostEqual(list.TotalDistance(), expected_distance)) {
        std::cout << "TestAddressListTotalDistance: " << green << "Passed\n" << white;
    } 
    else {
        std::cout << "TestAddressListTotalDistance: " << red << "Failed\n" << white;
    }
}

void TestRouteInitialization() {
  Route route;
  const Address& depot = route.GetDepot();
  
  // Check if the first and last addresses are the depot
  if (route.GetAddresses().front() == depot && route.GetAddresses().back() == depot) {
    std::cout << "TestRouteInitialization: " << green << "Passed\n" << white;
  } 
  else {
    std::cout << "TestRouteInitialization: " << red << "Failed\n" << white;
  }
}

void TestRouteTotalDistance() {
    AddressList list;
    list.AddAddress(Address(3, 4, 10));
    list.AddAddress(Address(6, 8, 15));
    
    Route route(list.GetAddresses());
    double expected_distance = 28.0; // Total Manhattan distance

    if (AreAlmostEqual(route.TotalDistance(), expected_distance)) {
        std::cout << "TestRouteTotalDistance: " << green << "Passed\n" << white;
    } 
    else {
        std::cout << "TestRouteTotalDistance: " << red << "Failed\n" << white;
    }
}

void TestRouteGreedyRoute() {
    AddressList list;
    list.AddAddress(Address(9, 1, 5));
    list.AddAddress(Address(6, 6, 15));
    list.AddAddress(Address(1, 3, 10));
    list.AddAddress(Address(5, 4, 10));
    
    Route route(list.GetAddresses());
    double initial_distance = route.TotalDistance();
    double expected_initial_distance = 40; // Total Manhattan distance

    route.GreedyRoute();
    double greedy_distance = route.TotalDistance();
    double expected_greedy_distance = 30;

    if (AreAlmostEqual(initial_distance, expected_initial_distance) && AreAlmostEqual(greedy_distance, expected_greedy_distance)) {
        std::cout << "TestRouteGreedyRoute: " << green << "Passed\n" << white;
    } 
    else {
        std::cout << "TestRouteGreedyRoute: " << red << "Failed\n" << white;
    }
}

void TestRouteOptimizeRoute() {
    AddressList list;
    list.AddAddress(Address(9, 1, 5));
    list.AddAddress(Address(6, 6, 15));
    list.AddAddress(Address(1, 3, 10));
    list.AddAddress(Address(9, 4, 10));
    
    Route route(list.GetAddresses());
    double initial_distance = route.TotalDistance();
    double expected_initial_distance = 48; // Total Manhattan distance

    route.OptimizeRoute();
    double greedy_distance = route.TotalDistance();
    double expected_greedy_distance = 30;

    if (AreAlmostEqual(initial_distance, expected_initial_distance) && AreAlmostEqual(greedy_distance, expected_greedy_distance)) {
        std::cout << "TestRouteOptimizeRoute: " << green << "Passed\n" << white;
    } 
    else {
        std::cout << "TestRouteOptimizeRoute: " << red << "Failed\n" << white;
    }
}

// Execute test cases
int main(){
    std::cout << blue << "Testing: Address\n" << white;
    TestAddressDistance();
    TestAddressHasSameLocation();

    std::cout << blue << "\nTesting: AddressList\n" << white;
    TestAddressListAddAddress();
    TestAddressListTotalDistance();

    std::cout << blue << "\nTesting: Route\n" << white;
    TestRouteInitialization();
    TestRouteTotalDistance();
    TestRouteGreedyRoute();
    TestRouteOptimizeRoute();
    return 0;
}

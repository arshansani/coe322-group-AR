#include "route.cpp"  // Include the file that contains the Address and AddressList classes
#include <cassert>
#include <iostream>

const char* red = "\033[31m";
const char* green = "\033[32m";
const char* white = "\033[0m";
const char* blue = "\033[34m";

// Function to check if two double values are approximately equal
bool AreAlmostEqual(double a, double b, double epsilon = 0.001) {
    return std::abs(a - b) < epsilon;
}

// Test function for Address Distance method
void TestAddressDistance() {
    Address a1(0, 0, 5);
    Address a2(3, 4, 10);
    double expected_distance = 7; // Manhattan distance

    if (AreAlmostEqual(a1.Distance(a2), expected_distance)) {
    std::cout << "TestAddressDistance: " << green << "Passed\n" << white;
    } 
    else {
    std::cout << "TestAddressDistance: " << red << "Failed\n" << white;
    }
}

// Test function for AddAddress & TotalDistance methods
void TestAddressListAddAddress() {
    AddressList list;
    list.AddAddress(Address(0, 0, 5));
    list.AddAddress(Address(0, 0, 8));
    list.AddAddress(Address(0, 0, 3));
    int expected_delivery_date = 3; // Earliest Delivery Date
    
    if (list.GetAddresses().at(0).GetLastDeliveryDate() == expected_delivery_date) {
        std::cout << "TestAddressListAddAddress: " << green << "Passed\n" << white;
    } 
    else {
        std::cout << "TestAddressListAddAddress: " << red << "Failed\n" << white;
    }
}

// Test function for TotalDistance method
void TestAddressListTotalDistance() {
    AddressList list;
    list.AddAddress(Address(0, 0, 5));
    list.AddAddress(Address(3, 4, 10));
    list.AddAddress(Address(6, 8, 15));
    double expected_distance = 14.0; // Total Manhattan distance

    if (AreAlmostEqual(list.TotalDistance(), expected_distance)) {
        std::cout << "TestAddressListTotalDistance: " << green << "Passed\n" << white;
    } 
    else {
        std::cout << "TestAddressListTotalDistance: " << red << "Failed\n" << white;
    }
}

// Test function for IndexClosestTo method
void TestAddressListIndexClosestTo() {
    AddressList list;
    list.AddAddress(Address(0, 0, 5));
    list.AddAddress(Address(3, 4, 10));
    list.AddAddress(Address(6, 8, 15));
    Address query(2, 3, 7);
    int expected_index = 1; // Index of the address (3, 4)

    if (list.IndexClosestTo(query) == expected_index) {
        std::cout << "TestAddressListIndexClosestTo: " << green << "Passed\n" << white;
    } 
    else {
        std::cout << "TestAddressListIndexClosestTo: " << red << "Failed\n" << white;
    }
}

// Test function for Route initialization with depot
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

// Test function for TotalDistance method
void TestRouteTotalDistance() {
    AddressList list;
    list.AddAddress(Address(0, 0, 5));
    list.AddAddress(Address(3, 4, 10));
    list.AddAddress(Address(6, 8, 15));
    
    Route route(list.GetAddresses());
    double expected_distance = 28.0; // Total Manhattan distance

    if (AreAlmostEqual(route.TotalDistance(), expected_distance)) {
        std::cout << "TestAddressListTotalDistance: " << green << "Passed\n" << white;
    } 
    else {
        std::cout << "TestAddressListTotalDistance: " << red << "Failed\n" << white;
    }
}

// Main function to execute test cases
int main(){
    std::cout << blue << "Testing: Address\n" << white;
    TestAddressDistance();

    std::cout << blue << "\nTesting: AddressList\n" << white;
    TestAddressListAddAddress();
    TestAddressListTotalDistance();
    TestAddressListIndexClosestTo();

    std::cout << blue << "\nTesting: Route\n" << white;
    TestRouteInitialization();
    TestRouteTotalDistance();
    return 0;
}
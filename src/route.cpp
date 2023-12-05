#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <limits>

// Class representing an individual address
class Address {
private:
    double x_, y_;
    int last_delivery_date_;
public:
    Address(double x, double y, int last_delivery_date) 
        : x_(x), y_(y), last_delivery_date_(last_delivery_date) {}

    // Calculate the distance to another address
    double Distance(const Address& other) const {
        //return std::sqrt(std::pow(x_ - other.x_, 2) + std::pow(y_ - other.y_, 2));    // Straight distance calculation
        return std::abs(x_ - other.x_) + std::abs(y_ - other.y_);   // Manhattan distance calculation
    }

    // Check if two addresses are at the same location
    bool HasSameLocation(const Address& other) const {
        return x_ == other.x_ && y_ == other.y_;
    }

    // Accessors
    double GetX() const { 
        return x_; 
    }
    double GetY() const { 
        return y_;
    }
    int GetLastDeliveryDate() const {
        return last_delivery_date_;
    }

    // Overload equality operator for Address
    bool operator==(const Address& other) const {
        return x_ == other.x_ && y_ == other.y_ && last_delivery_date_ == other.last_delivery_date_;
    }
};

// Class representing a list of addresses
class AddressList {
protected:
    std::vector<Address> addresses_;

    // Find the closest unvisited address to the given address
    int IndexClosestTo(const Address& address, const std::vector<bool>& visited) const {
        int closest_index = -1;
        double min_distance = std::numeric_limits<double>::max();
        for (size_t i = 0; i < addresses_.size(); ++i) {
            if(!visited[i]){
                double distance = address.Distance(addresses_[i]);
                if (distance < min_distance) {
                    min_distance = distance;
                    closest_index = static_cast<int>(i);
                }
            }
        }
        return closest_index;
    }
public:
    AddressList() {}

    // Constructor that accepts a vector of Address objects
    AddressList(const std::vector<Address>& initial_addresses) 
        : addresses_(initial_addresses) {}

    // Add an address to the list
    void AddAddress(const Address& address) {
        for (Address& existing_address : addresses_) {
            if (existing_address.HasSameLocation(address)) {
                // If the address location already exists, update the delivery date if the new date is earlier
                if (address.GetLastDeliveryDate() < existing_address.GetLastDeliveryDate()) {
                    existing_address = address; // Update existing address with the new address details
                }
                return; // Address already exists based on location, so return
            }
        }
        addresses_.push_back(address);
    }

    // Calculate the total distance of the route
    double TotalDistance() const {
        double total_distance = 0.0;
        for (size_t i = 0; i < addresses_.size() - 1; ++i) {
            total_distance += addresses_[i].Distance(addresses_[i + 1]);
        }
        return total_distance;
    }

    // Accessors
    std::vector<Address> GetAddresses() const {
        return addresses_;
    }
};

// Class representing the actual route including the depots
class Route : public AddressList {
private:
    void AddDepot(){
        Address depot = GetDepot();
        addresses_.insert(addresses_.begin(), depot);
        addresses_.push_back(depot);
    }
    // Calculates the total distance for an input vector
    double CalculateSetTotalDistance(const std::vector<Address>& route) {
        double totalDistance = 0.0;
        for (size_t i = 0; i < route.size() - 1; ++i) {
            totalDistance += route[i].Distance(route[i + 1]);
        }
        return totalDistance;
    }
    // opt2 heuristic
    bool ReverseSegmentIfImprovesRoute(size_t start, size_t end) {
        std::vector<Address> newRoute = addresses_;
        std::reverse(newRoute.begin() + start, newRoute.begin() + end + 1);

        if (CalculateSetTotalDistance(newRoute) < CalculateSetTotalDistance(addresses_)) {
            addresses_ = newRoute;
            return true;
        }
        return false;
    }
public:
    // Constructors to create Route with initial_addresses
    // Don't really understand why its this way, changing it gave me errors
    Route() : AddressList() {
        AddDepot();
    }
    Route(const std::vector<Address>& initial_addresses) : AddressList(initial_addresses) {
        AddDepot();
    }
    
    // Location of the depot, always the origin
    Address GetDepot(){
        Address depot(0, 0, 0);
        return depot;
    }

    // Method to optimize the route using the greedy search algorithm
    void GreedyRoute() {
        Address depot = GetDepot();
        addresses_.pop_back();

        std::vector<Address> optimized_route;
        std::vector<bool> visited(addresses_.size(), false);
        
        // Mark depot as visited & as the starting point
        visited[0] = true; 
        Address we_are_here = depot;
        optimized_route.push_back(depot);

        // Iterate over the list, finding the nearest unvisited address each time
        for (size_t i = 1; i < addresses_.size(); ++i) {
            int closest_index = IndexClosestTo(we_are_here, visited);
            if (closest_index != -1 && !visited[closest_index]) {
                we_are_here = addresses_[closest_index];
                optimized_route.push_back(we_are_here);
                visited[closest_index] = true;
            }
        }

        // Return to the depot after all addresses
        optimized_route.push_back(GetDepot());
        addresses_ = optimized_route;
    }

    // Method to optimize the route using opt2
    void OptimizeRoute() {
        bool improved = true;
        while (improved) {
            improved = false;
            for (size_t i = 1; i < addresses_.size() - 2; ++i) {
                for (size_t j = i + 1; j < addresses_.size() - 1; ++j) {
                    if (ReverseSegmentIfImprovesRoute(i, j)) {
                        improved = true;
                    }
                }
            }
        }
    }
};

/*
int main() {
    // Example addresses
    Address address1(2, 0, 5);
    Address address2(1, 0, 3);
    Address address3(3, 0, 3);
    Address address4(15, 6, 11);

    // Create a route with initial addresses
    Route route({address1, address2, address3, address4});

    // Print the initial route
    std::cout << "Initial Route: ";
    for (const Address& address : route.GetAddresses()) {
        std::cout << "(" << address.GetX() << ", " << address.GetY() << ") ";
    }
    std::cout << "\nInitial Total Distance: " << route.TotalDistance() << "\n";

    // Apply the Kernighan-Lin optimization
    route.OptimizeRoute();

    // Print the optimized route
    std::cout << "\nOptimized Route: ";
    for (const Address& address : route.GetAddresses()) {
        std::cout << "(" << address.GetX() << ", " << address.GetY() << ") ";
    }
    std::cout << "\nOptimized Total Distance: " << route.TotalDistance() << "\n";

    return 0;
}
*/
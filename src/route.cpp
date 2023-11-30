#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <limits>

// Class representing an individual address
class Address {
private:
    double x_, y_; // Coordinates
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

    // Define equality operator for Address
    bool operator==(const Address& other) const {
        return x_ == other.x_ && y_ == other.y_ && last_delivery_date_ == other.last_delivery_date_;
    }
};

// Class representing a list of addresses
class AddressList {
protected:
    std::vector<Address> addresses_;
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

    // Find the address closest to the given address
    int IndexClosestTo(const Address& address) const {
        int closest_index = -1;
        double min_distance = std::numeric_limits<double>::max();
        for (size_t i = 0; i < addresses_.size(); ++i) {
            double distance = address.Distance(addresses_[i]);
            if (distance < min_distance) {
                min_distance = distance;
                closest_index = static_cast<int>(i);
            }
        }
        return closest_index;
    }

    // Accessors for addresses
    std::vector<Address> GetAddresses() const {
        return addresses_;
    }
};

// Class representing the overall route
class Route : public AddressList {
private:
    void AddDepot(){
        Address depot(0, 0, 0);
        addresses_.insert(addresses_.begin(), depot);
        addresses_.push_back(depot);
    }
public:
    Route() : AddressList() {
        AddDepot();
    }

    // Constructor to accept a vector of initial addresses
    Route(const std::vector<Address>& initial_addresses) : AddressList(initial_addresses) {
        AddDepot();
    }

    // Method to optimize the route
    void OptimizeRoute() {
        Address depot = GetDepot();
        addresses_.pop_back();

        std::vector<Address> optimized_route;
        std::vector<bool> visited(addresses_.size(), false);  // Keep track of visited addresses
        visited[0] = true;  // Mark depot as visited

        // Start from the depot
        Address current_address = depot;
        optimized_route.push_back(current_address); 
        
        // Iterate over the list, finding the nearest unvisited address each time
        for (size_t i = 1; i < addresses_.size(); ++i) { 
            int closest_index = -1;
            double min_distance = std::numeric_limits<double>::max();

            for (size_t j = 1; j < addresses_.size(); ++j) {
                if (!visited[j]) {
                    double distance = current_address.Distance(addresses_[j]);
                    if (distance < min_distance) {
                        min_distance = distance;
                        closest_index = j;
                    }
                }
            }

            if (closest_index != -1) {
                current_address = addresses_[closest_index];
                optimized_route.push_back(current_address);
                visited[closest_index] = true; // Mark as visited
            }
        }
        
        // Return to the depot after all addresses
        optimized_route.push_back(GetDepot()); 
        addresses_ = optimized_route;
    }

    // Accessors
    const Address& GetDepot() const {
        return addresses_.front(); // Assuming the depot is always the first address
    }
};

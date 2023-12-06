// Name: Arsh Dauwa
// uteid: ae26933
// TACC Username: ArshDauwa

// Name: Arshan Saniei-Sani
// uteid: AS97297
// TACC Username: as97297

#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <limits>

// Class representing an individual address
class Address {
private:
    double x_, y_;
    bool prime_;
public:
    Address() : x_(0.0), y_(0.0), prime_(0) {}

    Address(double x, double y, int prime) 
        : x_(x), y_(y), prime_(prime) {}

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
    bool GetPrimeStatus() const {
        return prime_;
    }

    // Overload equality operator for Address
    bool operator==(const Address& other) const {
        return x_ == other.x_ && y_ == other.y_ && prime_ == other.prime_;
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
                if (address.GetPrimeStatus() < existing_address.GetPrimeStatus()) {
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

    bool AnyAddressIsPrime(size_t start, size_t end) const {
        for (size_t i = start; i <= end; ++i) {
            if (addresses_[i].GetPrimeStatus()) {
                return true;
            }
        }
        return false;
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

    static std::vector<Address> ExtractAndReverseSegment(const std::vector<Address>& route, size_t start, size_t end, bool reverse) {
        std::vector<Address> segment(route.begin() + start, route.begin() + end + 1);
        if (reverse) {
            std::reverse(segment.begin(), segment.end());
        }
        return segment;
    }

    static void SwapSegments(std::vector<Address>& route1, size_t start1, size_t end1, 
                             std::vector<Address>& route2, size_t start2, size_t end2) {
        // Validate indices to be within bounds
        if (start1 > end1 || start2 > end2 || end1 >= route1.size() || end2 >= route2.size()) {
            std::cout << "Index out of bounds. Exiting SwapSegments.\n";
            return;
        }

        // Calculate the number of elements to swap
        size_t numElements1 = end1 - start1 + 1;
        size_t numElements2 = end2 - start2 + 1;

        // Swapping segments
        for (size_t i = 0; i < std::min(numElements1, numElements2); ++i) {
            std::swap(route1[start1 + i], route2[start2 + i]);
        }

        // Handle cases where segment sizes are different
        if (numElements1 != numElements2) {
            // Example strategy: rotate elements to keep the route continuity
            if (numElements1 > numElements2) {
                std::rotate(route2.begin() + start2, route2.begin() + start2 + numElements1, route2.end());
            } else {
                std::rotate(route1.begin() + start1, route1.begin() + start1 + numElements2, route1.end());
            }
        }
    }

    bool SwapAndReverseSegments(Route& otherRoute, size_t i1, size_t j1, size_t i2, size_t j2) {
        bool improvementFound = false;
        double originalTotalDistance = CalculateSetTotalDistance(this->addresses_) 
                                     + CalculateSetTotalDistance(otherRoute.addresses_);

        // Create copies of the original routes for manipulation
        std::vector<Address> newRoute1 = this->addresses_;
        std::vector<Address> newRoute2 = otherRoute.addresses_;
        
        for (int reverse1 = 0; reverse1 <= 1; ++reverse1) {
            for (int reverse2 = 0; reverse2 <= 1; ++reverse2) {
                
                std::vector<Address> segment1 = ExtractAndReverseSegment(newRoute1, i1, j1, reverse1);
                std::vector<Address> segment2 = ExtractAndReverseSegment(newRoute2, i2, j2, reverse2);
                
                SwapSegments(newRoute1, i1, j1, newRoute2, i2, j2);

                double newTotalDistance = CalculateSetTotalDistance(newRoute1) 
                                        + CalculateSetTotalDistance(newRoute2);

                if (newTotalDistance < originalTotalDistance) {
                    this->SetRoute(newRoute1);
                    otherRoute.SetRoute(newRoute2);
                    originalTotalDistance = newTotalDistance;
                    improvementFound = true;
                }
            }
        }

        return improvementFound;
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

    void SetRoute(const std::vector<Address>& newAddresses) {
        addresses_ = newAddresses;
    }

    // Optimize the route using the greedy search algorithm
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

    // Optimize the route using the opt2 heuristic
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

    // Optimize multiple routes using the opt2 heuristic
    static void OptimizeTwoRoutes(Route& route1, Route& route2) {
        try {
            //std::cout << "Inside OptimizeTwoRoutes method...\n";

            bool improved = true;
            while (improved) {
                improved = false;

                //for (size_t i1 = 1; i1 < std::min(static_cast<size_t>(5), route1.addresses_.size() - 2); ++i1) {
                for (size_t i1 = 1; i1 < route1.addresses_.size() - 2; ++i1) {
                    for (size_t j1 = i1 + 1; j1 < route1.addresses_.size() - 1; ++j1) {
                        for (size_t i2 = 1; i2 < route2.addresses_.size() - 2; ++i2) {
                            for (size_t j2 = i2 + 1; j2 < std::min(static_cast<size_t>(5), route1.addresses_.size() - 2); ++j2) {
                            //for (size_t j2 = i2 + 1; j2 < route2.addresses_.size() - 1; ++j2) {
                                // std::cout << "Entering inner loop \n";
                                if (!route1.AnyAddressIsPrime(i1, j1) && !route2.AnyAddressIsPrime(i2, j2)) {
                                    if (route1.SwapAndReverseSegments(route2, i1, j1, i2, j2)) {
                                        improved = true;
                                    }
                                }
                                // std::cout << "Exiting inner loop, improved = " << improved << "\n";
                            }
                        }
                    }
                }
            }
            //std::cout << "Exiting OptimizeTwoRoutes method...\n";
        }
        catch (const std::exception& e) {
            std::cerr << "Exception caught in OptimizeTwoRoutes: " << e.what() << std::endl;
        }
    }
};

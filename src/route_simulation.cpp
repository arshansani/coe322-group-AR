#include "route.cpp"  // Include the class definitions
#include <ctime>
#include <fstream>
#include <sstream>

void WriteRouteToJson(const std::vector<Address>&, const std::string&);
Address GenerateRandomAddress(int, int);

int main() {
    std::srand(std::time(0)); // Seed for random number generation

    // Delivery truck parameters
    int truck_daily_deliveries = 100;
    int num_trucks = 1;
    // Address parameters
    int service_range = 20;
    int prime_latest_delivery_date = 1;
    int regular_latest_delivery_date = 14;
    int num_addresses = 1000;
    double prime_customers_percent = 0.3;
    // Create a quantity of prime addresses and non prime addresses
    int num_prime_addresses = num_addresses * prime_customers_percent;
    int num_regular_addresses = num_addresses * (1 - prime_customers_percent);


    // Generate and add random addresses
    AddressList list;
    for (int i = 0; i < num_prime_addresses; ++i) {
        Address random_address = GenerateRandomAddress(service_range, prime_latest_delivery_date);
        list.AddAddress(random_address);
    }
    for (int i = 0; i < num_regular_addresses; ++i) {
        Address random_address = GenerateRandomAddress(service_range, regular_latest_delivery_date);
        list.AddAddress(random_address);
    }

    // Initialize route so the starting end ending points are the depot
    Route route(list.GetAddresses());

    // Calculate initial total distance
    double initial_distance = route.TotalDistance();
    std::cout << "Initial total distance: " << initial_distance << std::endl;
    WriteRouteToJson(route.GetAddresses(), "route_before_optimization.json");

    // Greedy routing optimization
    route.GreedyRoute();
    double greedy_distance = route.TotalDistance();
    std::cout << "Greedy total distance: " << greedy_distance << std::endl;
    WriteRouteToJson(route.GetAddresses(), "route_after_greedy_search.json");

    // Opt2 routing optimization
    route.OptimizeRoute();
    double optimized_distance = route.TotalDistance();
    std::cout << "Optimized total distance: " << optimized_distance << std::endl;
    WriteRouteToJson(route.GetAddresses(), "route_after_optimization.json");

    return 0;
}

// Generate a random address
Address GenerateRandomAddress(int range, int latest_delivery_date) {
    int x = (std::rand() % (2 * range + 1)) - range;
    int y = (std::rand() % (2 * range + 1)) - range;
    int last_delivery_date = latest_delivery_date; // Random date within a month
    return Address(x, y, last_delivery_date);
}

// Write the list of addresses to a JSON file for plotting using python
void WriteRouteToJson(const std::vector<Address>& addresses, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    file << "["; // Start of JSON array

    for (size_t i = 0; i < addresses.size(); ++i) {
        const Address& addr = addresses[i];
        file << "{\"x\": " << addr.GetX() << ", \"y\": " << addr.GetY() << "}";
        if (i < addresses.size() - 1) {
            file << ", ";
        }
    }

    file << "]"; // End of JSON array
    file.close();
}
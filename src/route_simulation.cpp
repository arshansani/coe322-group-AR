// Name: Arsh Dauwa
// uteid: ae26933
// TACC Username: ArshDauwa

// Name: Arshan Saniei-Sani
// uteid: AS97297
// TACC Username: as97297

#include "route.cpp"
#include <ctime>
#include <fstream>
#include <sstream>

// Initialize functions
void WriteRouteToJson(const std::vector<Address>&, const std::string&);
Address GenerateRandomAddress(int, int);

int main() {
    // Seed for random number generation
    //std::srand(std::time(0)); // Random seed
    std::srand(0); // Comparing the same routes

    // Delivery truck parameters
    int truck_daily_deliveries = 100;
    int num_trucks = 2;
    // Address parameters
    int service_range = 20;
    int num_addresses = 100;
    double prime_customers_percent = 0.0;
    // Create a quantity of prime addresses and non prime addresses
    int num_prime_addresses = num_addresses * prime_customers_percent;
    int num_regular_addresses = num_addresses - num_prime_addresses;
    // Initialize total distances
    double total_initial_distance = 0;
    double total_greedy_distance = 0;
    double total_individual_opt2_distance = 0;
    double total_simultaneous_opt2_distance = 0;

    // Generate random addresses
    AddressList list;
    for (int i = 0; i < num_prime_addresses; ++i) {
        Address random_address = GenerateRandomAddress(service_range, true);
        list.AddAddress(random_address);
    }
    for (int i = 0; i < num_regular_addresses; ++i) {
        Address random_address = GenerateRandomAddress(service_range, false);
        list.AddAddress(random_address);
    }

    // Divide addresses between trucks
    std::vector<Route> trucks(num_trucks, Route());
    for (int i = 0; i < num_addresses; ++i) {
        trucks[i % num_trucks].AddAddress(list.GetAddresses()[i]);
    }

    std::cout << "Route " << 1 << " Size: " << trucks[0].GetAddresses().size() << "\n";
    std::cout << "Route " << 2 << " Size: " << trucks[1].GetAddresses().size() << "\n";

    // Write the individual routes to JSON files
    for (int i = 0; i < num_trucks; ++i) {
        total_initial_distance += trucks[i].TotalDistance();
        std::cout << "Initial Route " << i + 1 << " Total Distance: " << trucks[i].TotalDistance() << "\n";
        std::string filename = "initial_route_" + std::to_string(i) + ".json";
        WriteRouteToJson(trucks[i].GetAddresses(), filename);

        trucks[i].GreedyRoute();
        total_greedy_distance += trucks[i].TotalDistance();
        std::cout << "Greedy Route " << i + 1 << " Total Distance: " << trucks[i].TotalDistance() << "\n";
        filename = "greedy_route_" + std::to_string(i) + ".json";
        WriteRouteToJson(trucks[i].GetAddresses(), filename);

        trucks[i].OptimizeRoute();
        total_individual_opt2_distance += trucks[i].TotalDistance();
        std::cout << "Individual Opt2 Route " << i + 1 << " Total Distance: " << trucks[i].TotalDistance() << "\n\n";
        filename = "individual_opt2_route_" + std::to_string(i) + ".json";
        WriteRouteToJson(trucks[i].GetAddresses(), filename);
    }
    std::cout << "Initial Combined Total Distance: " << total_initial_distance << "\n";
    std::cout << "Greedy Combined Total Distance: " << total_greedy_distance << "\n";
    std::cout << "Individual Opt2 Combined Total Distance: " << total_individual_opt2_distance << "\n\n";

    // Opt 2 Optimize across the two routes
    if (num_trucks == 2) {
        Route::OptimizeTwoRoutes(trucks[0], trucks[1]);

        for (int i = 0; i < num_trucks; ++i) {
            total_simultaneous_opt2_distance += trucks[i].TotalDistance();
            std::cout << "Combined Opt2 Route " << i + 1 << " Total Distance: " << trucks[i].TotalDistance() << "\n";
            std::string filename = "combined_opt2_route_" + std::to_string(i) + ".json";
            WriteRouteToJson(trucks[i].GetAddresses(), filename);
        }
        std::cout << "Simultaneous Opt2 Combined Total Distance: " << total_simultaneous_opt2_distance << "\n";
    }
    else {
        std::cout << "Not enough trucks for multi-route optimization.\n";
    }

/*
    // Initialize route using addresses
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
*/

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
    return; // Used to disable writing to JSON

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

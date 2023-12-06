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

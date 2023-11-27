#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>

class Address {
private:
    int x, y; // Coordinates
    int lastDeliveryDate;

public:
    Address(int x, int y, int lastDeliveryDate) : x(x), y(y), lastDeliveryDate(lastDeliveryDate) {}

    double distance(const Address& other) const {
        return std::sqrt(std::pow(x - other.x, 2) + std::pow(y - other.y, 2));
    }

    int getLastDeliveryDate() const {
        return lastDeliveryDate;
    }
};

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <math.h>

//String to int vector converter
std::vector<int> split(std::string s, std::string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<int> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(std::stoi(token));
    }

    res.push_back(std::stoi(s.substr(pos_start)));
    return res;
}

class Customer {
public:
    int id;
    int xc;
    int yc;
    int demand;
    int readyT;
    int finT;
    int serviceT;
    Customer() :
        id(), xc(), yc(), demand(), readyT(), finT(), serviceT() {}
    double distance(Customer other, double d = 0.0) const {
        double xn = other.xc - xc, yn = other.yc - yc, dist = sqrt(pow(xn, 2) + pow(yn, 2));
        d += dist;
        return d;
    }
    int capacity(Customer& other, int cap) {
        cap -= other.demand;
        if (cap < 0) {
            std::cerr << "Capacity overflow!" << std::endl;
        }
        return cap;
    }
};
class Car {
public:
    int num;
    int cap;
    double time;
    std::vector<int> route;
    Car(std::vector<int>& _route) :
        num(0), cap(200), time(0.0), route(_route) {}
    void routeCheck(std::vector<Customer>& customer) {
        for (int i = 0; i < route.size() - 1; i++) {
            time = customer[route[i]].distance(customer[route[i + 1]], time);
            if (time == -1) {
                break;

            }
            cap = customer[route[i]].capacity(customer[route[i + 1]], cap);
        }

    }
};
void txt_to_customer(std::vector<Customer>& customer) {
    std::ifstream fin;
    fin.open(".\\input\\R1_8_1.TXT");
    if (!fin) {
        std::cerr << "Error in opening the file" << std::endl;
        return;
    }

    Customer temp;
    while (fin >> temp.id >> temp.xc >> temp.yc >> temp.demand >> temp.readyT >> temp.finT >> temp.serviceT) {
        customer.push_back(temp);
    }
    fin.close();
}

void txt_to_cars(std::vector<Car>& cars) {
    std::ifstream fin;
    fin.open(".\\input\\r1_8_1.80_36767.9236.txt");
    std::vector<int> rout;
    std::string temp1;
    std::string delimeter = " ";
    while (std::getline(fin, temp1)) {
        rout = split(temp1, delimeter);
        Car car_temp(rout);
        cars.push_back(car_temp);
    }
}

void test(std::vector<Car>& cars, std::vector<Customer> customer) {
    double totalDist = 0.0, totalTime = 0.0;
    std::ofstream write_distance(".\\output\\distance.txt");
    for (int i = 0; i < cars.size(); i++) {
        cars[i].routeCheck(customer);
        if (cars[i].time == -1) {
            std::cout << i + 1 << ". WRONG!" << std::endl;
        }
        else {
            totalDist += cars[i].time;
            write_distance << std::fixed << i + 1 << ".\t Distance:\t" << cars[i].time << std::endl;
            std::cout << std::fixed << "#" << i + 1 << ".\t Distance:\t " << cars[i].time << std::endl;
            std::cout << "------------------------------------------------------------------------------------------------------------------" << std::endl;
        }
    }
    std::cout << std::fixed << std::endl << "Total distance: " << totalDist;
    write_distance << std::fixed << std::endl << "Total distance: " << totalDist;
    write_distance.close();
}
int main()
{
    std::vector<Customer> customer;
    std::vector<Car> cars;
    txt_to_customer(customer);
    txt_to_cars(cars);
    test(cars, customer);
    return 0;
}


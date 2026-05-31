#pragma once

#include <string>

struct Person {
    int id;
    std::string firstName;
    std::string lastName;
    std::string patronymic;

    int age;
    std::string gender;

    std::string city;
    std::string address;

    std::string email;
    std::string phone;

    std::string position;

    std::time_t createdAt;
    std::time_t updatedAt;
};

// environment.hpp
#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <unordered_map>
#include <string>
#include "object.hpp"
#include <memory>

namespace YOXS_OBJECT {

class Environment {
public:
    std::shared_ptr<Environment> outer;
    std::unordered_map<std::string, std::shared_ptr<Object>> store;

    Environment(std::shared_ptr<Environment> outer = nullptr) : outer(outer) {}
    std::shared_ptr<Object> Get(const std::string& name);
    std::shared_ptr<Object> Set(const std::string& name, std::shared_ptr<Object> val);
};

} //namespace YOXS_OBJECT

#endif // ENVIRONMENT_H
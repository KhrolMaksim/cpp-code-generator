#pragma once

#include <memory>
#include <string>
#include <vector>

namespace cppgen {

class CodeStructure {
public:
  enum class AccessModifier { Public, Protected, Private };

  struct Method {
    std::string returnType;
    std::string name;
    std::string parameters;
    std::string body;
    bool isVirtual = false;
    bool isPureVirtual = false;
    AccessModifier access = AccessModifier::Public;
  };

  struct Field {
    std::string type;
    std::string name;
    std::string defaultValue;
    AccessModifier access = AccessModifier::Private;
  };

  struct Class {
    std::string name;
    std::vector<std::string> baseClasses;
    std::vector<Method> methods;
    std::vector<Field> fields;
    std::vector<std::string> includes;
    std::vector<Class> nestedClasses;
    AccessModifier access = AccessModifier::Public;
  };

  void addClass(const Class &cls) {
    classes.push_back(cls);
  }

  void writeToFiles(const std::string &outputDir) const;

private:
  std::vector<Class> classes;
};

} // namespace cppgen
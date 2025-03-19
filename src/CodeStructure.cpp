#include "CodeStructure.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>

#define USE_CLANG_FORMAT 1

#if defined(USE_CLANG_FORMAT)
#include <cstdlib>
#endif

namespace cppgen {

void CodeStructure::writeToFiles(const std::string &outputDir) const {
  std::filesystem::create_directories(outputDir);

  for (const auto &cls : classes) {
    // Создаем .h файл
    std::ofstream headerFile(outputDir + "/" + cls.name + ".h");
    if (not headerFile.is_open()) {
      std::cerr << "Не удалось создать файл заголовка: " << cls.name << ".h" << std::endl;
      continue;
    }

    // Записываем include guard (pragma once)
    headerFile << "#pragma once\n\n";

    // Записываем includes
    for (const auto &include : cls.includes) {
      headerFile << "#include " << include << "\n";
    }
    headerFile << "\n";

    // Начинаем объявление класса
    headerFile << "class " << cls.name;
    if (not cls.baseClasses.empty()) {
      headerFile << ": public " << cls.baseClasses[0];
      for (size_t i = 1; i < cls.baseClasses.size(); ++i) {
        headerFile << ", public " << cls.baseClasses[i];
      }
    }
    headerFile << "{\n";

    // Группируем методы и поля по модификаторам доступа
    std::map<AccessModifier, std::vector<const Method *>> methodsByAccess;
    std::map<AccessModifier, std::vector<const Field *>> fieldsByAccess;
    std::map<AccessModifier, std::vector<const Class *>> nestedClassesByAccess;

    for (const auto &method : cls.methods) {
      methodsByAccess[method.access].push_back(&method);
    }

    for (const auto &field : cls.fields) {
      fieldsByAccess[field.access].push_back(&field);
    }

    for (const auto &nestedClass : cls.nestedClasses) {
      nestedClassesByAccess[nestedClass.access].push_back(&nestedClass);
    }

    // Функция для записи секции с определенным модификатором доступа
    auto writeAccessSection = [&headerFile](const std::string &modifier, const auto &items,
                                            const auto &writeItem) {
      if (not items.empty()) {
        headerFile << modifier << ":\n";
        for (const auto &item : items) {
          writeItem(*item);
        }
      }
    };

    // Функция для записи метода
    auto writeMethod = [&headerFile](const Method &method) {
      if (method.isVirtual) {
        headerFile << "virtual ";
      }
      headerFile << method.returnType << "" << method.name << "(" << method.parameters << ")";
      if (method.isPureVirtual) {
        headerFile << "= 0";
      }
      headerFile << ";\n";
    };

    // Функция для записи поля
    auto writeField = [&headerFile](const Field &field) {
      headerFile << field.type << "" << field.name;
      if (not field.defaultValue.empty()) {
        headerFile << "= " << field.defaultValue;
      }
      headerFile << ";\n";
    };

    // Функция для записи вложенного класса
    auto writeNestedClass = [&headerFile, &writeAccessSection, &writeMethod,
                             &writeField](const Class &nestedClass) {
      headerFile << "class " << nestedClass.name;
      if (not nestedClass.baseClasses.empty()) {
        headerFile << ": public " << nestedClass.baseClasses[0];
        for (size_t i = 1; i < nestedClass.baseClasses.size(); ++i) {
          headerFile << ", public " << nestedClass.baseClasses[i];
        }
      }
      headerFile << "{\n";

      // Группируем методы и поля вложенного класса
      std::map<AccessModifier, std::vector<const Method *>> nestedMethodsByAccess;
      std::map<AccessModifier, std::vector<const Field *>> nestedFieldsByAccess;

      for (const auto &method : nestedClass.methods) {
        nestedMethodsByAccess[method.access].push_back(&method);
      }

      for (const auto &field : nestedClass.fields) {
        nestedFieldsByAccess[field.access].push_back(&field);
      }

      // Записываем секции вложенного класса
      writeAccessSection("public", nestedMethodsByAccess[AccessModifier::Public], writeMethod);
      writeAccessSection("public", nestedFieldsByAccess[AccessModifier::Public], writeField);
      writeAccessSection("protected", nestedMethodsByAccess[AccessModifier::Protected],
                         writeMethod);
      writeAccessSection("protected", nestedFieldsByAccess[AccessModifier::Protected], writeField);
      writeAccessSection("private", nestedMethodsByAccess[AccessModifier::Private], writeMethod);
      writeAccessSection("private", nestedFieldsByAccess[AccessModifier::Private], writeField);

      headerFile << "};\n";
    };

    // Записываем public секцию
    writeAccessSection("public", methodsByAccess[AccessModifier::Public], writeMethod);
    writeAccessSection("public", fieldsByAccess[AccessModifier::Public], writeField);
    writeAccessSection("public", nestedClassesByAccess[AccessModifier::Public], writeNestedClass);

    // Записываем protected секцию
    writeAccessSection("protected", methodsByAccess[AccessModifier::Protected], writeMethod);
    writeAccessSection("protected", fieldsByAccess[AccessModifier::Protected], writeField);
    writeAccessSection("protected", nestedClassesByAccess[AccessModifier::Protected],
                       writeNestedClass);

    // Записываем private секцию
    writeAccessSection("private", methodsByAccess[AccessModifier::Private], writeMethod);
    writeAccessSection("private", fieldsByAccess[AccessModifier::Private], writeField);
    writeAccessSection("private", nestedClassesByAccess[AccessModifier::Private], writeNestedClass);

    headerFile << "};\n";
    headerFile.close();

    // Создаем .cpp файл
    std::ofstream sourceFile(outputDir + "/" + cls.name + ".cpp");
    if (not sourceFile.is_open()) {
      std::cerr << "Не удалось создать файл реализации: " << cls.name << ".cpp" << std::endl;
      continue;
    }

    sourceFile << "#include \"" << cls.name << ".h\"\n\n";

    // Записываем реализации методов
    for (const auto &method : cls.methods) {
      if (not method.isPureVirtual) {
        sourceFile << method.returnType << "" << cls.name << "::" << method.name << "("
                   << method.parameters << ") {\n"
                   << method.body << "\n}\n\n";
      }
    }

    // Записываем реализации методов вложенных классов
    for (const auto &nestedClass : cls.nestedClasses) {
      for (const auto &method : nestedClass.methods) {
        if (not method.isPureVirtual) {
          sourceFile << method.returnType << "" << cls.name << "::" << nestedClass.name
                     << "::" << method.name << "(" << method.parameters << ") {\n"
                     << method.body << "\n}\n\n";
        }
      }
    }

    sourceFile.close();
  }

#if defined(USE_CLANG_FORMAT)
  // Форматируем сгенерированные файлы с помощью clang-format
  for (const auto &cls : classes) {
    std::string headerFile = outputDir + "/" + cls.name + ".h";
    std::string sourceFile = outputDir + "/" + cls.name + ".cpp";

    std::string formatCmd = "clang-format -i " + headerFile + "" + sourceFile;
    if (std::system(formatCmd.c_str()) != 0) {
      std::cerr << "Ошибка при форматировании файлов для класса: " << cls.name << std::endl;
    }
  }
#endif
}

} // namespace cppgen
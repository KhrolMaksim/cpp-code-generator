#include <iostream>

#include "CodeStructure.hpp"

int main() {
  cppgen::CodeStructure codeGen;

  cppgen::CodeStructure::Class baseClass;
  baseClass.name = "BaseClass";

  // Создаем пример класса
  cppgen::CodeStructure::Class exampleClass;
  exampleClass.name = "Example";
  exampleClass.baseClasses.push_back("BaseClass");
  exampleClass.includes.push_back("<string>");
  exampleClass.includes.push_back("<vector>");
  exampleClass.includes.push_back("<iostream>");
  exampleClass.includes.push_back("\"BaseClass.h\"");

  // Добавляем public методы
  cppgen::CodeStructure::Method method;
  method.returnType = "void";
  method.name = "doSomething";
  method.parameters = "const std::string& input";
  method.body = "std::cout << \"Processing: \"<< input << std::endl;";
  method.access = cppgen::CodeStructure::AccessModifier::Public;
  exampleClass.methods.push_back(method);

  // Добавляем protected виртуальный метод
  method.returnType = "int";
  method.name = "calculate";
  method.parameters = "int a, int b";
  method.body = "return a + b;";
  method.isVirtual = true;
  method.access = cppgen::CodeStructure::AccessModifier::Protected;
  exampleClass.methods.push_back(method);

  // Добавляем private метод
  method.returnType = "void";
  method.name = "internalHelper";
  method.parameters = "";
  method.body = "// Внутренняя логика";
  method.isVirtual = false;
  method.access = cppgen::CodeStructure::AccessModifier::Private;
  exampleClass.methods.push_back(method);

  // Добавляем public поле
  cppgen::CodeStructure::Field field;
  field.type = "std::string";
  field.name = "publicName";
  field.defaultValue = "\"public\"";
  field.access = cppgen::CodeStructure::AccessModifier::Public;
  exampleClass.fields.push_back(field);

  // Добавляем protected поле
  field.type = "int";
  field.name = "protectedValue";
  field.defaultValue = "42";
  field.access = cppgen::CodeStructure::AccessModifier::Protected;
  exampleClass.fields.push_back(field);

  // Добавляем private поле
  field.type = "std::string";
  field.name = "privateData";
  field.defaultValue = "\"private\"";
  field.access = cppgen::CodeStructure::AccessModifier::Private;
  exampleClass.fields.push_back(field);

  // Создаем вложенный класс
  cppgen::CodeStructure::Class nestedClass;
  nestedClass.name = "NestedClass";
  nestedClass.access = cppgen::CodeStructure::AccessModifier::Public;

  // Добавляем методы во вложенный класс
  method.returnType = "void";
  method.name = "nestedMethod";
  method.parameters = "";
  method.body = "std::cout << \"Nested class method called\"<< std::endl;";
  method.isVirtual = false;
  method.access = cppgen::CodeStructure::AccessModifier::Public;
  nestedClass.methods.push_back(method);

  // Добавляем поле во вложенный класс
  field.type = "int";
  field.name = "nestedValue";
  field.defaultValue = "100";
  field.access = cppgen::CodeStructure::AccessModifier::Private;
  nestedClass.fields.push_back(field);

  // Создаем private вложенный класс
  cppgen::CodeStructure::Class privateNestedClass;
  privateNestedClass.name = "PrivateNested";
  privateNestedClass.access = cppgen::CodeStructure::AccessModifier::Private;

  // Добавляем метод в private вложенный класс
  method.returnType = "void";
  method.name = "privateMethod";
  method.parameters = "";
  method.body = "// Private nested class method";
  method.isVirtual = false;
  method.access = cppgen::CodeStructure::AccessModifier::Private;
  privateNestedClass.methods.push_back(method);

  // Добавляем вложенные классы в основной класс
  exampleClass.nestedClasses.push_back(nestedClass);
  exampleClass.nestedClasses.push_back(privateNestedClass);

  // Добавляем классы в генератор
  codeGen.addClass(baseClass);
  codeGen.addClass(exampleClass);

  // Генерируем файлы
  codeGen.writeToFiles("generated");

  std::cout << "Код успешно сгенерирован в директории 'generated'" << std::endl;
  return 0;
}

# C++ Code Generator

Библиотека для генерации C++ кода, позволяющая создавать структуру классов с поддержкой модификаторов доступа и вложенных классов.

## Требования

- C++20 компилятор
- Meson (>= 0.60.0)
- Ninja

## Установка

### Как подпроект в вашем проекте

1. Создайте директорию `subprojects` в корне вашего проекта:
```bash
mkdir subprojects
```

2. Добавьте wrap файл `subprojects/cpp-code-generator.wrap`:
```ini
[wrap-git]
directory = cpp-code-generator
url = https://github.com/KhrolMaksim/cpp-code-generator.git
revision = main
depth = 1 
```

3. В вашем `meson.build` добавьте:
```meson
cpp_code_generator_proj = subproject('cpp-code-generator')
cpp_code_generator_lib = cpp_code_generator_proj.get_variable('lib')

executable('your-project', 
  sources,
  link_with: cpp_code_generator_lib,
  include_directories: cpp_code_generator_proj.get_variable('includes'))
```

4. При сборке вашего проекта используйте:
```bash
meson setup build
ninja -C build
```

### Как отдельная библиотека

1. Клонируйте репозиторий:
```bash
git clone https://github.com/KhrolMaksim/cpp-code-generator.git
cd cpp-code-generator
```

2. Соберите и установите:
```bash
meson setup build
ninja -C build
ninja -C build install
```

## Использование

```cpp
#include <CodeStructure.hpp>

int main() {
    cppgen::CodeStructure codeGen;

    // Создаем класс
    cppgen::CodeStructure::Class exampleClass;
    exampleClass.name = "Example";
    exampleClass.includes.push_back("<string>");

    // Добавляем метод
    cppgen::CodeStructure::Method method;
    method.returnType = "void";
    method.name = "doSomething";
    method.parameters = "const std::string& input";
    method.body = "std::cout << \"Processing: \" << input << std::endl;";
    method.access = cppgen::CodeStructure::AccessModifier::Public;
    exampleClass.methods.push_back(method);

    // Добавляем поле
    cppgen::CodeStructure::Field field;
    field.type = "std::string";
    field.name = "name";
    field.defaultValue = "\"default\"";
    field.access = cppgen::CodeStructure::AccessModifier::Private;
    exampleClass.fields.push_back(field);

    // Добавляем класс в генератор
    codeGen.addClass(exampleClass);

    // Генерируем файлы
    codeGen.writeToFiles("generated");

    return 0;
}
```

## Возможности

- Генерация классов с методами и полями
- Поддержка модификаторов доступа (public, protected, private)
- Поддержка виртуальных и чисто виртуальных методов
- Поддержка вложенных классов
- Генерация заголовочных (.h) и файлов реализации (.cpp)
- Форматирование кода в соответствии с .clang-format

## Лицензия

MIT 

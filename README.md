# Лабораторна робота №7 — Варіант 6

**Тема:** Розробка C++-проєкту з використанням винятків та модульного тестування (C++20).

**Завдання варіанта 6:** Розв’язання звичайних диференціальних рівнянь (ОДР) чисельними методами з обробленням помилок та перевіркою результатів тестами.

## Склад проєкту

- `include/ODETask.h` — оголошення класу для постановки задачі ОДР та збереження траєкторії розв’язку.
- `include/ODEExceptions.h` — класи власних винятків для помилок обчислення.
- `src/ODETask.cpp` — реалізація методів Ейлера, Гойна та Рунге–Кутти четвертого порядку.
- `demo/main.cpp` — приклади розв’язання ОДР, оброблення винятків, повторного генерування та RAII.
- `antipattern/main.cpp` — приклад небажаного використання винятків типів `int` і `std::string`.
- `tests/ODETests.cpp` — модульні тести на базі Google Test.
- `docs/` — звіт Word, приклади виводу та матеріали перевірки.
- `CMakeLists.txt` — налаштування бібліотеки, програм і тестів.
- `CMakePresets.json` — пресет збирання Windows Debug.
- `vcpkg.json` — залежність від Google Test.
- `.gitignore` — файли, які не потрібно додавати до Git.

## Архітектура

```text
ODELab7Variant6
 ├─ ODELib (статична бібліотека)
 │   ├─ ODETask → перевірка параметрів → Euler / Heun / RK4
 │   └─ ODEExceptions → класи винятків
 ├─ ODEDemo → розв’язання ОДР → оброблення помилок → RAII
 ├─ AntipatternDemo → демонстрація невдалих типів винятків
 └─ ODETests → Google Test → перевірка коректності
```

## Оброблення винятків

Програма перевіряє допустимість параметрів задачі та результатів чисельного обчислення. Використовуються `std::invalid_argument` і власні класи `InvalidDerivativeException`, `StepLimitException`, `NumericalOverflowException`.

Методи розв’язання спочатку формують результат у локальному контейнері й лише після успішного завершення замінюють збережену траєкторію. Якщо виникає виняток, попередній результат залишається незмінним (сильна гарантія безпеки).

## Вимоги

Visual Studio з компонентом **Desktop development with C++**, CMake 3.21+, компілятор із підтримкою **C++20**. Для модульних тестів потрібен **Google Test**, який можна встановити через **vcpkg**. Бібліотека SFML не використовується.

## Збирання

Відкрийте кореневу папку проєкту у Visual Studio через **File → Open → Folder**. Середовище розпізнає `CMakeLists.txt`.

Для збирання демонстрацій без Google Test у терміналі:

```powershell
cmake -S . -B build -DBUILD_TESTING=OFF
cmake --build build --config Debug
```

Для повного збирання з Google Test вкажіть шлях до встановленого vcpkg (замініть `C:/path/to/vcpkg` на власний):

```powershell
cmake -S . -B out/build/windows-debug -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE="C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake"
cmake --build out/build/windows-debug
```

У режимі manifest залежність `gtest` указана у `vcpkg.json`. Якщо CMake було налаштовано без vcpkg, видаліть старий кеш або використайте **Project → Delete Cache and Reconfigure**.

## Запуск

Після збирання виберіть цілі `ODEDemo` або `AntipatternDemo` у Visual Studio. Для Ninja у Windows можна також запустити:

```powershell
.\out\build\windows-debug\ODEDemo.exe
.\out\build\windows-debug\AntipatternDemo.exe
```

Якщо проєкт зібрано у папці `build`, використайте відповідний шлях до виконуваних файлів.

## Тестування

За наявності Google Test CMake створює ціль `ODETests`. Для запуску всіх тестів:

```powershell
ctest --test-dir out/build/windows-debug --output-on-failure
```

Для перевірки кількості зареєстрованих тестів:

```powershell
ctest --test-dir out/build/windows-debug -N
```

**Увага:** якщо Google Test не знайдено, CMake виведе попередження, але демонстраційні програми все одно зберуться. У цьому разі `ODETests` не створюється, а успішне збирання **не означає**, що модульні тести виконані.

## Примітки до експериментів

У демонстрації розглянуто методи Ейлера, Гойна та Рунге–Кутти 4-го порядку, перевірку некоректних параметрів і збереження попереднього результату після винятку. Фактичні результати тестів необхідно перевірити на комп’ютері, де встановлено Google Test.

## GitHub

Папки `build/`, `out/` і тимчасові файли Visual Studio не потрібно завантажувати до репозиторію. Перевірте, що вони виключені через `.gitignore`.

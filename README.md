# О проекте

**В ближайшие часы проект изменится. Пожалуйста, не занижайте за изменения после дедлайна**

Проект был выполнен студентом БПИ233, [Амировым Агилем](https://t.me/amirovagil), в рамках задания по курсу Программирование на C++

Условия:
- [Задание 2](./task/fluid-2.pdf)
- [Задание 3](./task/fluid-3.pdf)


# Структура проекта:

Программа эмулирует поведение жидкости в далекой от реальной физике. За один тик происходят какие-то изменения, определяемые алгоритмом и, если произошла какая-то перестановка, программа печатает новую расстановку. 

## Эмулятор
Эмулятор представляет собой класс [Simulator](./include/Simulator.hpp), который отнаследован от класса Base (в целях дальнейших улучшений кода). Он создается с помощью входного файла (дефолтный конструктор создан с другой целью).

Чтобы запустить эмуляцию необходимо вызвать метод run, который управляет всем алгоритмом. 

## Типы

Пользователю предоставлено 4 основных типа, которые могут быть использованы при эмуляции:

- double - встроенный тип C++
- float - встроенный тип C++
- Fixed(N, K) - кастомный тип, который внутри хранит intN_t (Такой тип должен существовать, иначе программа выдаст ошибку)
- FastFixed(N, K) - тот же Fixed, но хранит int_fastN\*_t, где N\* - ближайшее сверху число, которое потенциально может быть использовано программой (N<=64)

[Fixed и FastFixed](./include/Fixed.hpp) ссылаются на один и тот же базовый тип [FixedBase](./include/FixedBase.hpp), в котором определены все допустимые операции и для удобства добавлены min, operator>> и operator=

Все допустимые типы вводятся до компиляции в [CMakeLists.txt](./CMakeLists.txt) в флаг -DTYPES

Используемые типы в обязательном порядке вводятся перед запуском исполняемого кода в специальные флаги

## Поле
Поле задается в файле input.txt

В первой строке через пробел задаются данные в следующем порядке: количество полей по вертикали, по горизонтали (включая границы), ускорение свободного падения g в данной среде и количество типов жидкости count

В следующих count строках задаются типы жидкости и их плотности в следующем формате: "x y", где x - один символ, показывающий тип жидкости, y - число, показывающее плотность данной жидкости.

Далее вводится само поле, с помошью выбранных типов жидкости, " " и "#".

### Формат поля: 
В предоставленном примере используется только один тип жидкости. Были использованы следующие обозначения:

" " - воздух

"." - жидкость

"#" - границы

### Хранение поля:
Все допустимые размеры поля вводятся до компиляции в [CMakeLists.txt](./CMakeLists.txt) в флаг -DSIZES

Поле хранится в классе [Field](./include/Field.hpp), в котором хранятся все поля, считанные из файла. 

### Сохранение текущего состояния
При прерывании программы с помощью сигнала SIGINT, программа сохраняет текущее состояние поля в файл, который перед исполнением был введен в специальный флаг (по умолчанию "../output.txt")

# Как запускать проект

Чтобы получить исполняемый файл необходимо выполнить следующий код:
```bash
cd your_project
mkdir build
cd build
cmake ..  
make
../MyExecutable --p-types="*Тип*" --v-types="*Тип*" --vf-types="*Тип*" --input="*Путь к файлу*" --output="*Путь к файлу*" 
```
Флаги input и output являются опциональными.
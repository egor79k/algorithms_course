/*
    Задача: https://codeforces.com/contest/363/problem/B

    Поликарп приобрел рояль и теперь ищет наилучший способ занести рояль в
свой дом. Для того, чтобы осуществить задуманное, ему придется выломать ровно k
подряд идущих досок в заборе. Так как высокие доски выламывать сложнее,
Поликарп хочет найти такие k последовательных досок, что сумма их высот
минимальна.
    Напишите программу, которая найдет номера k последовательных досок с
наименьшей суммой высот. Обратите внимание, забор не окружает дом Поликарпа,
а находится перед ним (другими словами, забор не зациклен).
*/

#include <iostream>
#include <vector>


int main() {
    int planks_num = 0;
    int piano_width = 0;

    std::cin >> planks_num >> piano_width;
    
    // Выделяем массив
    std::vector<int> fence(planks_num);
    
    // Читаем ввод
    for (int i = 0; i < planks_num; ++i)
        std::cin >> fence[i];

    int sum = 0;

    // Инициализируем сумму
    for (int i = 0; i < piano_width; ++i)
        sum += fence[i];

    int min_sum = sum;
    int min_id = 0;

    for (int i = 0; i < planks_num - piano_width; ++i) {
        // Сдвигаем сумму
        sum -= fence[i];
        sum += fence[i + piano_width];

        if (sum < min_sum) {
            min_sum = sum;
            min_id = i + 1;
        }
    }

    // Выводим результат
    std::cout << min_id + 1;

    return 0;
}
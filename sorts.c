#include "linked_two_way_list.h"
#include <errno.h>
#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

enum SORTING_METHOD { SELECTION, SPLIT_INSERT, SHELL };

#define DEFAULT_ALLOC_SIZE 4

// Сортировка выбором
TwoWayNode* selectionSort(TwoWayNode* array) {
    // Копия массива
    array = LinkedTwoWayList_start(LinkedTwoWayList_copy(array));
    // Длина изначального массива
    size_t length = LinkedTwoWayList_length(array);
    TwoWayNode* sorted_array = NULL; // Отсортированный массив
    size_t sorted_length = 0; // Длина отсортированного массива

    // Заполняем массив n - 1 элементом
    for (size_t i = 0; i < length - 1; i++) {
        // Находим минимальный элемент
        TwoWayNode* min = array;
        for (TwoWayNode* node = array->next; node != NULL; node = node->next)
            if (node->number < min->number)
                min = node;

        // Промежуточный вывод
        printf("%zu:", i);
        if (sorted_array != NULL) {
            printf(" ");
            for (TwoWayNode* n = LinkedTwoWayList_start(sorted_array);
                 n != NULL; n = n->next)
                printf("%f ", n->number);
            printf("|");
        }
        for (TwoWayNode* n = array; n != min; n = n->next)
            printf(" %f", n->number);
        printf(" [%f]", min->number);
        for (TwoWayNode* n = min->next; n != NULL; n = n->next)
            printf(" %f", n->number);
        printf("\n");

        // Добавляем элемент к массиву
        sorted_array = LinkedTwoWayList_appendNumber(sorted_array, min->number);
        // Удаляем элемент из копии изначального массива
        if (min == array)
            // При необходимости смещаем указатель на первый элемент (если
            // первый элемент является минимальным)
            array = LinkedTwoWayList_remove(min);
        else
            LinkedTwoWayList_remove(min);
    }

    // Добавляем последний оставшийся элемент и освобождаем память
    sorted_array = LinkedTwoWayList_appendNumber(sorted_array, array->number);
    free(array);

    // Итоговый вывод
    printf("%zu: ", length - 1);
    for (TwoWayNode* n = LinkedTwoWayList_start(sorted_array); n != NULL;
         n = n->next)
        printf("%f ", n->number);
    printf("|\n");

    return sorted_array;
}

// Сортировка методом половинного деления
TwoWayNode* splitInsertSort(TwoWayNode* array) {
    // Начало исходного массива
    TwoWayNode* node = LinkedTwoWayList_start(array);
    // Отсортированный массив
    TwoWayNode* sorted_array =
        LinkedTwoWayList_new(node->number); // Первый элемент задаётся сразу

    // Проходим по исходному массиву
    for (node = node->next; node != NULL; node = node->next) {
        // Промежуточный вывод
        for (TwoWayNode* n = sorted_array; n != NULL; n = n->next)
            printf("%f ", n->number);
        printf("|");
        for (TwoWayNode* n = node; n != NULL; n = n->next)
            printf(" %f", n->number);
        printf("\n");

        // Начало рассматриваемого отрезка
        TwoWayNode* subarray_start = sorted_array;
        // Конец рассматриваемого отрезка
        TwoWayNode* subarray_end = LinkedTwoWayList_end(sorted_array);
        // Сближаем начало и конец отрезка, пока не найдём точку, слева или
        // справа от которой должен быть новый элемент
        while (subarray_start != subarray_end) {
            // Длина отрезка
            size_t dist =
                LinkedTwoWayList_distance(subarray_start, subarray_end);
            // Середина отрезка
            TwoWayNode* subarray_mid =
                LinkedTwoWayList_at(subarray_start, dist / 2);
            // Если начало и конец - соседние точки, то мы выбираем одну из них
            if (dist == 1) {
                if (node->number < subarray_start->number) {
                    printf("%f < %f\n", node->number, subarray_mid->number);
                    break;
                } else if (node->number > subarray_mid->number) {
                    printf("%f > %f\n", node->number, subarray_mid->number);
                    subarray_start = subarray_end;
                    break;
                }
            }
            // Иначе мы делим отрезок пополам
            else {
                if (node->number < subarray_mid->number) {
                    printf("%f < %f\n", node->number, subarray_mid->number);
                    subarray_end = subarray_mid;
                } else {
                    printf("%f > %f\n", node->number, subarray_mid->number);
                    subarray_start = subarray_mid;
                }
            }

            // Промежуточный вывод
            for (TwoWayNode* n = subarray_start; n != subarray_end; n = n->next)
                printf("%f ", n->number);
            printf("\n");
        }

        // Вставляем элемент слева или справа от найденной точки
        if (node->number < subarray_start->number)
            LinkedTwoWayList_insertNumber(subarray_start, node->number);
        else if (node->number > subarray_start->number)
            LinkedTwoWayList_appendNumber(subarray_start, node->number);
        sorted_array = LinkedTwoWayList_start(sorted_array);
    }

    // Итоговый вывод
    for (TwoWayNode* n = sorted_array; n != NULL; n = n->next)
        printf("%f ", n->number);
    printf("\b\n");

    return sorted_array;
}

// Сортировка Шелла
TwoWayNode* shellSort(TwoWayNode* array) {
    // Отсортированный массив (копия исходного)
    TwoWayNode* sorted_array =
        LinkedTwoWayList_start(LinkedTwoWayList_copy(array));
    // Длина массива
    size_t length = LinkedTwoWayList_length(sorted_array);

    // Делаем сортировки, пока шаг не станет нулевым (т.е. пока мы не сделали
    // сортировку с шагом 1)
    for (size_t step = length / 2; step > 0; step /= 2) {
        // Промежуточный вывод (массив и шаг)
        for (TwoWayNode* n = sorted_array; n != NULL; n = n->next)
            printf("%f ", n->number);
        printf("// h = %zu\n", step);

        // Промежуточный вывод (сортируемые группы)
        for (size_t i = 0; i < step; i++) {
            printf("[");
            for (size_t j = 0; i + j < length; j += step)
                printf("%f ", LinkedTwoWayList_at(sorted_array, i + j)->number);
            printf("\b] ");
        }
        printf("\b\n");

        // Сортируем вставками элементы, смещаясь на шаг вместо 1
        for (size_t i = 0; i < step; i++) {
            for (size_t j = i + step; j < length; j += step) {
                TwoWayNode* n;
                TwoWayNode* m;
                double tmp;
                for (size_t k = j;
                     k > i &&
                     ((n = LinkedTwoWayList_at(sorted_array, k))->number <
                      (m = LinkedTwoWayList_at(sorted_array, k - step))
                          ->number);
                     k -= step) {
                    tmp = n->number;
                    n->number = m->number;
                    m->number = tmp;
                }
            }
        }

        // Промежуточный вывод (отсортированные группы)
        for (size_t i = 0; i < step; i++) {
            printf("[");
            for (size_t j = 0; i + j < length; j += step)
                printf("%f ", LinkedTwoWayList_at(sorted_array, i + j)->number);
            printf("\b] ");
        }
        printf("\b\n");
    }

    // Итоговый вывод
    for (TwoWayNode* n = sorted_array; n != NULL; n = n->next)
        printf("%f ", n->number);
    printf("\b\n");

    return sorted_array;
}

int main(int argc, char* argv[]) {
    TwoWayNode* array = NULL;
    int sorting_method = SELECTION;
    setlocale(LC_ALL, "ru_RU.UTF-8");

    double num_buffer;
    // Работа с аргументами
    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            char* arg = argv[i];
            // Справка
            if (strcmp(arg, "--help") == 0 || strcmp(arg, "-h") == 0) {
                printf("--selection\t| -s\tСортировка выбором\n");
                printf("--split-insert\t| -si\tСортировка методом половинного "
                       "деления\n");
                printf("--shell\t| -sh\tСортировка Шелла\n");
                exit(0);
            }
            // Выбор алгоритма сортировки
            else if (strcmp(arg, "--selection") == 0 || strcmp(arg, "-s") == 0)
                sorting_method = SELECTION;
            else if (strcmp(arg, "--split-insert") == 0 ||
                     strcmp(arg, "-si") == 0)
                sorting_method = SPLIT_INSERT;
            else if (strcmp(arg, "--shell") == 0 || strcmp(arg, "-sh") == 0)
                sorting_method = SHELL;
            // Ввод числа
            else {
                num_buffer = atof(arg);
                if (errno == ERANGE) {
                    printf("Аргумент %d (%s) не является числом\n", i, arg);
                    errno = 0;
                    continue;
                }
                array = LinkedTwoWayList_appendNumber(array, num_buffer);
            }
        }
    }
    // Работа с клавиатурой
    else {
        // Выбор алгоритма сортировки
        printf("Выберите метод сортировки:\n1 - выбором\n2 - метод "
               "половинного деления\n3 - сортировка Шелла\n> ");
        size_t line_length = DEFAULT_ALLOC_SIZE;
        char* line = (char*)malloc(sizeof(char) * DEFAULT_ALLOC_SIZE);
        while (1) {
            getline(&line, &line_length, stdin);
            if (line[0] == '1') {
                sorting_method = SELECTION;
                break;
            } else if (line[0] == '2') {
                sorting_method = SPLIT_INSERT;
                break;
            } else if (line[0] == '3') {
                sorting_method = SHELL;
                break;
            } else {
                printf("> ");
            }
        }
        free(line);
        // Ввод чисел
        line = (char*)malloc(sizeof(char) * DEFAULT_ALLOC_SIZE);
        printf("Введите числа:\n> ");
        getline(&line, &line_length, stdin);
        char* start = line;
        char* end;
        for (num_buffer = strtod(start, &end); start != end;
             num_buffer = strtod(start, &end)) {
            if (errno == ERANGE) {
                errno = 0;
                continue;
            }
            array = LinkedTwoWayList_appendNumber(array, num_buffer);
            start = end;
        }
        free(line);
    }

    TwoWayNode* sorted_array;
    switch (sorting_method) {
    case SELECTION:
    default:
        sorted_array = selectionSort(array);
        break;
    case SPLIT_INSERT:
        sorted_array = splitInsertSort(array);
        break;
    case SHELL:
        sorted_array = shellSort(array);
    }

    LinkedTwoWayList_free(array);
    LinkedTwoWayList_free(sorted_array);

    return 0;
}

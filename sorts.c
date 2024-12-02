#include "linked_two_way_list.h"
#include <errno.h>
#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

enum SORTING_METHOD { SELECTION, SPLIT_INSERT, SHELL };

#define DEFAULT_ALLOC_SIZE 4

TwoWayNode* insertSort(TwoWayNode* array) {
    TwoWayNode* node = LinkedTwoWayList_start(array);
    TwoWayNode* sorted_array = LinkedTwoWayList_new(node->number);

    for (node = node->next; node != NULL; node = node->next) {
        int inserted = 0;
        for (TwoWayNode* sorted_node = sorted_array; sorted_node != NULL;
             sorted_node = sorted_node->prev) {
            if (node->number >= sorted_node->number) {
                LinkedTwoWayList_appendNumber(sorted_node, node->number);
                sorted_array = LinkedTwoWayList_end(sorted_node);
                inserted = 1;
                break;
            }
        }
        if (!inserted)
            LinkedTwoWayList_insertNumber(LinkedTwoWayList_start(sorted_array),
                                          node->number);
    }

    return sorted_array;
}

TwoWayNode* selectionSort(TwoWayNode* array) {
    array = LinkedTwoWayList_start(LinkedTwoWayList_copy(array));
    TwoWayNode* sorted_array = NULL;
    size_t sorted_length = 0;
    size_t length = LinkedTwoWayList_length(array);

    for (size_t i = 0; i < length - 1; i++) {
        TwoWayNode* min = array;
        for (TwoWayNode* node = array->next; node != NULL; node = node->next)
            if (node->number < min->number)
                min = node;

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

        sorted_array = LinkedTwoWayList_appendNumber(sorted_array, min->number);
        if (min == array)
            array = LinkedTwoWayList_remove(min);
        else
            LinkedTwoWayList_remove(min);
    }
    sorted_array = LinkedTwoWayList_appendNumber(sorted_array, array->number);
    free(array);

    printf("%zu: ", length - 1);
    for (TwoWayNode* n = LinkedTwoWayList_start(sorted_array); n != NULL;
         n = n->next)
        printf("%f ", n->number);
    printf("|\n");

    return sorted_array;
}

// Сортировка методом половинного деления
TwoWayNode* splitInsertSort(TwoWayNode* array) {
    TwoWayNode* node = LinkedTwoWayList_start(array);
    TwoWayNode* sorted_array = LinkedTwoWayList_new(node->number);

    for (node = node->next; node != NULL; node = node->next) {
        for (TwoWayNode* n = sorted_array; n != NULL; n = n->next)
            printf("%f ", n->number);
        printf("|");
        for (TwoWayNode* n = node; n != NULL; n = n->next)
            printf(" %f", n->number);
        printf("\n");

        TwoWayNode* subarray_start = sorted_array;
        TwoWayNode* subarray_end = LinkedTwoWayList_end(sorted_array);
        while (subarray_start != subarray_end) {
            size_t dist =
                LinkedTwoWayList_distance(subarray_start, subarray_end);
            TwoWayNode* subarray_mid =
                LinkedTwoWayList_at(subarray_start, dist / 2);
            if (dist == 1) {
                if (node->number < subarray_start->number) {
                    printf("%f < %f\n", node->number, subarray_mid->number);
                    break;
                } else if (node->number > subarray_mid->number) {
                    printf("%f > %f\n", node->number, subarray_mid->number);
                    subarray_start = subarray_end;
                    break;
                }
            } else if (node->number < subarray_mid->number) {
                printf("%f < %f\n", node->number, subarray_mid->number);
                subarray_end = subarray_mid;
            } else if (node->number > subarray_mid->number) {
                printf("%f > %f\n", node->number, subarray_mid->number);
                subarray_start = subarray_mid;
            }
            for (TwoWayNode* n = subarray_start; n != subarray_end; n = n->next)
                printf("%f ", n->number);
            printf("\n");
        }
        if (node->number < subarray_start->number)
            LinkedTwoWayList_insertNumber(subarray_start, node->number);
        else if (node->number > subarray_start->number)
            LinkedTwoWayList_appendNumber(subarray_start, node->number);
        sorted_array = LinkedTwoWayList_start(sorted_array);
    }

    for (TwoWayNode* n = sorted_array; n != NULL; n = n->next)
        printf("%f ", n->number);
    printf("\b\n");

    return sorted_array;
}

TwoWayNode* shellSort(TwoWayNode* array) {
    TwoWayNode* sorted_array =
        LinkedTwoWayList_start(LinkedTwoWayList_copy(array));
    size_t length = LinkedTwoWayList_length(sorted_array);

    size_t step = length / 2;
    for (size_t step = LinkedTwoWayList_length(array) / 2; step > 0;
         step /= 2) {

        for (TwoWayNode* n = sorted_array; n != NULL; n = n->next)
            printf("%f ", n->number);
        printf("// h = %zu\n", step);

        TwoWayNode* subarrays[step];
        TwoWayNode* sorted_sub;
        for (size_t shift = 0; shift < step; shift++) {
            subarrays[shift] = LinkedTwoWayList_new(
                LinkedTwoWayList_at(sorted_array, shift)->number);
            for (size_t i = step; shift + i < length; i += step) {
                subarrays[shift] = LinkedTwoWayList_appendNumber(
                    subarrays[shift],
                    LinkedTwoWayList_at(sorted_array, shift + i)->number);
            }
            subarrays[shift] = LinkedTwoWayList_start(subarrays[shift]);
        }

        for (size_t shift = 0; shift < step; shift++) {
            printf("[");
            for (TwoWayNode* n = subarrays[shift]; n != NULL; n = n->next) {
                printf("%f ", n->number);
            }
            printf("\b] ");
        }
        printf("\b\n");

        for (size_t shift = 0; shift < step; shift++) {
            sorted_sub = insertSort(subarrays[shift]);
            free(subarrays[shift]);
            subarrays[shift] = sorted_sub;
        }

        free(sorted_array);
        sorted_array =
            LinkedTwoWayList_start(LinkedTwoWayList_mergeCopy(step, subarrays));
        for (size_t i = 0; i < step; i++)
            free(subarrays[i]);
    }

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
    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            char* arg = argv[i];
            if (strcmp(arg, "--help") == 0 || strcmp(arg, "-h") == 0) {
                printf("--selection\t| -s\tСортировка выбором\n");
                printf("--split-insert\t| -si\tСортировка методом половинного "
                       "деления\n");
                printf("--shell\t| -sh\tСортировка Шелла\n");
                exit(0);
            } else if (strcmp(arg, "--selection") == 0 ||
                       strcmp(arg, "-s") == 0)
                sorting_method = SELECTION;
            else if (strcmp(arg, "--split-insert") == 0 ||
                     strcmp(arg, "-si") == 0)
                sorting_method = SPLIT_INSERT;
            else if (strcmp(arg, "--shell") == 0 || strcmp(arg, "-sh") == 0)
                sorting_method = SHELL;
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
    } else {
        printf("Выберите метод сортировки:\n1 - вставками\n2 - метод "
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

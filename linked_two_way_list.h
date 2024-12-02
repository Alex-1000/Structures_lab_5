#include <stdlib.h>

// Элемент двусвязного списка
struct TwoWayNode {
    double number;
    struct TwoWayNode* prev;
    struct TwoWayNode* next;
};

// typedef позволяет не набирать [struct] TwoWayNode

// Элемент двусвязного списка
typedef struct TwoWayNode TwoWayNode;

// Создать новый элемент двусвязного списка
TwoWayNode* LinkedTwoWayList_newEmpty() {
    TwoWayNode* node = (TwoWayNode*)malloc(sizeof(TwoWayNode));
    node->number = 0;
    node->prev = NULL;
    node->next = NULL;
    return node;
}
// Создать новый элемент двусвязного списка со значением
TwoWayNode* LinkedTwoWayList_new(double number) {
    TwoWayNode* node = (TwoWayNode*)malloc(sizeof(TwoWayNode));
    node->number = number;
    node->prev = NULL;
    node->next = NULL;
    return node;
}
// Создать новый элемент двусвязного списка с указателями
TwoWayNode* LinkedTwoWayList_newNode(double number, TwoWayNode* previous,
                                     TwoWayNode* next) {
    TwoWayNode* node = (TwoWayNode*)malloc(sizeof(TwoWayNode));
    node->number = number;
    node->prev = previous;
    node->next = next;
    return node;
}

// Найти начало двусвязного списка
TwoWayNode* LinkedTwoWayList_start(TwoWayNode* node) {
    if (node == NULL)
        return NULL;
    while (node->prev != NULL)
        node = node->prev;
    return node;
}
// Найти конец двусвязного списка
TwoWayNode* LinkedTwoWayList_end(TwoWayNode* node) {
    if (node == NULL)
        return NULL;
    while (node->next != NULL)
        node = node->next;
    return node;
}

// Вставить элемент после указанного
TwoWayNode* LinkedTwoWayList_append(TwoWayNode* prev, TwoWayNode* node) {
    if (node == NULL)
        return NULL;
    if (prev == NULL)
        return node;
    TwoWayNode* next = prev->next;

    if (next != NULL)
        next->prev = node;
    node->next = next;

    prev->next = node;
    node->prev = prev;
    return node;
}
// Вставить символ перед указанным
// Возвращает новый элемент
TwoWayNode* LinkedTwoWayList_appendNumber(TwoWayNode* prev, double number) {
    return LinkedTwoWayList_append(prev, LinkedTwoWayList_new(number));
}
// Вставить элемент перед указанным
// Возвращает новый элемент
TwoWayNode* LinkedTwoWayList_insert(TwoWayNode* next, TwoWayNode* node) {
    if (node == NULL)
        return NULL;
    if (next == NULL)
        return node;

    TwoWayNode* prev = next->prev;
    if (prev != NULL)
        prev->next = node;
    node->prev = prev;

    next->prev = node;
    node->next = next;
    return node;
}
// Вставить символ перед указанным
// Возвращает новый элемент
TwoWayNode* LinkedTwoWayList_insertNumber(TwoWayNode* next, double number) {
    return LinkedTwoWayList_insert(next, LinkedTwoWayList_new(number));
}
// Удалить следующий элемент
// Возвращает новый следующий элемент
TwoWayNode* LinkedTwoWayList_removeNext(TwoWayNode* previous) {
    if (previous == NULL || previous->next == NULL)
        return NULL;
    TwoWayNode* next = previous->next->next;
    free(previous->next);
    previous->next = next;
    if (next != NULL)
        next->prev = previous;
    return next;
}
// Удалить предыдущий элемент
// Возвращает новый предыдущий элемент
TwoWayNode* LinkedTwoWayList_removePrevious(TwoWayNode* next) {
    if (next == NULL || next->prev == NULL)
        return NULL;
    TwoWayNode* previous = next->prev->prev;
    free(next->prev);
    if (previous != NULL)
        previous->next = next;
    next->prev = previous;
    return previous;
}
// Удалить данный элемент
// Возвращает следующий элемент
TwoWayNode* LinkedTwoWayList_remove(TwoWayNode* node) {
    TwoWayNode* next = NULL;
    if (node == NULL)
        return NULL;
    if (node->prev != NULL)
        node->prev->next = node->next;
    if (node->next != NULL) {
        next = node->next;
        node->next->prev = node->prev;
    }
    free(node);
    return next;
}

// Элемент с указанным индексом, считая от старта с нуля
TwoWayNode* LinkedTwoWayList_at(TwoWayNode* node, size_t index) {
    TwoWayNode* n = node;
    for (size_t i = 0; i < index; i++) {
        if (n->next == NULL)
            return NULL;
        n = n->next;
    }
    return n;
}

// Отчистить данный элемент и удалить все остальные
TwoWayNode* LinkedTwoWayList_empty(TwoWayNode* node) {
    TwoWayNode* next;
    if (node->next != NULL) {
        for (TwoWayNode* n = node->next; next != NULL; n = next) {
            next = n->next;
            free(n);
        }
    }
    if (node->prev != NULL) {
        for (TwoWayNode* n = node->prev; next != NULL; n = next) {
            next = n->prev;
            free(n);
        }
    }
    node->next = NULL;
    node->number = 0;
    node->prev = NULL;
    return node;
}
// Удалить все элементы массива
void LinkedTwoWayList_free(TwoWayNode* node) {
    TwoWayNode* next;
    if (node->next != NULL) {
        for (TwoWayNode* n = node->next; next != NULL; n = next) {
            next = n->next;
            free(n);
        }
    }
    if (node->prev != NULL) {
        for (TwoWayNode* n = node->prev; next != NULL; n = next) {
            next = n->prev;
            free(n);
        }
    }
    free(node);
}

// Скопировать массив; возвращает начало нового массива
TwoWayNode* LinkedTwoWayList_copy(TwoWayNode* node) {
    TwoWayNode* new_node = NULL;
    for (node = LinkedTwoWayList_start(node); node != NULL; node = node->next)
        new_node = LinkedTwoWayList_appendNumber(new_node, node->number);
    return new_node;
}

// Объединить множество массивов; первым аргументом должно быть передано
// количество массивов; исходные массиве не изменяются
TwoWayNode* LinkedTwoWayList_mergeCopy(size_t count, TwoWayNode* nodes[count]) {
    TwoWayNode* new_node = NULL;
    for (size_t i = 0; i < count; i++) {
        for (TwoWayNode* node = LinkedTwoWayList_start(nodes[i]); node != NULL;
             node = node->next) {
            new_node = LinkedTwoWayList_appendNumber(new_node, node->number);
        }
    }
    return new_node;
}

// Количество элементов списка
size_t LinkedTwoWayList_length(TwoWayNode* node) {
    size_t len = 0;
    for (TwoWayNode* end = LinkedTwoWayList_start(node); end != NULL;
         end = end->next)
        len++;
    return len;
}

// Расстояние между элементами списка
// Если A == B, возвращает 0
// Если A справа от B или в другом списке, возвращает -1
// Если A == NULL или B == NULL, возвращает -1
size_t LinkedTwoWayList_distance(TwoWayNode* A, TwoWayNode* B) {
    if (A == NULL || B == NULL)
        return -1;

    size_t len = 0;
    int found = 0;
    for (TwoWayNode* node = A; node != NULL; node = node->next) {
        len++;
        if (node->next == B) {
            found = 1;
            break;
        }
    }
    return found ? len : -1;
}

// double* LinkedTwoWayList_asArray(TwoWayNode* node) {
//     size_t length = LinkedTwoWayList_length(node);
//     double array[length];

// }

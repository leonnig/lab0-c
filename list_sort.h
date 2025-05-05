/* SPDX-License-Identifier: GPL-2.0 */
#include <list.h>
#include <queue.h>
#include <stdint.h>

#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

typedef int (*list_cmp_func_t)(const struct list_head *,
                               const struct list_head *);

int cmp(const struct list_head *a, const struct list_head *b);

struct list_head *merge(list_cmp_func_t cmp,
                        struct list_head *a,
                        struct list_head *b);

void merge_final(list_cmp_func_t cmp,
                 struct list_head *head,
                 struct list_head *a,
                 struct list_head *b);

void list_sort(struct list_head *head, list_cmp_func_t cmp, bool descend);

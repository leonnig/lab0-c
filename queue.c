#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *head = malloc(sizeof(struct list_head));
    if (!head) {
        return NULL;
    }
    INIT_LIST_HEAD(head);

    return (head);
}

/* Free all storage used by queue */
void q_free(struct list_head *head)
{
    if (head) {
        struct list_head *cur = head->next;
        while (cur != head) {
            element_t *ele = container_of(cur, element_t, list);
            cur = cur->next;
            q_release_element(ele);
        }
        free(head);
    }
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    // element_t *new_node = malloc(sizeof(element_t));
    if (!head || !s)
        return false;
    element_t *new_node = malloc(sizeof(element_t));
    if (!new_node)
        return false;
    new_node->value = malloc(strlen(s) + 1);
    if (!new_node->value) {
        free(new_node);
        return false;
    }
    strncpy(new_node->value, s, strlen(s));
    new_node->value[strlen(s)] = '\0';
    list_add(&new_node->list, head);

    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head || !s)
        return false;
    element_t *new_node = malloc(sizeof(element_t));
    if (!new_node)
        return false;
    new_node->value = malloc(strlen(s) + 1);
    if (!new_node->value) {
        free(new_node);
        return false;
    }
    strncpy(new_node->value, s, strlen(s));
    new_node->value[strlen(s)] = '\0';
    list_add_tail(&new_node->list, head);

    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || (head->next == head))
        return NULL;
    element_t *ele = container_of(head->next, element_t, list);
    if (sp) {
        strncpy(sp, ele->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }
    list_del(&ele->list);

    return ele;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || (head->next == head))
        return NULL;
    element_t *ele = container_of(head->prev, element_t, list);
    if (sp) {
        strncpy(sp, ele->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }
    list_del(&ele->list);

    return ele;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (!head)
        return 0;
    int cnt = 0;
    struct list_head *cur;
    list_for_each(cur, head)
        cnt++;

    return cnt;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    if (!head || head->next == head)
        return NULL;

    struct list_head *ptr = head->next;
    for (const struct list_head *fast = head->next;
         fast->next != head && fast->next->next != head;
         fast = fast->next->next) {
        ptr = ptr->next;
    }
    element_t *ele = list_entry(ptr, element_t, list);
    list_del(ptr);
    free(ele->value);
    free(ele);

    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    if (!head || list_empty(head))
        return false;

    element_t *ele, *dup, *safe;
    struct list_head *ptr, *tmp;
    list_for_each_entry_safe(ele, safe, head, list) {
        ptr = (&(ele->list))->next;
        bool check = false;
        if (ptr == head)
            return true;
        dup = list_entry(ptr, element_t, list);
        while (ptr != head && strcmp(dup->value, ele->value) == 0) {
            check = true;
            tmp = ptr->next;
            list_del(ptr);
            q_release_element(dup);
            ptr = tmp;
            dup = list_entry(ptr, element_t, list);
            safe = dup;
        }
        if (check) {
            list_del(&ele->list);
            q_release_element(ele);
        }
    }
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    if (head && !list_empty(head) && !list_is_singular(head)) {
        struct list_head *one = head->next;
        struct list_head *two = one->next;
        while (one != head && two != head) {
            list_del(one);
            if (two->next == head) {
                list_add_tail(one, head);
                break;
            }
            list_add(one, two);
            one = one->next;
            two = one->next;
        }
    }
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (head && !list_empty(head)) {
        struct list_head *end = head->next;
        struct list_head *target = end->next;
        while (end->next != head) {
            list_move(target, head);
            target = end->next;
        }
    }
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    if (!head || list_empty(head) || list_is_singular(head))
        return;

    struct list_head *start, *end;
    LIST_HEAD(trans);
    LIST_HEAD(tmp);
    while (q_size(head) >= k) {
        start = head->next;
        end = start;
        for (int i = 1; i < k; i++) {
            end = end->next;
        }
        list_cut_position(&trans, head, end);
        q_reverse(&trans);
        list_splice_tail_init(&trans, &tmp);
    }
    list_splice_tail_init(head, &tmp);
    list_splice_tail(&tmp, head);
}

/* Merge two lists in ascending order */
struct list_head *mergeTwoLists(struct list_head *L1,
                                struct list_head *L2,
                                bool descend)
{
    element_t *L1_ele = list_entry(L1->next, element_t, list),
              *L2_ele = list_entry(L2->next, element_t, list), *next;
    while (&L1_ele->list != L1 && &L2_ele->list != L2) {
        if ((strcmp(L1_ele->value, L2_ele->value) <= 0) ^ descend) {
            L1_ele = list_entry(L1_ele->list.next, element_t, list);
        } else {
            next = list_entry(L2_ele->list.next, element_t, list);
            list_move(&L2_ele->list, L1_ele->list.prev);
            L2_ele = next;
        }
    }

    list_splice_tail_init(L2, L1);

    return L1;
}

struct list_head *mergesort_list(struct list_head *head, bool descend)
{
    if (!head || list_empty(head) || list_is_singular(head))
        return head;


    struct list_head *slow = head;
    for (const struct list_head *fast = head->next;
         fast != head && fast->next != head; fast = fast->next->next)
        slow = slow->next;


    LIST_HEAD(cut);
    cut.prev = head->prev;
    head->prev->next = &cut;
    slow->next->prev = &cut;
    cut.next = slow->next;
    slow->next = head;
    head->prev = slow;



    struct list_head *left = mergesort_list(head, descend),
                     *right = mergesort_list(&cut, descend);
    return mergeTwoLists(left, right, descend);
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend)
{
    if (!head || list_empty(head) || list_is_singular(head))
        return;

    mergesort_list(head, descend);
}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}

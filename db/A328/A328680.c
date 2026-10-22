/*
 * A328680  The number of iterations before a repeated value appears when
 *          starting from n and performing the iterative cycle as described in
 *          the comments, which involves setting the next iterative number to
 *          either A053392 or A040115 depending on the current numbers' size
 *          relative to n.
 * https://oeis.org/A328680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 109, u16 n <= 109, u32 n <= 109, u64 n <= 109,
 *            u128 n <= 109, big any n
 * Helpers:   a040115, a053392
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes A328680: the number of iterations
   before a repeated value appears when iterating with the
   rules described in the OEIS entry.  It works for both
   plain integer builds and arbitrary‑precision builds. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/a040115.h"
#include "../utils/a053392.h"

/* --------------------------------------------------------------------
   Simple singly linked list to store seen values.
   -------------------------------------------------------------------- */
typedef struct Node {
    seq_t val;
    struct Node *next;
} Node;

static bool seen_contains(Node *head, seq_t v)
{
    for (Node *p = head; p; p = p->next) {
        if (p->val == v)
            return true;
    }
    return false;
}

static void free_list(Node *head)
{
    while (head) {
        Node *next = head->next;
#ifdef __cplusplus
        delete head;
#else
        free(head);
#endif
        head = next;
    }
}

/* --------------------------------------------------------------------
   Main sequence function
   -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    seq_t start = (seq_t)n;
    seq_t m = a040115(start);

    Node *seen_head = NULL;
    size_t count = 0;

    while (true) {
        if (seen_contains(seen_head, m)) {
            ++count;                /* include the repeating step */
            break;
        }

        /* store current value */
        Node *node;
#ifdef __cplusplus
        node = new Node{m, seen_head};
#else
        node = (Node *)malloc(sizeof(Node));
        node->val = m;
        node->next = seen_head;
#endif
        seen_head = node;
        ++count;

        /* compute next m */
        if (m > start) {
            m = a040115(m);
        } else {
            m = a053392(m);
        }
    }

    free_list(seen_head);
    return (seq_t)count;
}

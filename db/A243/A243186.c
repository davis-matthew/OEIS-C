/*
 * A243186  Nonnegative integers of the form x^2 + 6xy - 5y^2.
 * https://oeis.org/A243186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth smallest nonnegative integer of the form * x^2 +
 *            6xy – 5y^2 (equivalently u^2 – 14y^2 with u = x + 3y)
 * Range:     u8 n <= 3, u16 n <= 35, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

typedef struct {
    seq_t val;  /* the current value u^2 – 14y^2 */
    idx_t y;    /* the current y */
    seq_t u;    /* the current u  (>= ceil_sqrt(14*y^2)) */
} Node;

/* swap two heap nodes */
static inline void heap_swap(Node *a, Node *b)
{
    Node t = *a;
    *a = *b;
    *b = t;
}

/* sift a node down from position i in a heap of size sz */
static void heap_sift_down(Node *heap, size_t sz, size_t i)
{
    while (true) {
        size_t left = i << 1;
        size_t right = left + 1;
        size_t smallest = i;
        if (left <= sz && heap[left].val < heap[smallest].val) smallest = left;
        if (right <= sz && heap[right].val < heap[smallest].val) smallest = right;
        if (smallest == i) break;
        heap_swap(&heap[i], &heap[smallest]);
        i = smallest;
    }
}

/* sift a node up from position i in a heap */
static void heap_sift_up(Node *heap, size_t i)
{
    while (i > 1) {
        size_t p = i >> 1;
        if (heap[p].val <= heap[i].val) break;
        heap_swap(&heap[p], &heap[i]);
        i = p;
    }
}

/* push a node onto a heap of current size sz */
static void heap_push(Node *heap, size_t *sz, Node node)
{
    heap[++(*sz)] = node;
    heap_sift_up(heap, *sz);
}

/* pop the smallest node from a heap of size sz */
static Node heap_pop(Node *heap, size_t *sz)
{
    Node res = heap[1];
    heap[1] = heap[*sz];
    (*sz)--;
    heap_sift_down(heap, *sz, 1);
    return res;
}

/* compute the smallest integer u such that u^2 >= 14*y^2 */
static inline seq_t ceil_sqrt14(idx_t y)
{
    seq_t t = (seq_t)14 * y * y;
    seq_t u = floor_sqrt(t);
    if (u * u < t) ++u;
    return u;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1, but return 0 if asked 0 */

    /* heap will hold at most n entries (y from 0 .. n-1) */
    size_t heap_size = (size_t)n;               /* number of y's = n */
    Node *heap = (Node *)malloc((heap_size + 2) * sizeof(Node));
    if (!heap) return 0;                         /* allocation failure, unlikely */

    size_t sz = 0;
    /* initialise heap with each y's first value */
    for (idx_t y = 0; y < n; ++y) {
        seq_t u = ceil_sqrt14(y);
        seq_t val = u * u - (seq_t)14 * y * y;
        heap_push(heap, &sz, (Node){val, y, u});
    }

    seq_t prev_val = 0;
    bool first = true;
    seq_t result = 0;
    size_t count = 0;

    while (count < n) {
        Node node = heap_pop(heap, &sz);

        if (first || node.val != prev_val) {
            /* new distinct term */
            ++count;
            result = node.val;
            prev_val = node.val;
            if (count == n) break;
            first = false;
        }

        /* generate next value for this y */
        node.u += 1;
        node.val = node.u * node.u - (seq_t)14 * node.y * node.y;
        heap_push(heap, &sz, node);
    }

    free(heap);
    return result;
}

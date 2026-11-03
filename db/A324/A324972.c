/*
 * A324972  Squarefree polygonal numbers P(s,n) with s >= 3 and n >= 3.
 * https://oeis.org/A324972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   P(s,n) = (n^2*(s-2) - n*(s-4)) / 2 for s ≥ 3, n ≥ 3. A324972 is
 *            the sorted list of the square‑free values of P(s,n)
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 9452 (2026-11-03)
 * Helpers:   is_squarefree, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_squarefree.h"
#include "../utils/mem.h"

/* polygonal number P(s,n) with integer arithmetic */
static inline seq_t poly_val(idx_t s, idx_t n)
{
    seq_t nsq = (seq_t)n * (seq_t)n;
    seq_t part1 = nsq * ((seq_t)s - 2);
    seq_t part2 = (seq_t)n * ((seq_t)s - 4);
    return (part1 - part2) / 2;
}

typedef struct {
    seq_t val;      /* current polygonal value */
    idx_t s;        /* polygonal order */
    idx_t n;        /* current n for this s */
} Item;

/* swap two heap items */
static inline void heap_swap(Item *a, Item *b)
{
    Item tmp = *a;
    *a = *b;
    *b = tmp;
}

/* sift an item up the heap */
static void heap_sift_up(Item *heap, size_t idx, size_t size)
{
    while (idx > 0) {
        size_t parent = (idx - 1) >> 1;
        if (heap[parent].val <= heap[idx].val)
            break;
        heap_swap(&heap[parent], &heap[idx]);
        idx = parent;
    }
}

/* sift an item down the heap */
static void heap_sift_down(Item *heap, size_t idx, size_t size)
{
    while (1) {
        size_t left  = (idx << 1) + 1;
        size_t right = left + 1;
        size_t smallest = idx;

        if (left < size && heap[left].val < heap[smallest].val)
            smallest = left;
        if (right < size && heap[right].val < heap[smallest].val)
            smallest = right;

        if (smallest == idx)
            break;
        heap_swap(&heap[idx], &heap[smallest]);
        idx = smallest;
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t *values      = NULL;
    static size_t values_cap  = 0;
    static size_t values_cnt  = 0;
    static seq_t last_val     = 0;
    static bool   have_last   = false;

    static Item *heap     = NULL;
    static size_t heap_cap = 0;
    static size_t heap_size = 0;
    static idx_t   s_max   = 0;

    /* initialization: build a heap containing the first term (n=3)
       for all polygonal orders up to S_MAX. */
    if (!initialized) {
        const idx_t INITIAL_S_MAX = 50000;   /* large enough for n≤10000 */
        heap_cap   = (size_t)INITIAL_S_MAX;
        heap       = mem_new(Item, heap_cap);
        heap_size  = 0;
        s_max      = INITIAL_S_MAX;

        for (idx_t s = 3; s <= s_max; ++s) {
            Item it;
            it.s = s;
            it.n = 3;
            it.val = poly_val(s, 3);
            heap[heap_size++] = it;
        }
        /* build the min‑heap */
        for (size_t i = (heap_size / 2); i-- > 0; )
            heap_sift_down(heap, i, heap_size);

        /* storage for the resulting sequence */
        values_cap = 128;
        values     = mem_new(seq_t, values_cap);

        initialized = true;
    }

    /* generate terms until we have at least n of them */
    while (values_cnt < n) {
        /* pop the smallest item */
        Item cur = heap[0];
        heap[0] = heap[heap_size - 1];
        heap_sift_down(heap, 0, --heap_size);

        /* accept if square‑free and not duplicate */
        if (is_squarefree(cur.val) && (!have_last || cur.val != last_val)) {
            if (values_cnt == values_cap) {
                values_cap <<= 1;
                values = mem_resize(seq_t, values, values_cap);
            }
            values[values_cnt++] = cur.val;
            last_val = cur.val;
            have_last = true;
        }

        /* push the next term for the same s */
        cur.n++;
        cur.val = poly_val(cur.s, cur.n);
        heap[heap_size++] = cur;
        heap_sift_up(heap, heap_size - 1, heap_size);
    }

    return values[n - 1];
}

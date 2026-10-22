/*
 * A352243  Positive integers of the form (x-y)*(x^3-y^3).
 * https://oeis.org/A352243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth smallest distinct integer of the form *
 *            (x-y)*(x^3-y^3) for positive integers x>y
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bubble_down, bubble_up
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/bubble_down.h"
#include "../utils/bubble_up.h"

static inline seq_t compute_value(idx_t d, idx_t t)
{
    seq_t dseq = (seq_t)d;
    seq_t tseq = (seq_t)t;
    seq_t inside = 3 * tseq * tseq + 3 * dseq * tseq + dseq * dseq;
    seq_t d2 = dseq * dseq;
    return d2 * inside;
}

static inline void heap_swap(seq_t *a, seq_t *b)
{
    seq_t temp = *a;
    *a = *b;
    *b = temp;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* number of d-values to pre-insert into the heap */
    #define MAX_D 512

    /* heap node: [0]=value, [1]=d, [2]=t */
    seq_t heap[MAX_D + 1][3];
    size_t heap_size = 0;

    /* insert initial nodes for d=1..MAX_D with t=1 */
    for (idx_t d = 1; d <= MAX_D; ++d) {
        heap[++heap_size][0] = compute_value(d, 1);
        heap[heap_size][1] = (seq_t)d;
        heap[heap_size][2] = (seq_t)1;
    }
    /* build heap */
    for (size_t i = heap_size >> 1; i > 0; --i) bubble_down(heap, heap_size, i);

    seq_t last_val = 0;
    bool first = true;
    idx_t seen = 0;

    while (1) {
        /* pop minimum */
        seq_t val = heap[1][0];
        seq_t d   = heap[1][1];
        seq_t t   = heap[1][2];

        if (heap_size > 1) {
            heap[1][0] = heap[heap_size][0];
            heap[1][1] = heap[heap_size][1];
            heap[1][2] = heap[heap_size][2];
            --heap_size;
            bubble_down(heap, heap_size, 1);
        } else {
            heap_size = 0;
        }

        /* skip duplicates */
        if (first || val != last_val) {
            ++seen;
            if (seen == n) return val;
            last_val = val;
            first = false;
        }

        /* push next value for this d */
        seq_t next_t = t + 1;
        seq_t next_val = compute_value((idx_t)d, (idx_t)next_t);
        heap[++heap_size][0] = next_val;
        heap[heap_size][1] = d;
        heap[heap_size][2] = next_t;
        bubble_up(heap, heap_size);
    }
}

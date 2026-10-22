/*
 * A341622  Numbers that are either already perfect, or a perfect number is
 *          eventually reached if we start doubling them.
 * https://oeis.org/A341622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth smallest number of the form * (2^p − 1) · 2^e
 *            where p is a Mersenne‑prime exponent and 0 ≤ e < p. * The
 *            implementation generates the numbers in increasing order using a
 *            * small min‑heap, without relying on any external libraries
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 61, u64 n <= 101,
 *            u128 n <= 221, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/pow2.h"     /* seq_t pow2(idx_t exp); */

typedef struct {
    seq_t val;   /* current value (base · 2^e)   */
    int   p;     /* Mersenne‑prime exponent    */
    int   e;     /* current exponent of 2 (0 ≤ e < p) */
} heap_node;

/* ---- Small heap routine ----------------------------------- */
static void heap_push(heap_node h[], size_t *size, heap_node node)
{
    size_t i = *size;
    h[i] = node;
    (*size)++;

    /* sift up */
    while (i > 0) {
        size_t parent = (i - 1) >> 1;
        if (h[parent].val <= h[i].val)
            break;
        heap_node tmp = h[parent]; h[parent] = h[i]; h[i] = tmp;
        i = parent;
    }
}

static heap_node heap_pop(heap_node h[], size_t *size)
{
    heap_node ret = h[0];
    size_t last = --(*size);
    h[0] = h[last];

    /* sift down */
    size_t i = 0;
    while (true) {
        size_t left = (i << 1) + 1;
        if (left >= *size) break;
        size_t right = left + 1;
        size_t min_child = left;
        if (right < *size && h[right].val < h[left].val)
            min_child = right;
        if (h[i].val <= h[min_child].val)
            break;
        heap_node tmp = h[i]; h[i] = h[min_child]; h[min_child] = tmp;
        i = min_child;
    }
    return ret;
}

/* ---- Known Mersenne‑prime exponents (first 18, enough for n≈10000) */
static const int mprime_exps[] = {
    2, 3, 5, 7, 13, 17, 19, 31, 61, 89,
    107, 127, 521, 607, 1279, 2203, 2281, 3217
};
static const size_t mprime_cnt = sizeof(mprime_exps) / sizeof(mprime_exps[0]);

/* Compute the nth term. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n<1, but safe */

    /* Lazy initialization of base values and list of usable exponents. */
    static bool initialized = false;
    static seq_t base_vals[18];
    static int    exps[18];
    static size_t valid_cnt = 0;

    if (!initialized) {
        initialized = true;
        /* Determine which exponents are representable for the current seq_t. */
        for (size_t i = 0; i < mprime_cnt; ++i) {
            int p = mprime_exps[i];
            /* For seq_t <= 64 bits, only exponents <= 63 produce non‑overflowing bases. */
            if (sizeof(seq_t) <= 8 && p > 63) {
                continue;          /* Skip this exponent (it would overflow). */
            }
            /* Compute base = 2^p − 1. */
            seq_t powp;
            if (sizeof(seq_t) <= 8 && p <= 63) {
                powp = ((seq_t)1 << p);
            } else {
                powp = pow2(p);    /* arbitrary precision */
            }
            base_vals[valid_cnt] = powp - (seq_t)1;
            exps[valid_cnt] = p;
            ++valid_cnt;
        }
    }

    /* Initialize a tiny heap containing one entry per usable exponent. */
    heap_node heap[18];
    size_t heap_size = 0;
    for (size_t i = 0; i < valid_cnt; ++i) {
        heap_node node;
        node.val = base_vals[i];
        node.p   = exps[i];
        node.e   = 0;
        heap_push(heap, &heap_size, node);
    }

    /* Extract the minimum n times. */
    seq_t result = 0;
    size_t count = 0;
    while (count < (size_t)n) {
        heap_node cur = heap_pop(heap, &heap_size);
        result = cur.val;
        ++count;
        if (cur.e + 1 < cur.p) {          /* push next multiple by 2 */
            cur.val = cur.val << 1;       /* multiply by 2 */
            ++cur.e;
            heap_push(heap, &heap_size, cur);
        }
    }
    return result;
}

/*
 * A262518  Even bisection of A155043.
 * https://oeis.org/A262518
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of steps needed to reach zero when starting from
 *            k=2*n and repeatedly applying k := k - d(k), where d(k) is the
 *            number of divisors of k
 * Range:     u8 n <= 799, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Static caches for divisor count and steps */
static seq_t *steps = NULL;
static uint32_t *dcnt = NULL;
static size_t steps_cap = 0;

/* Ensure arrays have capacity for indices up to max_k (inclusive) */
static void ensure_capacity(idx_t max_k) {
    if (steps_cap > (size_t)max_k) return;          /* already enough */
    size_t old_cap = steps_cap;
    size_t new_cap = old_cap ? old_cap << 1 : 16;
    if (new_cap <= (size_t)max_k) new_cap = (size_t)max_k + 1;   /* at least max_k+1 */

    /* Reallocate arrays */
    steps    = (seq_t*)   realloc(steps,    new_cap * sizeof(seq_t));
    dcnt     = (uint32_t*)realloc(dcnt,     new_cap * sizeof(uint32_t));

    /* Initialize new entries */
    for (size_t i = old_cap; i < new_cap; ++i) {
        dcnt[i] = 0;
    }

    /* Incrementally update divisor counts for new indices */
    for (size_t i = 1; i < new_cap; ++i) {
        /* First multiple of i that is >= old_cap */
        if (old_cap == 0) {
            for (size_t j = i; j < new_cap; j += i) {
                dcnt[j]++;                     /* j > 0 always */
            }
        } else {
            size_t start = ((old_cap + i - 1) / i) * i;
            for (size_t j = start; j < new_cap; j += i) {
                if (j > 0) dcnt[j]++;           /* skip j==0 */
            }
        }
    }

    /* Compute steps for the newly added indices */
    if (old_cap == 0) {
        steps[0] = (seq_t)0;                /* base case */
        if (new_cap > 1)
            steps[1] = steps[1 - dcnt[1]] + (seq_t)1;
        size_t start_k = 2;
    } else {
        size_t start_k = old_cap;
    }
    size_t start_k = (old_cap == 0) ? 1 : old_cap;
    for (size_t k = start_k; k < new_cap; ++k) {
        size_t target = k - dcnt[k];
        steps[k] = steps[target] + (seq_t)1;
    }

    steps_cap = new_cap;
}

seq_t a(idx_t n) {
    idx_t start = n * 2;
    ensure_capacity(start);
    return steps[start];
}

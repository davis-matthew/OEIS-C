/*
 * A356621  a(n) = number of k-tuples (u(1), u(2), ..., u(k)) with 1 <= u(1) <
 *          u(2) < ... < u(k) <= n such that u(i) - u(i-1) <= 5 for i =
 *          2,...,k.
 * https://oeis.org/A356621
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=1, a(2)=4, a(3)=11, a(4)=26, a(5)=57, a(6)=119,
   a(n)=3*a(n-1)-2*a(n-2)-a(n-6)+a(n-7)  for n≥7. */

#include <stdlib.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static const seq_t init_vals[7] = {0, 1, 4, 11, 26, 57, 119};

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;

    /* Initialise cache for the first 7 terms. */
    if (cap == 0) {
        cap = 7;
        cache = seq_alloc(cap);
        if (!cache) return (seq_t)0;          /* allocation failed */
        for (size_t i = 0; i < 7; ++i) cache[i] = init_vals[i];
    }

    /* Extend cache if required. */
    if ((size_t)n + 1 > cap) {
        size_t old_cap = cap;
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = seq_realloc(cache, old_cap, new_cap);
        if (!tmp) return (seq_t)0;            /* allocation failed */
        cache = tmp;
        cap = new_cap;

        /* Compute new terms only once, starting from the first index
           that needs the recurrence. */
        size_t start = (old_cap < 7) ? 7 : old_cap;
        for (size_t i = start; i < cap; ++i) {
            seq_t val = 3 * cache[i - 1]
                        - 2 * cache[i - 2]
                        -  cache[i - 6]
                        +  cache[i - 7];
            cache[i] = val;
        }
    }

    /* Return the requested term. */
    return cache[(size_t)n];
}

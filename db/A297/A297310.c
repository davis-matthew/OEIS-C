/*
 * A297310  Number of n X 4 0..1 arrays with every 1 horizontally or
 *          antidiagonally adjacent to 1 or 2 neighboring 1s.
 * https://oeis.org/A297310
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A297310: Number of n×4 0/1 arrays with every 1
   horizontally or antidiagonally adjacent to 1 or 2 neighboring 1s.
   Empirical recurrence: a(n) = 4*a(n-1) + 26*a(n-2) + 22*a(n-3)
                      - 43*a(n-4) - a(n-5) + 7*a(n-6) - a(n-7) - a(n-8)
   for n > 9, with initial terms a(1)=7, a(2)=66, a(3)=497,
   a(4)=3808, a(5)=29212, a(6)=223995, a(7)=1717882,
   a(8)=13174266, a(9)=101033369. */
#include <stdlib.h>
#include <string.h>

/* External linkage required for the evaluation harness */
seq_t a(idx_t n)
{
    /* Static cache of computed values (a(1) .. a(computed)) */
    static seq_t *cache = NULL;
    static idx_t computed = 0;   /* number of terms stored */
    static idx_t capacity = 0;   /* allocated size of cache bytes */

    /* Ensure cache is large enough for requested index n */
    if ((idx_t)n > capacity) {
        idx_t new_cap = capacity ? capacity * 2 : 16;
        while (new_cap < (idx_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (cache) {
            memcpy(tmp, cache, computed * sizeof(seq_t));
            free(cache);
        }
        cache = tmp;
        capacity = new_cap;
    }

    /* Initial terms to seed the recurrence */
    const seq_t init[9] = {
        (seq_t)7,          /* a(1) */
        (seq_t)66,         /* a(2) */
        (seq_t)497,        /* a(3) */
        (seq_t)3808,       /* a(4) */
        (seq_t)29212,      /* a(5) */
        (seq_t)223995,     /* a(6) */
        (seq_t)1717882,    /* a(7) */
        (seq_t)13174266,   /* a(8) */
        (seq_t)101033369   /* a(9) */
    };

    /* Populate cache with initial values up to min(9, n) */
    if (computed < 9 && computed < n) {
        do {
            cache[computed] = init[computed];
            ++computed;
        } while (computed < 9 && computed < n);
    }

    /* Compute remaining terms using the recurrence */
    while (computed < n) {
        seq_t pos = 4 * cache[computed - 1]
                  + 26 * cache[computed - 2]
                  + 22 * cache[computed - 3]
                  + 7  * cache[computed - 6];
        seq_t neg = 43 * cache[computed - 4]
                  +      cache[computed - 5]
                  +      cache[computed - 7]
                  +      cache[computed - 8];
        cache[computed] = pos - neg;   /* a(computed+1) */
        ++computed;
    }

    return cache[n - 1];
}

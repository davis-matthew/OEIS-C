/*
 * A260509  Number of graphs on labeled vertices {x, y, 1, 2, ..., n}, such
 *          that there is a partition of the vertices into V_1 and V_2 with x
 *          in V_1, y in V_2, every v in V_1 adjacent to an even number of
 *          vertices in V_2, and every v in V_2 adjacent to an even number of
 *          vertices in V_1.
 * https://oeis.org/A260509
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   pow2, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A260509
   Recurrence: a(0) = 1
                a(n) + (2^n - 1)*a(n-1) = 2^(C(n+2,2)-1)
   where C(a,b) is binomial(a,b) */
#include <stdlib.h>                 // for malloc, realloc
#include "../utils/pow2.h"          // seq_t pow2(idx_t)
#include "../utils/seq_alloc.h"     // seq_t *seq_alloc(size_t)
#include "../utils/seq_realloc.h"   // seq_t *seq_realloc(seq_t*,size_t,size_t)

/* Compute A260509 term */
seq_t a(idx_t n)
{
    /* Handle base case */
    if (n == 0)
        return (seq_t)1;

    /* Memoization: static cache */
    static seq_t *cache = NULL;     /* cache[0..max_n] */
    static size_t cap = 0;          /* current allocated capacity */
    static size_t len = 0;          /* number of values stored (len-1 is max index computed) */

    /* Grow cache if necessary */
    if (n >= cap) {
        size_t new_cap = (cap == 0) ? (n + 1) : (cap * 2);
        if (new_cap <= n) new_cap = n + 1;
        seq_t *new_arr = seq_realloc(cache, cap, new_cap);
        /* If reallocation fails, new_arr will be NULL and we simply exit (undefined behaviour) */
        cache = new_arr;
        cap = new_cap;
    }

    /* Compute missing values up to n */
    for (idx_t i = len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
            continue;
        }

        /* exponent for 2^(C(n+2,2)-1) */
        idx_t exp = (i + 2) * (i + 1) / 2 - 1;   /* fits in 64‑bit for typical n */
        seq_t term = pow2(exp);                  /* 2^(C(i+2,2)-1) */

        /* (2^i - 1) * a(i-1) */
        seq_t two_pow_i = pow2(i);
        seq_t sub = two_pow_i - (seq_t)1;
        seq_t prod = sub * cache[i - 1];

        cache[i] = term - prod;
    }

    len = n + 1;     /* we have computed up to index n */

    return cache[n];
}

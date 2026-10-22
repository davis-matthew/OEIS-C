/*
 * A322340  Number of compositions (ordered partitions) of n into square
 *          pyramidal numbers (A000330).
 * https://oeis.org/A322340
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 40, u32 n <= 79, u64 n <= 155,
 *            u128 n <= 308, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A322340: Number of compositions (ordered partitions) of n into
   square pyramidal numbers (A000330).
   Recurrence:
     a(0) = 1
     a(n) = Σ_{k≥1, s_k≤n} a(n - s_k) ,
     where s_k = k(k+1)(2k+1)/6. */

#include <stdlib.h>

/* External interface required by the build system */
seq_t a(idx_t n)
{
    /* Static cache for computed values */
    static seq_t *a_cache = NULL;
    static idx_t   a_cap  = 0;

    /* Ensure base case a(0) is defined */
    if (a_cache == NULL) {
        a_cache = (seq_t *)calloc(1, sizeof(seq_t));
        a_cache[0] = 1;
        a_cap = 1;
    }

    /* If n exceeds cached range, grow the cache */
    if (n >= a_cap) {
        idx_t old_cap = a_cap;
        idx_t new_cap = n + 1;
        seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * new_cap);
        /* Copy existing values */
        for (idx_t i = 0; i < old_cap; ++i)
            new_cache[i] = a_cache[i];
        free(a_cache);
        a_cache = new_cache;
        a_cap = new_cap;

        /* Compute new entries using the recurrence */
        for (idx_t i = old_cap; i <= n; ++i) {
            seq_t sum = 0;
            for (idx_t k = 1;; ++k) {
                idx_t s = k * (k + 1) * (2 * k + 1) / 6; /* square pyramidal number */
                if (s > i) break;
                sum += a_cache[i - s];
            }
            a_cache[i] = sum;
        }
    }

    return a_cache[n];
}

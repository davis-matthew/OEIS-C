/*
 * A357290  a(n) = number of subsets S of {1,2,...,n} having more than 2
 *          elements such that (sum of least two elements of S) > difference
 *          between greatest two elements of S.
 * https://oeis.org/A357290
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 3*a(n-1) - a(n-2) - 2*a(n-3) - a(n-4) +
                     a(n-5) + 3*a(n-6) - 2*a(n-7),
   with initial values
   a(0)=0, a(1)=0, a(2)=0, a(3)=1, a(4)=5, a(5)=15, a(6)=39. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static cache for memoization: dynamic array of seq_t */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;          /* number of pre‑computed entries */

    /* If we already have the requested value, return it. */
    if (n < cache_len)
        return cache[n];

    /* Determine the new size we need (at least n+1, but for small n
       we also need space for the initial 7 values). */
    idx_t new_len = (n >= 7) ? (n + 1) : 7;

    /* Reallocate the cache if necessary */
    seq_t *new_cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
    if (!new_cache) {                     /* allocation failed – terminate */
        exit(1);
    }
    cache = new_cache;

    /* If we have not yet set the initial values, do it now. */
    if (cache_len < 7) {
        cache[0] = 0;
        cache[1] = 0;
        cache[2] = 0;
        cache[3] = 1;
        cache[4] = 5;
        cache[5] = 15;
        cache[6] = 39;
    }

    /* Compute only the new entries that are not yet known. */
    idx_t start = (cache_len > 7) ? cache_len : 7;  /* start after the known values */
    for (idx_t i = start; i < new_len; ++i) {
        /* Use the recurrence: a(i) = 3*a(i‑1) - a(i‑2) - 2*a(i‑3) - a(i‑4)
                                 + a(i‑5) + 3*a(i‑6) - 2*a(i‑7) */
        seq_t term1 = 3 * cache[i - 1];
        seq_t term2 = cache[i - 5];
        seq_t term3 = 3 * cache[i - 6];
        seq_t sum_pos = term1 + term2 + term3;

        seq_t term4 = cache[i - 2];
        seq_t term5 = 2 * cache[i - 3];
        seq_t term6 = cache[i - 4];
        seq_t term7 = 2 * cache[i - 7];
        seq_t sum_neg = term4 + term5 + term6 + term7;

        cache[i] = sum_pos - sum_neg;
    }

    /* Update the length of the cache */
    cache_len = new_len;
    return cache[n];
}

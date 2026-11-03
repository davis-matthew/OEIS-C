/*
 * A263899  Number of length n arrays of permutations of 0..n-1 with each
 *          element moved by -2 to 2 places and the total absolute value of
 *          displacements not greater than 2*(n-1).
 * https://oeis.org/A263899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-3) + a(n-4) + a(n-9) - (3*a(n-5) +
 *            2*a(n-7)) , with a(1)=1, a(2)=2, a(3)=6, a(4)=13, a(5)=31,
 *            a(6)=73, a(7)=172, a(8)=399, a(9)=932
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 27, u64 n <= 53, u128 n <= 105,
 *            big any n
 * Validated: standard, pass, 46 terms through n = 186 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cap   = 0;

    if (n < 1) return 0;              /* out of range – undefined for the problem */

    /* Make sure there is space for indices up to n */
    ensure_cache(n, &cache, &cap);

    /* Initialise the first nine terms once */
    if (cap >= 10 && cache[1] == 0) {
        cache[1] = 1;  cache[2] = 2;  cache[3] = 6;
        cache[4] = 13; cache[5] = 31; cache[6] = 73;
        cache[7] = 172; cache[8] = 399; cache[9] = 932;
    }

    /* Compute the remaining terms using the recurrence */
    for (idx_t i = 10; i <= n; ++i) {
        if (cache[i] != 0) continue;  /* already computed by a previous call */

        /* term = 2*a(i-1) + 2*a(i-3) + a(i-4) + a(i-9) */
        seq_t term  = cache[i-1] << 1;   /* 2*a(i-1) */
        term += cache[i-3] << 1;         /* 2*a(i-3) */
        term += cache[i-4];
        term += cache[i-9];

        /* subtract = 3*a(i-5) + 2*a(i-7) */
        seq_t sub   = cache[i-5] << 1;   /* 2*a(i-5) */
        sub += cache[i-5];               /* +1*a(i-5) => 3*a(i-5) */
        sub += cache[i-7] << 1;          /* +2*a(i-7) */

        term -= sub;
        cache[i] = term;
    }

    return cache[n];
}

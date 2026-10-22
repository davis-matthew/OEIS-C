/*
 * A304297  Number of nX3 0..1 arrays with every element unequal to 1, 2, 5 or
 *          8 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A304297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 22, u32 n <= 44, u64 n <= 88, u128 n <= 176,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A304297 – number of n×3 0..1 arrays with an upper‑left zero
   that avoid king‑move adjacency conflicts.  The values satisfy the
   linear recurrence
      a(n) = a(n‑2) + 3·a(n‑3) + 2·a(n‑4)
           – 4·a(n‑6) – 2·a(n‑7) – a(n‑8) – 2·a(n‑9),
   for n ≥ 10, with initial values
      a(1)=1, a(2)=2, a(3)=3, a(4)=5, a(5)=6,
      a(6)=14, a(7)=22, a(8)=33, a(9)=57.               */

#include <stdlib.h>
#include <stddef.h>

static const seq_t init[10] = {0, 1, 2, 3, 5, 6, 14, 22, 33, 57};

seq_t a(idx_t n)
{
    /* Direct return for the first nine terms. */
    if (n < 10)
        return init[n];

    /* Static cache for arbitrary order of calls. */
    static seq_t *cache      = NULL; /* array of seq_t */
    static idx_t  cache_cap  = 0;    /* current allocated size (n + 1) */
    static idx_t  cache_max  = 0;    /* largest index computed so far */

    /* Grow the cache if necessary. */
    if (n + 1 > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp)      /* unrecoverable allocation failure */
            return init[0];
        cache = tmp;
        cache_cap = n + 1;
        for (idx_t i = cache_max + 1; i <= n; ++i)
            cache[i] = 0;
    }

    /* Initialise cache with known small values if not already done. */
    if (cache_max < 9) {
        idx_t up = n < 9 ? n : 9;
        for (idx_t i = 1; i <= up; ++i)
            cache[i] = init[i];
        cache_max = up;
    }

    /* Compute missing terms using the recurrence. */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t pos = cache[i-2] + 3 * cache[i-3] + 2 * cache[i-4];
        seq_t neg = 4 * cache[i-6] + 2 * cache[i-7] + cache[i-8] + 2 * cache[i-9];
        cache[i] = pos - neg;
    }
    cache_max = n;

    return cache[n];
}

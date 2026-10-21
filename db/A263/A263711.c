/*
 * A263711  Number of length n arrays of permutations of 0..n-1 with each
 *          element moved by -2 to 2 places and every four consecutive
 *          elements having its maximum within 4 of its minimum.
 * https://oeis.org/A263711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-3) - a(n-4) + 2*a(n-5) - a(n-6) + a(n-7)
 * Range:     u8 n <= 12, u16 n <= 25, u32 n <= 53, u64 n <= 107,
 *            u128 n <= 210, big any n
 * Helpers:   _count_perm_, _dfs_perm_
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical initial values: a(1)=1, a(2)=2, a(3)=6, a(4)=14, a(5)=31, a(6)=34, a(7)=39, a(8)=46, a(9)=64, a(10)=104, a(11)=161, a(12)=249, a(13)=385, a(14)=561, a(15)=845, a(16)=1254, a(17)=1871, a(18)=2833 */
/* For n≤18 the values are computed by exhaustive search, for n>18 the recurrence is used. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/_count_perm_.h"
#include "../utils/_dfs_perm_.h"

/* ------------------------------------------------------------------ */
/*  Enumerate permutations 0..n-1 satisfying the bandwidth ≤2   and   */

/* ------------------------------------------------------------------ */
/*  Main sequence function                                                */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined, but harmless */

    static seq_t *cache = NULL;
    static idx_t cache_max = 0;   /* largest n already computed */

    if (n > cache_max) {
        size_t new_size = (size_t)(n + 1);        /* 1‑based indexing */
        if (cache == NULL) {
            cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        } else {
            cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        }
        if (cache == NULL)
            abort();           /* memory exhausted */

        /* Initialise newly allocated slots */
        for (idx_t i = cache_max + 1; i <= n; ++i) {
            if (i <= 18) {
                cache[i] = _count_perm_(i);
            } else {
                /* Empirical recurrence: a(n) = a(n-1) + a(n-3) - a(n-4) + 2*a(n-5) - a(n-6) + a(n-7) */
                seq_t v = cache[i - 1];
                v += cache[i - 3];
                v -= cache[i - 4];
                v += cache[i - 5] * (seq_t)2;
                v -= cache[i - 6];
                v += cache[i - 7];
                cache[i] = v;
            }
        }
        cache_max = n;
    }

    return cache[n];
}

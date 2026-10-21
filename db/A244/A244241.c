/*
 * A244241  Number of partitions of n into 5 parts such that every i-th
 *          smallest part (counted with multiplicity) is different from i.
 * https://oeis.org/A244241
 *
 * Offset:    20    (a(20) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) - a(n-5) - a(n-6) - a(n-7) + a(n-8) +
 *            a(n-9) + a(n-10) - a(n-13) - a(n-14) + a(n-15) for n > 55.
 *            Initial terms a(20)…a(55) are computed by a brute‑force search
 * Range:     u8 n <= 34, u16 n <= 120, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   brute_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* malloc, realloc */
#include "../utils/brute_count.h"

seq_t a(idx_t n)
{
    if (n < 20) return 0;          /* offset behaviour irrelevant */

    /*  static cache  */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_last = 0;
    const idx_t precompute_limit = 55;   /* compute first 36 terms by brute */

    /*  ensure cache has space for indices up to n  */
    if (cache_cap < n + 1) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) {               /* out of memory – aborting is safe here */
            free(cache);
            cache = NULL;
            cache_cap = 0;
            cache_last = 0;
            return 0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /*  compute missing terms  */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        if (i <= precompute_limit) {
            cache[i] = brute_count(i);
        } else {
            /*  recurrence – compute positive part first to avoid unsigned underflow  */
            seq_t pos_sum = cache[i-1] + cache[i-2] + cache[i-8] + cache[i-9]
                           + cache[i-10] + cache[i-15];
            pos_sum -= cache[i-5];
            pos_sum -= cache[i-6];
            pos_sum -= cache[i-7];
            pos_sum -= cache[i-13];
            pos_sum -= cache[i-14];
            cache[i] = pos_sum;
        }
    }
    cache_last = n;
    return cache[n];
}

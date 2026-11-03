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
 * Validated: standard, pass, 65 terms through n = 937 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* malloc, realloc */
#include "../utils/mem.h"

static seq_t brute_count(idx_t n)
{
    seq_t count = 0;
    if (n < 20) return 0;          /* minimise loop ranges */
    /* parts p1<=p2<=p3<=p4<=p5, p_i != i (i=1..5) */
    for (idx_t p1 = 2; p1 <= n-4; ++p1) {          /* p1 >= 2 */
        if (p1 == 1) continue;                    /* safety */
        for (idx_t p2 = p1; p2 <= n-p1-3; ++p2) { /* p2 >= p1, p2 != 2 */
            if (p2 == 2) continue;
            for (idx_t p3 = p2; p3 <= n-p1-p2-2; ++p3) { /* p3 >= p2, p3 != 3 */
                if (p3 == 3) continue;
                for (idx_t p4 = p3; p4 <= n-p1-p2-p3-1; ++p4) { /* p4 >= p3, p4 != 4 */
                    if (p4 == 4) continue;
                    idx_t p5 = n - p1 - p2 - p3 - p4;
                    if (p5 < p4) continue;          /* keep non‑decreasing */
                    if (p5 == 5) continue;          /* p5 != 5 */
                    ++count;
                }
            }
        }
    }
    return count;
}

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
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) {               /* out of memory – aborting is safe here */
            mem_free(cache);
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

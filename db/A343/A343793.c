/*
 * A343793  Number of ordered partitions of an n-set without blocks of size
 *          10.
 * https://oeis.org/A343793
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(n) = ∑_{k=1..n} C(n,k) * a(n-k) – (n≥10 ? C(n,10) *
 *            a(n-10) : 0) where C(n,k) = n!/(k!(n-k)!)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cached values */
    static size_t cap  = 0;       /* capacity of cache buffer */
    static size_t len  = 0;       /* number of values currently stored (next needed index = len) */

    /* Grow cache if needed */
    if (len <= n) {
        size_t new_cap = (cap == 0) ? 8 : cap << 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failed – return 0 (unlikely in tests) */
        cache = tmp;
        cap   = new_cap;

        /* Compute missing values */
        for (idx_t idx = len; idx <= n; ++idx) {
            if (idx == 0) {
                cache[0] = (seq_t)1;
                continue;
            }

            seq_t sum = 0;
            seq_t bin = 1;   /* C(idx,0) */
            /* k runs from 1 to idx; compute C(idx,k) iteratively */
            for (idx_t k = 1; k <= idx; ++k) {
                bin = bin * (seq_t)(idx - k + 1) / (seq_t)k;   /* C(idx,k) */
                sum += bin * cache[(size_t)(idx - k)];
            }

            /* subtract contributions with a block of size 10, if allowed */
            if (idx >= 10) {
                seq_t bin10 = 1;
                for (idx_t i = 1; i <= 10; ++i)
                    bin10 = bin10 * (seq_t)(idx - i + 1) / (seq_t)i;   /* C(idx,10) */
                sum -= bin10 * cache[(size_t)(idx - 10)];
            }
            cache[(size_t)idx] = sum;
        }
        len = (size_t)n + 1;
    }
    return cache[(size_t)n];
}

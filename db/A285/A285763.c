/*
 * A285763  a(n) = a(a(n - 2)) + a(n - a(n - 2)), with a(1) = 1, a(2) = a(3) =
 *          a(4) = 2, a(5) = 3.
 * https://oeis.org/A285763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(a(n-2)) + a(n - a(n-2)) initial values: a(1)=1;
 *            a(2)=a(3)=a(4)=2; a(5)=3
 * Range:     u8 n <= 497, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization; index 0 unused */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t computed = 0;   /* maximum index computed so far */

    if (n == 0)    /* safety: not used in OEIS offset, but avoid UB */
        return 0;

    /* Grow cache if needed */
    if (cache == NULL || n >= cache_cap) {
        idx_t new_cap = (n + 1) > (cache_cap * 2) ? (n + 1) : (cache_cap * 2);
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (new_cache == NULL)
            return 0;    /* allocation failed: return zero (unlikely in tests) */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t k = computed + 1; k <= n; ++k) {
        if (k == 1) {
            cache[1] = 1;
        } else if (k <= 5) {
            if (k <= 4)
                cache[k] = 2;
            else /* k == 5 */
                cache[k] = 3;
        } else {
            /* Recurrence: a(k) = a(a(k-2)) + a(k - a(k-2)) */
            seq_t val_k_minus_2 = cache[k - 2];
            idx_t idx1 = (idx_t)val_k_minus_2;      /* a(k-2) fits in idx_t */
            idx_t idx2 = k - idx1;                  /* k - a(k-2) ≥ 1 */
            cache[k] = cache[idx1] + cache[idx2];
        }
    }
    computed = n;
    return cache[n];
}

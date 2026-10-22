/*
 * A334535  a(1) = 1, a(2) = 2, and for n > 2, a(n) =
 *          2*a(a(n-i))+a(n-1)-a(n-2) where i >= 2 is the smallest number that
 *          satisfies a(n-i) < n.
 * https://oeis.org/A334535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 1350, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A334535
    a(1) = 1, a(2) = 2.
    For n > 2:
        a(n) = 2 * a( a(n-i) ) + a(n-1) - a(n-2),
    where i ≥ 2 is the smallest integer satisfying a(n-i) < n.
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache : cache[k-1] holds a(k) for 1 ≤ k ≤ cache_size */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* ---------- initialize cache for a(1) and a(2) ---------- */
    if (cache_size == 0) {
        cache_size = 2;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failure */
        cache[0] = (seq_t)1;                 /* a(1) */
        cache[1] = (seq_t)2;                 /* a(2) */
    }

    /* ---------- return if already cached ---------- */
    if (n <= cache_size)
        return cache[n - 1];

    /* ---------- expand cache up to n ---------- */
    idx_t new_size = n;
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) return (seq_t)0;          /* allocation failure */
    cache = new_cache;

    for (idx_t k = cache_size + 1; k <= new_size; ++k) {
        /* a(k-1) and a(k-2) already present */
        seq_t a_k_1 = cache[k - 2];           /* a(k-1) */
        seq_t a_k_2 = cache[k - 3];           /* a(k-2) */

        /* find minimal i ≥ 2 with a(k-i) < k */
        idx_t i = 2;
        while (i < k) {
            seq_t a_prev = cache[k - i - 1];  /* a(k-i) */
            if (a_prev < (seq_t)k) break;
            ++i;
        }
        seq_t a_prev = cache[k - i - 1];       /* a(k-i) */

        /* compute a(a(k-i)) – its index is a_prev */
        seq_t a_a = cache[(idx_t)a_prev - 1];   /* a(a(k-i)) */

        /* compute a(k) */
        cache[k - 1] = (seq_t)2 * a_a + a_k_1 - a_k_2;
    }

    cache_size = new_size;
    return cache[n - 1];
}

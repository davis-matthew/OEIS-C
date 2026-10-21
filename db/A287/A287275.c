/*
 * A287275  Number of set partitions of [n] such that for each block all
 *          absolute differences between consecutive elements are <= three.
 * https://oeis.org/A287275
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 39, u128 n <= 77,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A287275 – partitions of [n] with block differences ≤ 3
   Recurrence: a(0)=1, a(1)=1, a(2)=2, a(3)=5
                a(n)=4*a(n-1) – 2*a(n-2) – 2*a(n-3) + a(n-4)   for n≥4
   (generating function (x³–3x+1)/((x‑1)(x³–x²–3x+1))). */

#include <stdbool.h>
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    static seq_t *cache  = NULL;
    static size_t cap    = 0;
    static idx_t  max_computed = 0;
    static bool   init  = false;

    /* First‑time initialization of the first four values. */
    if (!init) {
        ensure_cache(3, &cache, &cap);          /* need indices 0..3   */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)5;
        max_computed = 3;
        init = true;
    }

    /* Return already computed values immediately. */
    if (n <= max_computed) {
        return cache[n];
    }

    /* Extend cache up to n, if necessary. */
    ensure_cache(n, &cache, &cap);

    /* Compute missing values using the linear recurrence. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t v = (seq_t)4 * cache[i - 1];
        v -= (seq_t)2 * cache[i - 2];
        v -= (seq_t)2 * cache[i - 3];
        v += cache[i - 4];
        cache[i] = v;
    }

    max_computed = n;
    return cache[n];
}

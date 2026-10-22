/*
 * A301680  Coordination sequence for node of type V2 in "krn" 2-D tiling (or
 *          net).
 * https://oeis.org/A301680
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1 For n ≥ 1: a(n) = a(n-1) + (n≥7?a(n-7):0) -
 *            (n≥8?a(n-8):0) + c[n] where c[0..11] = { 1, 3, 5, 6, 5, 6, 6, 3,
 *            1, 1, 0, -1 } and c[n] = 0 for n ≥ 12. This recurrence follows
 *            from the rational generating function A(x) = -(x^11 - x^9 - x^8
 *            - 3x^7 - 6x^6 - 6x^5 -5x^4 -6x^3 -5x^2 -3x -1) / (x^...
 * Range:     u8 n <= 49, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;      /* sequence values up to current length */
static idx_t  cache_len = 0;     /* number of elements currently stored */
static idx_t  cache_cap = 0;     /* allocated capacity of cache */

static const int c[12] = { 1, 3, 5, 6, 5, 6, 6, 3, 1, 1, 0, -1 };

/* Ensure that cache can hold indices up to `needed`. */
static void ensure_capacity(idx_t needed)
{
    if (needed <= cache_cap)
        return;
    idx_t new_cap = cache_cap ? cache_cap : 64;
    while (new_cap <= needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < cache_len)
        return cache[n];

    ensure_capacity(n + 1);

    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t prev = (i >= 1 ? cache[i - 1] : (seq_t)0);
        seq_t a7   = (i >= 7 ? cache[i - 7] : (seq_t)0);
        seq_t a8   = (i >= 8 ? cache[i - 8] : (seq_t)0);

        /* compute a[i] = prev + a7 - a8 + c[i] (if i < 12) */
        seq_t val = prev + a7;
        val -= a8;                       /* guaranteed non‑negative */

        if (i < 12) {
            int ci = c[i];
            if (ci >= 0)
                val += (seq_t)ci;
            else
                val -= (seq_t)(-ci);      /* ci is -1 only at i == 11 */
        }

        cache[i] = val;
    }
    cache_len = n + 1;
    return cache[n];
}

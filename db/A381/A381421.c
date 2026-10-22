/*
 * A381421  a(n) = Sum_{k=0..n} (k+1) * binomial(2*k,2*n-2*k).
 * https://oeis.org/A381421
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∑_{k=0..n} (k+1) * binomial(2*k, 2*n-2*k) Recurrence:
 *            a(0)=1,a(1)=2,a(2)=5,a(3)=22,a(4)=68,a(5)=206,a(6)=631,a(7)=1870
 *            a(n)=4*a(n‑1)-2*a(n‑2)-11*a(n‑4)-2*a(n‑6)+4*a(n‑7)-a(n‑8) (n≥8)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 43, u128 n <= 88,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mul_small.h"

static seq_t *cache = NULL;
static idx_t cache_len = 0;

/* ensure cache has room for indices 0..n (inclusive) and
   compute any missing terms using the recurrence. */
static void ensure_cache(idx_t n)
{
    if (cache_len <= n) {
        /* grow cache */
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp) return;           /* allocation failed – leave old cache */
        cache = tmp;
        cache_len = n + 1;
    }

    /* Compute any missing terms from the current end up to n. */
    for (idx_t i = 0; i <= n; ++i) {
        if (i == 0)          cache[i] = 1;
        else if (i == 1)     cache[i] = 2;
        else if (i == 2)     cache[i] = 5;
        else if (i == 3)     cache[i] = 22;
        else if (i == 4)     cache[i] = 68;
        else if (i == 5)     cache[i] = 206;
        else if (i == 6)     cache[i] = 631;
        else if (i == 7)     cache[i] = 1870;
        else if (i >= 8) {
            /* use mul_small to avoid overflow when seq_t is a fixed-size unsigned type */
            seq_t t = mul_small(cache[i-1], 4);
            t -= mul_small(cache[i-2], 2);
            t -= mul_small(cache[i-4], 11);
            t -= mul_small(cache[i-6], 2);
            t += mul_small(cache[i-7], 4);
            t -= cache[i-8];
            cache[i] = t;
        }
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}

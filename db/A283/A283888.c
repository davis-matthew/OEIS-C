/*
 * A283888  Relative of Hofstadter Q-sequence: a(n) = max(0, n+27298) for n <=
 *          0; a(n) = a(n-a(n-1)) + a(n-a(n-2)) + a(n-a(n-3)) for n > 0.
 * https://oeis.org/A283888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 61, u32 n <= 8571, u64 n <= 8571,
 *            u128 n <= 8571, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A283888
   Recurrence: a(n) = a(n-a(n-1)) + a(n-a(n-2)) + a(n-a(n-3))  (n > 0)
   with base for non‑positive indices: a(k) = max(0, k+27298).
   Offset: 1  (a(1) is the first listed term)
*/

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Cached values: cache[0] = a(0), cache[1] = a(1), … */
static seq_t *cache = NULL;
static idx_t cache_len = 0;       /* allocated array length (>= n+1) */
static idx_t computed   = 0;      /* highest index already computed */

/* Ensure the cache can hold indices up to 'need' (inclusive). */
static void ensure_cache(idx_t need)
{
    if (cache_len > need) return;

    idx_t new_len = cache_len ? cache_len << 1 : 1;
    while (new_len <= need) new_len <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) abort();           /* fatal error if memory allocation fails */
    cache = tmp;
    cache_len = new_len;

    /* initialise the zero‑index value a(0) once */
    if (computed == 0 && cache_len > 0) cache[0] = (seq_t)27298;
}

/* Base value for non‑positive indices: a(k) = max(0, k+27298). */
static inline seq_t base_value(int64_t k)
{
    int64_t tmp = k + 27298;
    return (tmp > 0) ? (seq_t)tmp : (seq_t)0;
}

/* Main sequence function, memoised as required. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)27298;   /* safety for call with n==0 */

    if (computed < n) {
        ensure_cache(n);
        for (idx_t i = computed + 1; i <= n; ++i) {
            /* a(i-1) */
            seq_t a1 = cache[i - 1];

            /* a(i-2) */
            seq_t a2;
            if (i >= 2)          a2 = cache[i - 2];
            else                 a2 = base_value((int64_t)i - 2); /* i==1 */

            /* a(i-3) */
            seq_t a3;
            if (i >= 3)          a3 = cache[i - 3];
            else {
                if (i == 2) a3 = base_value((int64_t)i - 3); /* i==2 */
                else if (i == 1) a3 = base_value((int64_t)i - 3);
                else            a3 = base_value((int64_t)i - 3); /* never reached */
            }

            /* Compute the three indices: i - a(i-1), etc. */
            int64_t r1 = (int64_t)i - (int64_t)a1;
            int64_t r2 = (int64_t)i - (int64_t)a2;
            int64_t r3 = (int64_t)i - (int64_t)a3;

            seq_t v1 = (r1 <= 0) ? base_value(r1) : cache[(idx_t)r1];
            seq_t v2 = (r2 <= 0) ? base_value(r2) : cache[(idx_t)r2];
            seq_t v3 = (r3 <= 0) ? base_value(r3) : cache[(idx_t)r3];

            cache[i] = v1 + v2 + v3;
        }
        computed = n;
    }
    return cache[n];
}

/*
 * A351050  G.f. A(x) satisfies: A(x) = 1 + x + x^2 * A(x/(1 - 4*x)) / (1 -
 *          4*x).
 * https://oeis.org/A351050
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = a(1) = 1; for n >= 2, a(n) = Sum_{k=0..n-2}
 *            binomial(n-2,k) * 4^k * a(n-k-2)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute terms of OEIS A351050.  The function is memoised so that
 * repeated calls with any order of n are efficient.  It works for
 * any seq_t/idx_t combination provided by the build. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* Static cache for already computed values */
static seq_t *cache      = NULL;     /* pointer to the array of values   */
static size_t   cap      = 0;        /* allocated capacity (size_t)      */
static idx_t    computed = (idx_t)-1;/* largest n for which cache[n] is set */

/* Ensure the cache can hold at least (need+1) entries */
static void
ensure_capacity(idx_t need)
{
    if (need < cap) return;                     /* already big enough */
    size_t new_cap = cap ? cap : 8;             /* start with at least 8 */
    while (new_cap <= (size_t)need)             /* grow until big enough   */
        new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                          /* out‑of‑memory */
    cache = tmp;
    cap   = new_cap;
}

seq_t a(idx_t n)
{
    /* First time: initial allocation and base values */
    if (computed == (idx_t)-1) {
        ensure_capacity(1);                     /* need at least two slots */
        cache[0] = 1;
        cache[1] = 1;
        computed = 1;                           /* highest n computed so far */
    }

    /* If the requested n is not yet computed, extend the cache and fill the gap */
    if (n > computed) {
        /* Make sure there's room for n */
        ensure_capacity(n);

        /* Compute from the next index up to n inclusive */
        for (idx_t idx = computed + 1; idx <= n; ++idx) {
            if (idx < 2) {                      /* safety for idx_t underflow */
                cache[idx] = 1;
                continue;
            }
            idx_t m = idx - 2;                  /* n-2 in the recurrence   */
            seq_t coeff = 1;                    /* C(m,0)*4^0              */
            seq_t sum   = 0;                    /* accumulator for a[idx]  */

            /* Summation: k = 0 .. m        (note: k uses idx_t)          */
            for (idx_t k = 0; k <= m; ++k) {
                idx_t prev = idx - 2 - k;        /* n-k-2 (already computed) */
                sum += coeff * cache[prev];

                /* Update coeff for the next k (if any) */
                if (k < m) {
                    coeff = coeff * (m - k);     /* multiply by (m-k)   */
                    coeff = coeff / (k + 1);     /* divide by (k+1)    */
                    coeff = coeff * 4;           /* multiply by 4      */
                }
            }
            cache[idx] = sum;
        }
        computed = n;                           /* all values up to n computed */
    }

    return cache[n];
}

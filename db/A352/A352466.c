/*
 * A352466  a(0) = 1; a(n) = (1/n) * Sum_{k=1..n} binomial(2*n,2*k)^3 * k *
 *          a(n-k).
 * https://oeis.org/A352466
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A352466
 * a(0) = 1
 * a(n) = (1/n) * Σ_{k=1..n} binomial(2*n,2*k)^3 * k * a(n-k)
 * Offset: 0
 *
 * This implementation memoises the sequence.  It uses the provided
 * helper routine binomial(a,b).  The code works for any seq_t defined
 * by the build system (uint8/16/32/64 or arbitrary‑precision).
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/binomial.h"

static seq_t *a_cache = NULL;   /* memoisation array */
static size_t a_cache_sz = 0;   /* number of stored entries (a_cache[0..sz-1]) */

seq_t a(idx_t n)
{
    /* initialise cache with a(0) = 1 */
    if (a_cache_sz == 0) {
        a_cache = (seq_t *)malloc(sizeof(seq_t));
        if (!a_cache) return (seq_t)0;   /* allocation failure */
        a_cache[0] = (seq_t)1;
        a_cache_sz = 1;
    }

    /* extend cache up to n if necessary */
    if ((idx_t)n >= a_cache_sz) {
        size_t new_sz = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(a_cache, new_sz * sizeof(seq_t));
        if (!tmp) return (seq_t)0;       /* allocation failure */
        a_cache = tmp;

        /* compute missing terms */
        for (idx_t i = (idx_t)a_cache_sz; i <= n; ++i) {
            seq_t sum = (seq_t)0;
            for (idx_t k = 1; k <= i; ++k) {
                seq_t c = binomial(2 * i, 2 * k);     /* C(2i,2k) */
                seq_t c2 = c * c;                     /* C^2 */
                seq_t c3 = c2 * c;                    /* C^3 */
                seq_t term = c3;
                term *= (seq_t)k;
                term *= a_cache[i - k];
                sum += term;
            }
            a_cache[i] = sum / (seq_t)i;              /* divide by i */
        }
        a_cache_sz = new_sz;
    }

    return a_cache[(size_t)n];
}

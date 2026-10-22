/*
 * A371889  G.f. A(x) satisfies A(x) = 1 - x/A(x)^2 * (1 - A(x) - A(x)^3).
 * https://oeis.org/A371889
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (1/n) * Sum_{k=0..n} binomial(n,k) *
 *            binomial(n-3*k,n-k-1)
 * Range:     i8 n <= 6, i16 n <= 12, i32 n <= 22, i64 n <= 30, i128 n <= 30,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/binom.h"

static seq_t *cache = NULL;
static size_t cache_size = 0;   /* number of computed elements */
static size_t cache_capacity = 0; /* allocated size */

/* Ensure caching array can hold at least n+1 elements. */
static void ensure_cache_capacity(size_t n)
{
    if (n < cache_capacity)
        return;
    size_t new_cap = cache_capacity ? (cache_capacity << 1) : 1;
    while (new_cap <= n)
        new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        abort(); /* allocation failed */
    cache = new_cache;
    cache_capacity = new_cap;
}

/* Generalised binomial for negative upper indices:
   binom(-m,b) = (-1)^b * binom(m+b-1,b)  (m>0, b>=0).
*/
static seq_t binom_neg_top(int m, idx_t b)
{
    /* m = -top > 0, b >= 1 here (b=0 case handled earlier). */
    seq_t t = binom((idx_t)(m + b - 1), b);
    return (b & 1) ? -t : t;
}

/* Compute all a(k) up to and including n. */
static void compute_up_to(idx_t n)
{
    if ((size_t)n < cache_size)
        return;

    ensure_cache_capacity((size_t)n);

    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0) {
            cache[0] = 1;
            continue;
        }

        seq_t sum = 0;
        seq_t binom1 = 1; /* binomial(i,0) */

        for (idx_t k = 0; k <= i; ++k) {
            /* binomial(i,k) is already in binom1 */
            int top    = (int)i - 3 * (int)k;
            int bottom = (int)i - (int)k - 1;

            if (bottom < 0) {
                /* binomial(i,k) * 0 contributes nothing */
            } else {
                seq_t binom2;
                if (top >= 0) {
                    if (top < bottom) {
                        binom2 = 0;
                    } else {
                        binom2 = binom((idx_t)top, (idx_t)bottom);
                    }
                } else { /* top < 0 */
                    if (bottom == 0) {
                        binom2 = 1;
                    } else {
                        int m = -top;                /* m > 0 */
                        binom2 = binom_neg_top(m, bottom);
                    }
                }
                sum += binom1 * binom2;
            }

            /* Update binom1 to binomial(i,k+1) for next iteration. */
            if (k < i) {
                /* binom(i,k+1) = binom(i,k) * (i-k) / (k+1) */
                binom1 = binom1 * (seq_t)(i - k) / (seq_t)(k + 1);
            }
        }

        cache[i] = sum / (seq_t)i;
    }

    cache_size = (size_t)n + 1;
}

/* Public function required by the assignment. */
seq_t a(idx_t n)
{
    compute_up_to(n);
    return cache[(size_t)n];
}

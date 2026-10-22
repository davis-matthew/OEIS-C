/*
 * A395342  a(n) = (2*n)! * [x^(2*n)] cosh(x)^5 / cos(x)^2.
 * https://oeis.org/A395342
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n)! * coeff_{x^(2*n)} (cosh(x)^5 / cos(x)^2)
 *            Recurrence: a(0) = 1 a(n) = (10 + 5*9^n + 25^n)/16 – (1/2) *
 *            Σ_{k=0}^{n-1} (-4)^(n-k) * binomial(2*n,2*k) * a(k)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/binomial.h"   /* binomial(idx_t n, idx_t k) -> seq_t */

static seq_t *a_cache      = NULL;
static seq_t *pow9_cache   = NULL;
static seq_t *pow25_cache  = NULL;
static seq_t *pow4_cache   = NULL;
static idx_t  cache_size   = 0;  /* number of elements in the caches (next index to compute + 1) */

/* Ensure that caches contain entries for all indices up to n. */
static void ensure_cache(idx_t n)
{
    if (n < cache_size) return;

    size_t new_size = (size_t)n + 1;   /* indices 0..n inclusive */

    /* Reallocate all arrays */
    a_cache     = (seq_t *)realloc(a_cache,     new_size * sizeof(seq_t));
    pow9_cache  = (seq_t *)realloc(pow9_cache,  new_size * sizeof(seq_t));
    pow25_cache = (seq_t *)realloc(pow25_cache, new_size * sizeof(seq_t));
    pow4_cache  = (seq_t *)realloc(pow4_cache,  new_size * sizeof(seq_t));

    /* First-time initialization */
    if (cache_size == 0) {
        pow9_cache[0]  = (seq_t)1;
        pow25_cache[0] = (seq_t)1;
        pow4_cache[0]  = (seq_t)1;
        a_cache[0]     = (seq_t)1;   /* a(0) */
        cache_size     = 1;
    }

    /* Extend auxiliary power tables and compute remaining terms */
    for (idx_t i = cache_size; i <= n; ++i) {
        /* Powers of 9, 25 and 4 */
        pow9_cache[i]  = pow9_cache[i-1] * (seq_t)9;
        pow25_cache[i] = pow25_cache[i-1] * (seq_t)25;
        pow4_cache[i]  = pow4_cache[i-1] * (seq_t)4;

        /* Skip recurrence for i==0 (already set) */
        if (i == 0) continue;

        /* Constant term: (10 + 5*9^i + 25^i) / 16 */
        seq_t const_term = ((seq_t)10 
                            + (seq_t)5 * pow9_cache[i] 
                            + pow25_cache[i]) / (seq_t)16;

        /* Accumulate Σ (-4)^(i-k) * binomial(2*i, 2*k) * a(k) */
        seq_t pos = (seq_t)0;   /* sum of terms with sign +1 */
        seq_t neg = (seq_t)0;   /* sum of terms with sign -1 */

        seq_t C = (seq_t)1;     /* binomial(2*i, 0) */

        for (idx_t k = 0; k < i; ++k) {
            /* term = C * a(k) * 4^(i-k) */
            seq_t term = C * a_cache[k];
            term = term * pow4_cache[i - k];

            /* Sign determined by (-4)^(i-k) = (-1)^(i-k) * 4^(i-k) */
            if (((i - k) & (seq_t)1) == 0)
                pos += term;
            else
                neg += term;

            /* Update C to binomial(2*i, 2*(k+1)) */
            if (k + 1 < i) {
                seq_t num1 = (seq_t)(2 * i - 2 * k);
                seq_t num2 = (seq_t)(2 * i - 2 * k - 1);
                seq_t den1 = (seq_t)(2 * k + 2);
                seq_t den2 = (seq_t)(2 * k + 1);

                C = C * num1;
                C = C * num2;
                C = C / den1;
                C = C / den2;
            }
        }

        /* Compute a(i) = const_term – (S/2) where S = pos – neg */
        seq_t diff = (pos >= neg) ? (pos - neg) : (neg - pos);
        seq_t half = diff / (seq_t)2;
        seq_t a_val = (pos >= neg) ? (const_term - half) : (const_term + half);

        a_cache[i] = a_val;
    }

    cache_size = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return a_cache[n];
}

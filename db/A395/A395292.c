/*
 * A395292  a(n) = (2*n)! * [x^(2*n)] cos(x)^3 / cos(sqrt(2)*x).
 * https://oeis.org/A395292
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 6, i64 n <= 10, i128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A395292
   a(n) = (-1)^n*(3+9^n)/4 - sum_{k=0}^{n-1} (-2)^{n-k}*binomial(2n,2k)*a(k)
   a(0) = 1
*/

#include <stdlib.h>
#include <stdbool.h>

/* Forward declaration of the binomial function provided by the build */
seq_t binomial(idx_t n, idx_t k);

/* Static cache for computed values and auxiliary tables */
static seq_t *cache      = NULL;   /* the a(n) values */
static bool   *computed  = NULL;   /* whether cache[i] is filled */
static seq_t *pow_m2     = NULL;   /* (-2)^i */
static seq_t *pow_9      = NULL;   /* 9^i */
static idx_t  cache_size = 0;      /* allocated size of the caches */

/* Ensure that the cache and power tables hold at least up to n (inclusive). */
static void ensure_cache_size(idx_t n)
{
    if (n < cache_size) return;               /* nothing to do */

    idx_t old_size = cache_size;
    if (old_size == 0) old_size = 1;
    while (old_size <= n) old_size <<= 1;      /* double until big enough */

    /* reallocate arrays */
    cache     = (seq_t*)realloc(cache,     old_size * sizeof(seq_t));
    computed  = (bool*)realloc(computed,  old_size * sizeof(bool));
    pow_m2    = (seq_t*)realloc(pow_m2,    old_size * sizeof(seq_t));
    pow_9     = (seq_t*)realloc(pow_9,     old_size * sizeof(seq_t));

    /* initialise new entries */
    for (idx_t i = cache_size; i < old_size; ++i)
    {
        computed[i] = false;
        if (i == 0) {
            pow_m2[i] = (seq_t)1;   /* (-2)^0 */
            pow_9[i]  = (seq_t)1;   /* 9^0   */
        } else {
            pow_m2[i] = pow_m2[i-1] * ((seq_t)-2);
            pow_9[i]  = pow_9[i-1]  * ((seq_t)9);
        }
    }
    cache_size = old_size;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* make sure we have space for n */
    ensure_cache_size(n);

    /* compute all missing values up to n */
    for (idx_t i = 0; i <= n; ++i)
    {
        if (computed[i]) continue;          /* already known */

        if (i == 0) {
            cache[i] = 1;                     /* a(0) */
            computed[i] = true;
            continue;
        }

        /* compute sum_{k=0}^{i-1} (-2)^{i-k} *
           binomial(2*i,2*k) * a(k) */
        seq_t sum = 0;
        seq_t binom = 1;                     /* C(2*i, 0) */
        for (idx_t k = 0; k < i; ++k)
        {
            /* term = binom * a(k) * (-2)^{i-k} */
            seq_t term = binom;
            term *= cache[k];
            term *= pow_m2[i - k];
            sum += term;

            /* update binom to C(2*i, 2*(k+1)) for next iteration */
            if (k + 1 < i) {
                idx_t A = 2 * i - 2 * k;      /* (2i - 2k)   */
                idx_t B = A - 1;              /* (2i - 2k - 1) */
                idx_t C = 2 * (k + 1);        /* 2(k+1)      */
                idx_t D = C - 1;              /* 2(k+1) - 1  */

                binom = binom * (seq_t)A * (seq_t)B /
                        ((seq_t)C * (seq_t)D);
            }
        }

        /* compute the first part: (-1)^i * (3 + 9^i)/4 */
        seq_t first = (3 + pow_9[i]) / 4;
        if (i & 1) first = -first;            /* i odd */

        cache[i] = first - sum;
        computed[i] = true;
    }

    return cache[n];
}

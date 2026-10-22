/*
 * A367754  E.g.f. satisfies A(x) = exp(x * (1 + x) * A(x^2/2)).
 * https://oeis.org/A367754
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)! * Σ_{k=0}^{n-1} (k+1) * a(⌊k/2⌋) * a(n-1-k) /
 *            (2^⌊k/2⌋ * ⌊k/2⌋! * (n-1-k)!) Initial values: a(0) = 1, a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>


/* Cached values of the sequence */
static seq_t *cache = NULL;          /* array of stored a(i) */
static size_t cache_capacity = 0;    /* allocated size of 'cache' */
static idx_t  cache_len = 0;         /* number of values already computed */

/* Ensure that 'cache' can hold indices up to n */
static void
ensure_capacity(idx_t n)
{
    if ((idx_t)cache_capacity <= n) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc((void *)cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_capacity = new_cap;
    }
}

seq_t
a(idx_t n)
{
    /* Return pre‑computed value if available */
    if (cache && n < cache_len)
        return cache[n];

    /* Make sure the array is large enough */
    ensure_capacity(n);

    /* Compute missing values in ascending order */
    while ((idx_t)cache_len <= n) {
        if (cache_len == 0) {          /* a(0) */
            cache[0] = (seq_t)1;
            ++cache_len;
            continue;
        }

        idx_t   i   = cache_len;       /* index to compute */
        idx_t   m   = i - 1;           /* m = n-1 in the recurrence */
        seq_t   sum = (seq_t)0;

        seq_t ff              = (seq_t)1;   /* falling product (m)(m-1)… */
        idx_t  cur_f          = 0;          /* floor(k/2) */
        seq_t  cur_f_fact     = (seq_t)1;   /* f!   */
        seq_t  cur_f_pow2     = (seq_t)1;   /* 2^f  */

        for (idx_t k = 0; k <= m; ++k) {
            /* Denominator = f! * 2^f */
            seq_t denom = cur_f_fact * cur_f_pow2;

            /* product part – guaranteed divisible */
            seq_t temp  = ff / denom;

            seq_t term = (seq_t)(k + 1);
            term *= cache[cur_f];          /* a(⌊k/2⌋) */
            term *= cache[m - k];         /* a(m-k)   */
            term *= temp;

            sum += term;

            if (k == m)   /* last iteration – no further update needed */
                break;

            /* Prepare state for the next k = k+1 */
            ff *= (seq_t)(m - k);  /* multiply by the next factor of the falling product */

            idx_t next_f = (k + 1) >> 1;   /* floor((k+1)/2) */
            if (next_f > cur_f) {
                cur_f_fact *= (seq_t)(next_f);
                cur_f_pow2 *= (seq_t)2;
                cur_f = next_f;
            }
        }

        cache[cache_len] = sum;
        ++cache_len;
    }

    return cache[n];
}

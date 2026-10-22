/*
 * A328488  Expansion of e.g.f. 1 / (2 - exp(x * exp(x))).
 * https://oeis.org/A328488
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (OEIS notation):
   a(0) = 1
   a(n) = Sum_{k=1..n} binomial(n,k) * B(k) * a(n-k)          (n ≥ 1)
   where B(k) are the coefficients of the exponential generating
   function exp(x*exp(x)) (i.e. B(0)=1, B(1)=1, B(2)=3, B(3)=10, …).
   Recurrence for B(k):
   B(0) = 1
   B(n+1) = Sum_{i=0..n} binom(n,i) * (i+1) * B(n-i)           (n ≥ 0)
   These two recurrences together generate the sequence A328488. */

#include <stdlib.h>

/* Cache for computed values of a(n) and B(n) (coefficients of exp(x*exp(x))) */
static seq_t *a_cache   = NULL;   /* a[n] */
static seq_t *b_cache   = NULL;   /* B[n] */
static idx_t  cache_cap = 0;      /* size of the cache arrays (in elements) */
static idx_t  last_val  = 0;      /* highest n for which a[n] and B[n] are stored */

static void ensure_cache(idx_t n)
{
    if (cache_cap >= n + 1)
        return;   /* already large enough */

    /* Allocate new arrays of required capacity */
    seq_t *new_a = (seq_t*)calloc((size_t)(n + 1), sizeof(seq_t));
    seq_t *new_b = (seq_t*)calloc((size_t)(n + 1), sizeof(seq_t));

    /* If this is the first call, initialize a[0] and B[0] */
    if (a_cache == NULL) {
        new_a[0] = (seq_t)1;
        new_b[0] = (seq_t)1;
    } else {
        /* Copy existing cached values */
        for (idx_t i = 0; i <= last_val; ++i) {
            new_a[i] = a_cache[i];
            new_b[i] = b_cache[i];
        }
    }

    /* Free old arrays if they existed */
    free(a_cache);
    free(b_cache);

    a_cache   = new_a;
    b_cache   = new_b;
    cache_cap = n + 1;
}

/* Main function computing a(n) */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1;   /* a(0) = 1 */

    /* Make sure the cache is large enough */
    ensure_cache(n);

    /* Compute missing terms up to n */
    for (idx_t m = last_val + 1; m <= n; ++m) {
        /* ----------- Compute B(m) (coefficients of exp(x*exp(x))) ----------- */
        seq_t sum_b = (seq_t)0;
        seq_t bin_b = (seq_t)1;                 /* binom(m-1,0) = 1 */

        for (idx_t i = 0; i < m; ++i) {        /* i runs from 0 to m-1 */
            seq_t term = bin_b * (seq_t)(i + 1) * b_cache[m - 1 - i];
            sum_b += term;

            if (i + 1 < m) {                  /* update to binom(m-1,i+1) */
                bin_b = bin_b * (seq_t)(m - 1 - i) / (seq_t)(i + 1);
            }
        }
        b_cache[m] = sum_b;

        /* ----------- Compute a(m) ---------------------------------------- */
        seq_t sum_a = (seq_t)0;
        seq_t bin_a = (seq_t)m;                 /* binom(m,1) = m */

        for (idx_t k = 1; k <= m; ++k) {
            seq_t term = bin_a * a_cache[m - k] * b_cache[k];
            sum_a += term;

            if (k < m) {                       /* update to binom(m,k+1) */
                bin_a = bin_a * (seq_t)(m - k) / (seq_t)(k + 1);
            }
        }
        a_cache[m] = sum_a;
    }

    last_val = n;
    return a_cache[n];
}

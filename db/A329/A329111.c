/*
 * A329111  a(n) = round(b(n)) where b(n) = b(n-1)^(b(n-1)^(1/b(n-1))) and
 *          b(1)=2.
 * https://oeis.org/A329111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(b(n)) where b(1) = 2 b(n) = b(n-1) ^ ( b(n-1) ^ (1
 *            / b(n-1)) ) for n > 1 The implementation uses double‑precision
 *            arithmetic for the recursion of b(n) and rounds the result to
 *            produce a(n). The integer a(n) values are stored in a static
 *            cache so that the function works correctly for...
 * Range:     u8 n <= 15, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>

static seq_t *a_cache      = NULL; /* rounded values a(1..n) */
static double *b_cache     = NULL; /* double values b(1..n)   */
static size_t cache_cap    = 0;     /* current allocated size   */
static idx_t  cache_last   = 0;     /* highest n cached         */

/* Ensure the cache can hold values up to n (index 1 based). */
static void ensure_cache(idx_t n)
{
    if ((size_t)n < cache_cap) return;          /* already big enough */

    size_t new_cap = cache_cap > 0 ? cache_cap : 2;
    while ((size_t)n >= new_cap) new_cap <<= 1;  /* double until n fits */

    seq_t *a_new  = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!a_new) return;                         /* allocation failed */
    double *b_new = (double *)realloc(b_cache, new_cap * sizeof(double));
    if (!b_new) return;                         /* allocation failed */

    a_cache = a_new;
    b_cache = b_new;
    cache_cap = new_cap;
}

/* Compute a(n) as specified. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                       /* undefined, but safe */

    /* Initialise caches on first call. */
    if (cache_cap == 0) {
        ensure_cache(1);
        a_cache[1] = (seq_t)2;
        b_cache[1] = 2.0;
        cache_last = 1;
    }

    /* Grow cache if necessary. */
    ensure_cache(n);

    /* Compute missing terms. */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        double bi_minus1 = b_cache[i - 1];
        double exp_part  = exp(log(bi_minus1) / bi_minus1);   /* bi^(1/bi) */
        double bi        = pow(bi_minus1, exp_part);          /* bi+1 */
        b_cache[i] = bi;
        a_cache[i] = (seq_t)llround(bi);                     /* round to nearest int */
    }
    cache_last = n;
    return a_cache[n];
}

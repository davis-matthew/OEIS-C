/*
 * A385550  E.g.f. A(x) satisfies A(x) = Sum_{k>=0} x^k/k! * A(-k*x).
 * https://oeis.org/A385550
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 9, i64 n <= 13, i128 n <= 17,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Sum_{k=0..n-1} (k-n)^k * binomial(n,k) * a(k)
 * a(0) = 1
 *
 * Implements the OEIS recurrence using memoised values.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/big_pow.h"
#include "../utils/binomial.h"

/* Cached values of the sequence */
static seq_t *values   = NULL;   /* a[0..n] */
static unsigned char *known = NULL; /* 0 = uncomputed, 1 = computed */
static size_t cache_size   = 0;   /* number of elements allocated */

/* Grow the static arrays to be able to store a[0..n] */
static void ensure_cache(idx_t n)
{
    if ((idx_t)cache_size > n) return;   /* already enough */

    size_t new_size = (size_t)(n + 1);
    seq_t *tmpv = (seq_t *)realloc(values, new_size * sizeof(seq_t));
    unsigned char *tmpk = (unsigned char *)realloc(known, new_size);
    if (tmpv == NULL || tmpk == NULL) exit(1); /* unrecoverable */

    values = tmpv;
    known  = tmpk;

    /* Initialise newly allocated slots as unknown (0) */
    for (size_t i = cache_size; i < new_size; ++i) known[i] = 0;
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    /* Ensure storage for all values up to n */
    ensure_cache(n);

    if (known[n]) return values[n];               /* already computed */

    /* Compute all missing values up to n sequentially */
    for (idx_t i = 0; i <= n; ++i)
    {
        if (known[i]) continue;
        if (i == 0)
        {
            values[0] = (seq_t)1;
            known[0]  = 1;
            continue;
        }

        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k < i; ++k)
        {
            /* sign = (-1)^k for (k-i)^k = (-1)^k * (i-k)^k */
            seq_t sign = ((k & 1ULL) ? -(seq_t)1 : (seq_t)1);

            seq_t base   = (seq_t)(i - k);          /* i-k >= 1 here */
            seq_t power  = big_pow(base, (seq_t)k);
            seq_t bin    = binomial(i, k);
            seq_t term   = values[k];

            term *= bin;
            term *= power;
            if (sign < 0) term = -term;

            sum += term;
        }

        values[i] = sum;
        known[i]  = 1;
    }

    return values[n];
}

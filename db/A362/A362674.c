/*
 * A362674  E.g.f. satisfies A(x) = exp( x * exp(x^3) / A(x) ).
 * https://oeis.org/A362674
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n! * Sum_{k=0..⌊n/3⌋} (n-3k)^k * (-n+3k+1)^(n-3k-1) / (k!
 *            * (n-3k)!)
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 10, i64 n <= 17, i128 n <= 19,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/big_pow.h"

/* Cache of factorials up to the maximum n requested so far */
static seq_t *fac_cache = NULL;
static size_t fac_cap = 0;
static idx_t fac_max = 0;

static void ensure_factorial(idx_t n)
{
    if (n <= fac_max)
        return;

    size_t new_cap = fac_cap > 0 ? fac_cap : 1;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;

    /* Allocate or grow the cache */
    fac_cache = (seq_t *)realloc(fac_cache, new_cap * sizeof(seq_t));
    if (!fac_cache)
        exit(EXIT_FAILURE);           /* Out of memory */

    /* Initialize the first element if this is the first allocation */
    if (fac_cap == 0) {
        fac_cache[0] = (seq_t)1;
    }

    /* Compute factorials up to n */
    for (idx_t i = fac_max + 1; i <= n; ++i) {
        fac_cache[i] = fac_cache[i - 1] * (seq_t)i;
    }

    fac_cap = new_cap;
    fac_max = n;
}

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    ensure_factorial(n);

    seq_t fact_n = fac_cache[n];
    seq_t result = 0;

    for (idx_t k = 0; k <= n / 3; ++k) {
        idx_t m = n - 3 * k;          /* m = n - 3k */
        if (m == 0)                   /* term is zero for m == 0 and k > 0 */
            continue;

        /* Divide n! by k! * m! */
        seq_t denom = fac_cache[k] * fac_cache[m];
        seq_t term = fact_n / denom;  /* Exact division */

        /* (n-3k)^k */
        seq_t pow1 = big_pow((seq_t)m, (seq_t)k);
        term *= pow1;

        /* (-n+3k+1)^(n-3k-1) = (1 - m)^(m-1) */
        seq_t base = (seq_t)(-(seq_t)m + 1);
        seq_t expb = (seq_t)(m - 1);  /* m >= 1 here */
        seq_t pow2 = big_pow(base, expb);
        term *= pow2;

        result += term;
    }

    return result;
}

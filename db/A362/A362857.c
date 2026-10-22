/*
 * A362857  Expansion of e.g.f. exp(-2*x) / (1 + LambertW(-x)).
 * https://oeis.org/A362857
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} (-2)^(n-k) * k^k * binomial(n,k)
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 9, i64 n <= 16, i128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache of k^k values.  pow_cache[i] = i^i  (0^0 = 1).  */
static seq_t *pow_cache = NULL;
static idx_t pow_cap   = 0;

/* Integer exponentiation by squaring.  Works for any seq_t. */
static seq_t pow_seq_local(seq_t base, idx_t exp)
{
    seq_t result = 1;
    seq_t power  = base;
    while (exp)
    {
        if (exp & 1)
            result *= power;
        power *= power;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Grow the cache to contain 0^0 .. n^n. */
    if (n >= pow_cap)
    {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
        for (idx_t i = 0; i < new_cap; ++i)
        {
            if (i == 0)
                tmp[i] = 1;                     /* 0^0 = 1 */
            else
                tmp[i] = pow_seq_local((seq_t)i, i); /* i^i */
        }
        if (pow_cache)
            free(pow_cache);
        pow_cache = tmp;
        pow_cap   = new_cap;
    }

    seq_t binom  = 1;                           /* C(n,0)          */
    seq_t factor = pow_seq_local((seq_t)(-2), n);/* (-2)^n          */
    seq_t sum    = 0;

    for (idx_t k = 0; k <= n; ++k)
    {
        /* term = C(n,k) * k^k * (-2)^(n-k) */
        seq_t term = binom * pow_cache[k] * factor;
        sum += term;

        if (k == n)                /* last step, exit loop */
            break;

        /* Update C(n,k+1) = C(n,k) * (n-k) / (k+1) */
        binom *= (seq_t)(n - k);
        binom /= (seq_t)(k + 1);

        /* Update (-2)^(n-(k+1)) = (-2)^(n-k) / (-2) */
        factor /= (seq_t)(-2);
    }

    return sum;
}

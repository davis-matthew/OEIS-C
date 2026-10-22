/*
 * A372158  E.g.f. A(x) satisfies A(x) = exp( 2 * x * A(x)^(1/2) / (1 - x) ).
 * https://oeis.org/A372158
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * n! * Σ_{k=1..n} (k+2)^{k-1} * C(n-1, k-1) / k!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Tiny helper: exponentiation by squaring for arbitrary-precision type seq_t. */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* Cached powers (k+2)^(k-1) for k = 1..max_n */
static seq_t *pow_cache = NULL;
static idx_t pow_cap = 0;

static void ensure_pow_cache(idx_t need)
{
    if (need <= pow_cap)
        return;

    idx_t new_cap = need;
    seq_t *new_cache = (seq_t *)realloc(pow_cache, (new_cap + 1) * sizeof(seq_t));
    if (!new_cache)
        exit(1);        /* allocation failed */
    pow_cache = new_cache;

    /* Compute new powers */
    for (idx_t k = pow_cap + 1; k <= new_cap; ++k)
        pow_cache[k] = pow_seq((seq_t)(k + 2), (idx_t)(k - 1));

    pow_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;          /* a(0) = 1 */

    ensure_pow_cache(n);

    seq_t sum = 0;

    /* Start from k = n */
    seq_t curr_ratio  = 1;           /* n! / n! = 1 */
    seq_t curr_binom  = 1;           /* C(n-1, n-1) = 1 */
    seq_t pow_val = pow_cache[n];
    seq_t term = curr_ratio * curr_binom;
    term = term * pow_val;
    term = term * (seq_t)2;
    sum += term;

    for (idx_t k = n - 1; k; --k)
    {
        /* Update the ratio n! / k! by multiplying (k+1) */
        curr_ratio *= (seq_t)(k + 1);

        /* Update binomial C(n-1, k-1) via
           C(n-1, k-1) = C(n-1, k) * k / (n - k)              */
        curr_binom *= (seq_t)k;
        curr_binom /= (seq_t)(n - k);

        pow_val = pow_cache[k];
        term = curr_ratio * curr_binom;
        term = term * pow_val;
        term = term * (seq_t)2;
        sum += term;
    }

    return sum;
}

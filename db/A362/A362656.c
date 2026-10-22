/*
 * A362656  E.g.f. satisfies A(x) = exp( x * exp(x) * A(x)^3 ).
 * https://oeis.org/A362656
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} k^(n-k) * (3*k+1)^(k-1) * binomial(n,k)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"

/* Small power routine: base is seq_t, exponent is idx_t (non‑negative) */
static seq_t _pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp > 0)
    {
        if (exp & 1) result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t term = binomial(n, k);

        /* k^(n-k) */
        seq_t pow1;
        if (k == 0)
        {
            pow1 = (n == 0) ? 1 : 0;
        }
        else
        {
            pow1 = _pow_seq((seq_t)k, n - k);
        }
        term = term * pow1;

        /* (3*k+1)^(k-1) */
        seq_t pow2;
        if (k == 0)
        {
            pow2 = 1;  /* 1^(-1) is defined as 1 */
        }
        else
        {
            seq_t base = ((seq_t)3 * (seq_t)k) + 1;
            pow2 = _pow_seq(base, k - 1);
        }
        term = term * pow2;

        sum = sum + term;
    }
    return sum;
}

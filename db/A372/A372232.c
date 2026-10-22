/*
 * A372232  E.g.f. A(x) satisfies A(x) = exp( 2 * x * A(x)^(1/2) * (1 +
 *          A(x)^(1/2)) ).
 * https://oeis.org/A372232
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * Σ_{k=0..n} (n + k + 2)^(n-1) * binomial(n,k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Integer exponentiation for seq_t: base^exp with exp≥0. */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t res = (seq_t)1;
    while (exp)
    {
        if (exp & 1)
            res = res * base;
        exp >>= 1;
        if (exp)
            base = base * base;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)1;          /* a(0) = 1 */

    seq_t r = (seq_t)2;
    seq_t result = (seq_t)0;

    seq_t bin = (seq_t)1;           /* binomial(n,0) */

    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t base = (seq_t)(n + k + 2);
        seq_t pow = pow_seq(base, n - 1);

        result += bin * pow;

        if (k < n)
        {
            /* update bin = bin * (n - k) / (k + 1) */
            seq_t num = bin * (seq_t)(n - k);
            seq_t den = (seq_t)(k + 1);
            bin = num / den;
        }
    }

    result *= r;
    return result;
}

/*
 * A351572  Arithmetic derivative of the largest unitary divisor of n that is
 *          a square.
 * https://oeis.org/A351572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = arithmetic_derivative(largest_unitary_square_divisor(n))
 *            where largest_unitary_square_divisor(n) = ∏ p^e with e the
 *            exponent of prime p in n and e even. This implements the
 *            definition using trial division; n ≤ 2^64 and seq_t may be any
 *            size
 * Range:     u8 n <= 143, u16 n <= 9367, u32 n <= 9367, u64 n <= 9367,
 *            u128 n <= 9367, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required: define seq_t and idx_t by the build system. */
#include <stddef.h> /* for size_t */

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    idx_t m = n;               /* temporary copy of n for factorisation */
    seq_t d = 1;               /* largest unitary square divisor */

    /* Arrays for primes with even exponents and their exponents. */
    uint64_t prime_tbl[64];
    uint64_t exp_tbl[64];
    size_t cnt = 0;

    /* Trial division up to sqrt(m).  Use m/p to avoid overflow. */
    for (idx_t p = 2; p <= m / p; ++p)
    {
        if (m % p == 0)
        {
            uint64_t e = 0;
            while (m % p == 0)
            {
                m /= p;
                ++e;
            }

            if ((e & 1ULL) == 0)  /* even exponent – include in d */
            {
                /* multiply d by p^e */
                for (uint64_t i = 0; i < e; ++i)
                    d = d * (seq_t)p;

                prime_tbl[cnt] = p;
                exp_tbl[cnt]   = e;
                ++cnt;
            }
        }
    }

    /* Remaining prime factor with exponent 1 (odd) is ignored. */

    /* Compute arithmetic derivative: Σ e * (d / p). */
    seq_t derivative = 0;
    for (size_t i = 0; i < cnt; ++i)
    {
        seq_t dp = d / (seq_t)prime_tbl[i];
        derivative += (seq_t)exp_tbl[i] * dp;
    }

    return derivative;
}

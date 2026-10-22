/*
 * A383863  The number of divisors d of n having the property that for every
 *          prime p dividing n the p-adic valuation of d is either 0 or a
 *          unitary divisor of the p-adic valuation of n.
 * https://oeis.org/A383863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e‖n} (1 + 2^{ω(e)}), where ω(e) is the number of *
 *            distinct prime factors of the exponent e of p in n. This is
 *            equivalent * to the sequence definition: count the divisors d of
 *            n such that for * every prime p|n the p-adic valuation v_p(d) is
 *            either 0 or a unitary * divisor of v_p(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Count distinct prime factors of x (x ≥ 1). */
static unsigned count_distinct_prime_factors(idx_t x)
{
    unsigned cnt = 0;
    for (idx_t p = 2; p <= x / p; ++p)
    {
        if (x % p == 0)
        {
            ++cnt;
            while (x % p == 0)
                x /= p;
        }
    }
    if (x > 1)
        ++cnt;
    return cnt;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for n = 0; safe fallback */
    if (n == 1)
        return 1;          /* a(1) = 1 */

    seq_t result = 1;
    idx_t m = n;

    /* Factor n by trial division. */
    for (idx_t p = 2; p <= m / p; ++p)
    {
        if (m % p == 0)
        {
            unsigned e = 0;
            while (m % p == 0)
            {
                m /= p;
                ++e;
            }
            unsigned w = count_distinct_prime_factors((idx_t)e);
            seq_t factor = 1 + ((seq_t)1 << w);
            result *= factor;
        }
    }

    /* Remaining prime factor, if any.  Its exponent is 1. */
    if (m > 1)
    {
        /* For e = 1, ω(e) = 0 → 2^0 = 1, so factor = 1 + 1 = 2. */
        result *= (seq_t)2;
    }

    return result;
}

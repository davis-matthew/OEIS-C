/*
 * A309002  Multiplicative with a(p) = p^2 and a(p^e) = p^a(e) for any e > 1
 *          and prime number p.
 * https://oeis.org/A309002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) is multiplicative with * a(p) = p² * a(p^e) = p^(a(e)) for
 *            e > 1. * Thus for n = ∏pᵢ^eᵢ we have * a(n) = ∏pᵢ^( (eᵢ==1) ? 2
 *            : a(eᵢ) )
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 63, u64 n <= 255, u128 n <= 255,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t pow_uint(seq_t base, uint64_t exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1) result *= base;
        exp >>= 1;
        if (exp) base *= base;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 1) return 1;

    seq_t result = 1;
    uint64_t m = (uint64_t)n;
    for (uint64_t p = 2; p * p <= m; ++p)
    {
        if (m % p == 0)
        {
            uint64_t e = 0;
            while (m % p == 0)
            {
                m /= p;
                ++e;
            }
            seq_t exp = (e == 1) ? 2 : a((idx_t)e);
            seq_t pow = pow_uint((seq_t)p, (uint64_t)exp);
            result *= pow;
        }
    }
    if (m > 1)   /* remaining prime factor with exponent 1 */
    {
        seq_t pow = pow_uint((seq_t)m, 2);
        result *= pow;
    }
    return result;
}

/*
 * A372331  The number of infinitary divisors of the smallest number k such
 *          that k*n is a number whose number of divisors is a power of 2
 *          (A036537).
 * https://oeis.org/A372331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = 2^{ Σ over prime powers p^e || n of ( number of zero bits
 *            in the binary expansion of e ) }
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*                                      = Π over prime powers p^e || n  of 2^{ zeros(e) }    */
/*                                      = Π over prime powers p^e || n  of 2^{ bitlen(e) - popcount(e) }    */

#include <stdint.h>

static inline idx_t bitlen_idx(idx_t x)
{
    /* number of bits needed to represent x (x > 0) */
    return x == 0 ? 0 : (idx_t)64 - (idx_t)__builtin_clzll((unsigned long long)x);
}

/* multiplicative, small n factorization by trial division */
seq_t a(idx_t n)
{
    idx_t totalZeros = 0;
    idx_t tmp = n;

    /* factor 2 separately for speed */
    if ((tmp & 1ULL) == 0ULL)
    {
        idx_t exp = 0;
        while ((tmp & 1ULL) == 0ULL)
        {
            tmp >>= 1;
            ++exp;
        }
        totalZeros += bitlen_idx(exp) - (idx_t)__builtin_popcountll((unsigned long long)exp);
    }

    /* odd factors */
    for (idx_t p = 3; p * p <= tmp; p += 2)
    {
        if (tmp % p == 0)
        {
            idx_t exp = 0;
            while (tmp % p == 0)
            {
                tmp /= p;
                ++exp;
            }
            totalZeros += bitlen_idx(exp) - (idx_t)__builtin_popcountll((unsigned long long)exp);
        }
    }

    /* remaining prime factor, exponent = 1 contributes 0 zeros */
    /* (no action needed) */

    /* compute 2^{totalZeros} safely */
    seq_t result = 1;
    for (idx_t i = 0; i < totalZeros; ++i)
        result *= 2;   /* multiplication is modulo the width of seq_t */

    return result;
}

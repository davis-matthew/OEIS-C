/*
 * A372332  The number of "Fermi-Dirac primes" (A050376) that are
 *          noninfinitary divisors of n.
 * https://oeis.org/A372332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{p^e || n} (bitlen(e) - popcount(e)), where bitlen(e)
 *            is the number of bits in the binary representation of e (i.e.,
 *            floor(log2(e))+1) and popcount(e) is the number of 1‑bits
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is equivalent to the OEIS “additive” definition
   with a(p^e) = A023416(e).  A023416(e) counts the number of zeros
   in the binary expansion of e. */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;                     /* a(1) = 0 */

    uint64_t m = (uint64_t)n;
    seq_t total = 0;

    for (uint64_t p = 2; p * p <= m; ++p)
    {
        if (m % p == 0)
        {
            unsigned int exp = 0;
            while (m % p == 0)
            {
                m /= p;
                ++exp;
            }

            /* bitlen(e)  = number of bits needed for exp */
            unsigned int bitlen = 0;
            uint64_t t = exp;
            while (t)
            {
                ++bitlen;
                t >>= 1;
            }

            /* popcount(e) = number of 1‑bits in exp  */
            unsigned int pop = __builtin_popcountll(exp);

            total += (seq_t)(bitlen - pop);
        }
    }

    /* remaining prime factor (if >1) has exponent 1:
       bitlen(1)=1, pop(1)=1, so contributes 0 */
    return total;
}

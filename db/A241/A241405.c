/*
 * A241405  Sum of modified exponential divisors: if n = Product p_i^r_i then
 *          me-sigma(x) = Product (sum p_i^s_i such that s_i+1 divides r_i+1).
 * https://oeis.org/A241405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = sum of modified exponential divisors of n Using the
 *            multiplicative definition: for prime powers p^a, a(p^a) =
 *            Σ_{d|a+1} p^{d-1}
 * Range:     u8 n <= 119, u16 n <= 9901, u32 n <= 9901, u64 n <= 9901,
 *            u128 n <= 9901, big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation factorises n by trial division and
   computes the product of the sums for each prime factor. */

#include <stdint.h>          /* for uint64_t */
#include "../utils/pow_uint.h"   /* seq_t pow_uint(uint64_t base, idx_t exp) */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* defined only for n >= 1 in OEIS */
    uint64_t tmp = (uint64_t)n;
    seq_t result = 1;              /* multiplicative identity */

    /* factorise tmp by trial division */
    for (uint64_t p = 2; p <= tmp / p; ++p)
    {
        if (tmp % p == 0)
        {
            unsigned int exp = 0;
            while (tmp % p == 0)
            {
                tmp /= p;
                ++exp;
            }

            /* compute Σ_{d|exp+1} p^{d-1} */
            seq_t sum_term = 0;
            unsigned int rplus = exp + 1;
            for (unsigned int d = 1; d <= rplus; ++d)
            {
                if (rplus % d == 0)
                {
                    unsigned int s = d - 1;           /* exponent */
                    sum_term += pow_uint(p, s);
                }
            }
            result *= sum_term;
        }
    }

    /* if a prime > sqrt(original n) remains */
    if (tmp > 1)
    {
        uint64_t p = tmp;          /* remaining prime factor */
        unsigned int exp = 1;

        seq_t sum_term = 0;
        unsigned int rplus = exp + 1;   /* rplus = 2 */
        for (unsigned int d = 1; d <= rplus; ++d)
        {
            if (rplus % d == 0)
            {
                unsigned int s = d - 1;
                sum_term += pow_uint(p, s);
            }
        }
        result *= sum_term;
    }

    return result;
}

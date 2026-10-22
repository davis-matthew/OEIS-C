/*
 * A340676  If n is of the form s^(2^e), where s is a squarefree number, and e
 *          >= 0, then a(n) = 1+e, otherwise a(n) = 0.
 * https://oeis.org/A340676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1+e if n = s^(2^e) with s squarefree, otherwise 0
 * Range:     u8 n <= 9629, u16 n <= 9629, u32 n <= 9629, u64 n <= 9629,
 *            u128 n <= 9629, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where e >= 0 and s > 1. The special case n = 1 returns 0. */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n <= 1) return 0;   /* 1 is not of the required form */

    uint64_t val = (uint64_t)n;
    int common_exp = -1;          /* exponent in prime factorisation shared by all primes */

    /* Trial division */
    for (uint64_t p = 2; p * p <= val; ++p)
    {
        if (val % p == 0)
        {
            int cnt = 0;
            while (val % p == 0)
            {
                val /= p;
                ++cnt;
            }
            if (common_exp == -1)
                common_exp = cnt;
            else if (common_exp != cnt)
                return 0;      /* exponents differ, cannot be a power of a squarefree base */
        }
    }

    /* Remaining prime factor (if any) has exponent 1 */
    if (val > 1)
    {
        if (common_exp == -1)
            common_exp = 1;
        else if (common_exp != 1)
            return 0;
    }

    /* common_exp must be a power of two */
    if ((common_exp & (common_exp - 1)) != 0)
        return 0;

    /* Compute e = log2(common_exp) */
    int e = 0;
    uint64_t tmp = (uint64_t)common_exp;
    while (tmp > 1)
    {
        tmp >>= 1;
        ++e;
    }

    return (seq_t)(e + 1);
}

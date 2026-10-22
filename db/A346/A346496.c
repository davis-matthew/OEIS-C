/*
 * A346496  Smallest a(n) so that division by n can be performed by floor(x/n)
 *          = floor(x*A346495(n)/2^a(n)) for any 0 <= x < 2^32.
 * https://oeis.org/A346496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = if n is a power of two, a(n)=log₂(n). otherwise, a(n) is
 *            the smallest integer b such that 2^b > (2²⁰¹⁰⁴ ÷ mod n ... 2^32
 *            - (2^32 mod n) - 1) * (n-1-((2^b-1) mod n))
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of the OEIS sequence A346496.  */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Power of two case: a(n) = log₂(n).  */
    if ((n & (n - 1)) == 0)            /* n ≥ 1 */
    {
        unsigned shift = 0;
        idx_t t = n;
        while (t > 1)
        {
            t >>= 1;
            ++shift;
        }
        return (seq_t)shift;
    }

    /* Non‑power of two case.  */
    const uint64_t two32 = (uint64_t)1 << 32;          /* 2³² */
    uint64_t n_c = two32 - (two32 % n) - 1;           /* n_c = 2³² − (2³² mod n) − 1 */

    for (uint64_t b = 0; ; ++b)
    {
        uint64_t powb = (uint64_t)1 << b;              /* 2ᵇ   */
        uint64_t mod  = (powb - 1) % n;                /* (2ᵇ−1) mod n */
        uint64_t rhs  = n_c * (n - 1 - mod);           /* n_c · (n−1−mod) */

        if (powb > rhs)                               /* minimal b found */
            return (seq_t)b;
    }
}

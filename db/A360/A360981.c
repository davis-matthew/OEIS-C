/*
 * A360981  a(n) is the least positive multiple of n that is an evil number
 *          (A001969).
 * https://oeis.org/A360981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = least multiple m of n for which popcount(m) is even (an
 *            evil number)
 * Range:     u8 n <= 30, u16 n <= 510, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;      /* cast index to seq_t type */
    seq_t k  = 1;             /* current multiplier */

    for (;;)
    {
        seq_t prod = nn * k;  /* candidate multiple */

        /* compute parity of popcount(prod) */
        seq_t t = prod;
        unsigned parity = 0;
        while (t)
        {
            parity ^= (unsigned)(t & 1);
            t >>= 1;
        }

        if (parity == 0)        /* even number of 1-bits */
            return prod;        /* this is the least evil multiple */

        ++k;                    /* try next multiple */
    }
}

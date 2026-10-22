/*
 * A391576  Numbers k such that A048720(3,k) = 3*m for some m, where A048720
 *          is carryless base-2 multiplication.
 * https://oeis.org/A391576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A391576: numbers k such that A048720(3,k) ≡ 0 (mod 3) */
/* Equivalently, (k XOR (k<<1)) % 3 == 0. */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;             /* undefined for n < 1 */

    idx_t cnt = 0;             /* how many valid k's we have found */
    seq_t k   = 1;             /* candidate k */

    for (;; ++k)                /* iterate over k in order */
    {
        seq_t prod = k ^ (k << 1);   /* carry-less product of 3 and k */
        if ((prod % 3) == 0)        /* check divisibility by 3 */
        {
            ++cnt;
            if (cnt == n)           /* the nth valid k is found */
                return k;
        }
    }
}

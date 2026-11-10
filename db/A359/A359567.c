/*
 * A359567  Numbers k such that A102370(k) = k + 2.
 * https://oeis.org/A359567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th k with A102370(k) = k+2, i.e. k odd and k+j not
 *            divisible by 2^j for any odd j >= 3 (found by search)
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int A359567_ok(uint64_t k)
{
    if ((k & 1) == 0) return 0;               /* j=1 needs k+1 even */
    for (unsigned j = 3; j < 64; j += 2)      /* even j: k+j odd, never divisible */
    {
        unsigned __int128 s = (unsigned __int128)k + j;
        if ((s & (((unsigned __int128)1 << j) - 1)) == 0) return 0;
    }
    return 1;
}

seq_t a(idx_t n)
{
    uint64_t cnt = 0;
    uint64_t k = 1;
    for (;; k += 2)
    {
        if (A359567_ok(k))
        {
            if (++cnt == (uint64_t)n) return (seq_t)k;
        }
    }
}

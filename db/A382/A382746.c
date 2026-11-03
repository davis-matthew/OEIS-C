/*
 * A382746  If k appears, 8*k does not.
 * https://oeis.org/A382746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) are all positive integers 2^m * r, where r is odd and m ≡
 *            0,1,2 (mod 6)
 * Range:     u8 n <= 227, u16 n <= 8889, u32 n <= 8889, u64 n <= 8889,
 *            u128 n <= 8889, big any n
 * Validated: standard, pass, 87 terms through n = 8871 (2026-11-01)
 * Helpers:   ctz_u64_to_u32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* That is, include n iff the exponent of 2 in its factorisation is 0,1,2 modulo 6. */

#include <stdint.h>
#include "../utils/ctz_u64_to_u32.h"

seq_t a(idx_t n)
{
    /* offset 1: a(1) is the first term. */
    if (n == (idx_t)0)  /* defensive; not needed for the test cases */
        return (seq_t)0;

    idx_t target = n;
    idx_t cnt    = 0;
    uint64_t candidate = 1;

    while (cnt < target)
    {
        if (ctz_u64_to_u32(candidate) % 6 < 3)
        {
            ++cnt;
            if (cnt == target)
                return (seq_t)candidate;
        }
        ++candidate;
    }
    /* unreachable, keep compiler happy */
    return (seq_t)0;
}

/*
 * A244747  Positions at which powers of 2 occur in A051145.
 * https://oeis.org/A244747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: iterate A051145 via b(m) = ((b(m-1) OR b(m-2)) + 1) AND
 *            NOT b(m-1), b(0)=0, b(1)=1, and return the m at which b(m) is a
 *            power of 2 for the n-th time (A209229(b(m)) = 1)
 * Range:     u8 n <= 16, u16 n <= 36, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    unsigned __int128 prev2 = 0;   /* A051145(m-1) */
    unsigned __int128 prev1 = 1;   /* A051145(m)   */
    uint64_t m = 1;
    idx_t count = 1;               /* A051145(1) = 1 is a power of 2 */

    while (count < n)
    {
        unsigned __int128 cur = ((prev1 | prev2) + 1) & ~prev1;
        prev2 = prev1;
        prev1 = cur;
        ++m;
        if (cur != 0 && (cur & (cur - 1)) == 0)
            ++count;
    }
    return (seq_t)m;
}

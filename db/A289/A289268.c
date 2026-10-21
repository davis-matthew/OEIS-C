/*
 * A289268  Decimal representation of the diagonal from the corner to the
 *          origin of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 553", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A289268
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n+1) - 2 if n is odd 2^(n+1) - (2^p - 1) if n is even
 *            where p = 1 << floor_log2_uint64(n/2) (p = 1 when n = 0)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   floor_log2_uint64, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    seq_t top = pow2(n + 1);          // 2^(n+1)
    if (n & (idx_t)1)                // odd n
    {
        return top - (seq_t)2;
    }
    else                               // even n
    {
        idx_t m = n >> 1;             // n/2
        idx_t p;
        if (m == 0)
        {
            p = 1;
        }
        else
        {
            p = ((idx_t)1) << floor_log2_uint64(m);
        }
        seq_t sub = pow2(p) - (seq_t)1;
        return top - sub;
    }
}

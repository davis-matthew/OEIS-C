/*
 * A287188  Decimal representation of the diagonal from the corner to the
 *          origin of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 251", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A287188
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n+1) - 1 if n is odd, a(n) = 2^( 2^( floor(log2(n+2))
 *            - 1 ) ) - 1 if n is even
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 126,
 *            big any n
 * Helpers:   floor_log2_uint64, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    if (n & 1) /* n odd */
    {
        return pow2(n + 1) - 1;
    }
    /* n even */
    idx_t m = floor_log2_uint64(n + 2);      /* floor(log2(n+2)) */
    idx_t e = ((idx_t)1) << (m - 1);         /* e = 2^(m-1) */
    return pow2(e) - 1;
}

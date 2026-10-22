/*
 * A324271  a(n) = 181*13^(13*n).
 * https://oeis.org/A324271
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 181*13^(13*n). a(0)=181, a(n)=302875106592253*a(n-1)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 0, u64 n <= 1, u128 n <= 2,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t etc. used by seq_t and idx_t */

/* seq_t and idx_t are defined by the build environment. */

seq_t a(idx_t n)
{
    /* Base value 181 */
    seq_t result = (seq_t)181;
    /* Constant C = 13^13 = 302875106592253 */
    const seq_t C = (seq_t)302875106592253ULL;

    /* Multiply C n times */
    for (idx_t i = 0; i < n; ++i)
        result *= C;

    return result;
}

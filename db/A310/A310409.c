/*
 * A310409  Coordination sequence Gal.3.19.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310409
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*n + floor(4*n/5) + [n mod 5 == 2 or 4] (a(0)=1)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;
    /* convert index to seq_t only once for subsequent operations */
    seq_t sq = (seq_t)n;
    seq_t four_n = (seq_t)4 * sq;          /* 4*n   */
    seq_t extra   = four_n / (seq_t)5;      /* floor(4n/5) */
    seq_t result  = four_n + extra;         /* 4n + floor(4n/5) */

    /* add 1 when n % 5 equals 2 or 4 */
    if ((n % (idx_t)5) == (idx_t)2 || (n % (idx_t)5) == (idx_t)4)
        result += (seq_t)1;

    return result;
}

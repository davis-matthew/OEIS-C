/*
 * A310531  Coordination sequence Gal.5.135.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310531
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*n - 2*k where k = 2*floor((n-1)/9)+1 when (n-1)%9 < 8 k
 *            = 2*floor((n-1)/9)+2 when (n-1)%9 == 8 (and a(0) = 1). This
 *            generates the OEIS sequence A310531
 * Range:     u8 n <= 46, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    idx_t m = n - 1;          /* n-1 */
    idx_t t = m / 9;          /* floor((n-1)/9) */
    idx_t r = m % 9;          /* (n-1) % 9 */

    /* compute k as 2*t + 1, plus one more if r==8 */
    seq_t k = (seq_t)(2 * t + 1 + (r == 8 ? 1 : 0));

    /* a(n) = 6*n - 2*k */
    seq_t nseq = (seq_t)n;
    return 6 * nseq - 2 * k;
}

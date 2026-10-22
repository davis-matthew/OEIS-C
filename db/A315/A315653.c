/*
 * A315653  Coordination sequence Gal.5.305.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315653
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=6, and for n>=2, a(n) = 6*n - 2*cnt, where cnt is
 *            the number of integers i in [2,n] with i mod 9 equal to 3 or 7.
 *            (i.e. i≡3 or 7 (mod 9))
 * Range:     u8 n <= 45, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;

    idx_t c3 = (n >= 3)    ? ((n - 3) / 9 + 1) : 0;
    idx_t c7 = (n >= 7)    ? ((n - 7) / 9 + 1) : 0;
    idx_t c4 = c3 + c7;           /* number of occurrences of 4‑steps */

    seq_t res = (seq_t)6 * (seq_t)n - (seq_t)2 * (seq_t)c4;
    return res;
}

/*
 * A315651  Coordination sequence Gal.5.303.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315651
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*n - 2 * (2*(n/8) + (n%8>=3) + (n%8>=6)) for n>=1, a(0)
 *            = 1 (derived from that d(n)=6 except when n%8==3 or 6, where
 *            d(n)=4)
 * Range:     u8 n <= 46, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

extern seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    seq_t nn = (seq_t)n;          /* convert index to seq_t */
    seq_t full_blocks = nn / (seq_t)8;
    seq_t remainder   = nn % (seq_t)8;
    seq_t special = full_blocks * (seq_t)2;
    if (remainder >= (seq_t)3) special += (seq_t)1;
    if (remainder >= (seq_t)6) special += (seq_t)1;
    return (seq_t)6 * nn - (seq_t)2 * special;
}

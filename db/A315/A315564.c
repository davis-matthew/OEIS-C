/*
 * A315564  Coordination sequence Gal.6.665.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315564
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 For n > 0, let m = floor(n/6) and r = n mod 6. Then
 *            a(n) = 6*n – d, where d = 0 if r == 0 d = 2*m if r == 1 d = m +
 *            1 if r == 2 d = 0 if r == 3 d = m if r == 4 d = 2*(m+1)if r == 5
 * Range:     u8 n <= 43, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    idx_t m = n / 6;
    idx_t r = n % 6;

    seq_t d = 0;
    switch (r) {
    case 0: d = 0; break;
    case 1: d = (seq_t)(2 * m); break;
    case 2: d = (seq_t)(m + 1); break;
    case 3: d = 0; break;
    case 4: d = (seq_t)m; break;
    case 5: d = (seq_t)(2 * (m + 1)); break;
    }

    seq_t six_n = (seq_t)n * 6;
    return six_n - d;
}

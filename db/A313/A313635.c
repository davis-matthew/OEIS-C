/*
 * A313635  Coordination sequence Gal.4.54.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313635
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n ≥ 1: a(n) = 5*n + f(n) where f(n) = { -1 if n
 *            mod 8 = 3, +1 if n mod 8 = 5, 0 otherwise } (for even n, f(n) =
 *            0 which fits the same rule)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    /* base value 5*n, use seq_t on the multiplication to avoid overflow */
    seq_t val = (seq_t)5 * n;

    /* only odd n have a possible adjustment */
    if (n & 1ULL) {
        switch (n & 7ULL) {          /* n % 8 */
            case 3:
                val -= (seq_t)1;     /* subtract 1 */
                break;
            case 5:
                val += (seq_t)1;     /* add 1 */
                break;
            default:
                /* no change for other odd residues (1 or 7) */
                break;
        }
    }
    return val;
}

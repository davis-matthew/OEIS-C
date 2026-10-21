/*
 * A299256  Coordination sequence for 3D uniform tiling formed by stacking
 *          parallel layers of the 3.6.3.6 2D tiling (cf. A008579).
 * https://oeis.org/A299256
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 119, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(0)=1, a(1)=6;
 *        a(n) = 9*n^2/2          for even n>1
 *        a(n) = (9*n^2-1)/2      for odd  n>1
 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;

    seq_t nn = (seq_t)n;
    seq_t val = (seq_t)9 * nn * nn;   /* 9*n^2   */

    if ((n & 1) == 0) {
        return val / 2;               /* even n > 1 */
    } else {
        return (val - 1) / 2;          /* odd  n > 1 */
    }
}

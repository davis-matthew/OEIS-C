/*
 * A272103  Convolution of nonzero heptagonal numbers (A000566) with
 *          themselves.
 * https://oeis.org/A272103
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n + 1)*(n + 2)*(n + 3)*(5*n^2 + 5*n + 4)/24
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic to avoid overflow while computing the closed formula */
    unsigned __int128 nn = (unsigned __int128)n;
    unsigned __int128 res = (nn + 1) * (nn + 2) * (nn + 3) *
                            (5 * nn * nn + 5 * nn + 4) / 24;
    return (seq_t)res;
}

/*
 * A366817  Detour index of n body-centered cubic grid unit cells in a row.
 * https://oeis.org/A366817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (25*n^3 + 180*n^2 - 13*n) / 3
 * Range:     u8 n <= 1, u16 n <= 17, u32 n <= 799, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = n;                     /* copy index to seq_t */
    seq_t n_sq = nn * nn;             /* n^2 */
    seq_t v = 25 * n_sq + 180 * nn - 13;   /* 25*n^2 + 180*n - 13 */

    /* The numerator is guaranteed divisible by 3.
       Divide one of the two factors by 3 to avoid overflow. */
    seq_t res;
    if (nn % 3 == 0) {
        res = (nn / 3) * v;
    } else {                         /* v must be divisible by 3 */
        res = nn * (v / 3);
    }
    return res;
}

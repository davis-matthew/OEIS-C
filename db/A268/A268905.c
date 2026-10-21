/*
 * A268905  Number of 2 X n 0..2 arrays with some element plus some
 *          horizontally or antidiagonally adjacent neighbor totalling two
 *          exactly once.
 * https://oeis.org/A268905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*3^(n-3)*(8*n-3) for n >= 3 a(1) = 0 a(2) = 36 Uses
 *            iterative multiplication to compute the power of 3
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 36, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) {
        return 0;
    }
    if (n == 2) {
        return 36;
    }

    /* compute 3^(n-3) */
    seq_t pow3 = 1;
    idx_t exp = n - 3;
    for (idx_t i = 0; i < exp; ++i) {
        pow3 *= 3;
    }

    /* compute 8 * pow3 * (8*n - 3) */
    seq_t res = pow3 * 8;
    seq_t term = ((seq_t)8 * n) - 3;
    res *= term;

    return res;
}

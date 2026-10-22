/*
 * A352178  Let S = {t_1, t_2, ..., t_n} be a set of n distinct integers and
 *          consider the sums t_i + t_j (i<j); a(n) is the maximum number of
 *          such sums that are powers of 2, over all choices for S.
 * https://oeis.org/A352178
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 21, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (5n - (n mod 2))/2 - 11   for n >= 12
 * a(1) = 0, a(2) = 1, a(3) = 3, a(4) = 4, a(5) = 6, a(6) = 7,
 * a(7) = 9, a(8) = 11, a(9) = 13, a(10) = 15, a(11) = 17
 */
seq_t a(idx_t n)
{
    /* Small n: use explicit table */
    if (n < 12) {
        static const seq_t t[12] = {
            0,  /* t[0] unused */
            0,  /* a(1) */
            1,  /* a(2) */
            3,  /* a(3) */
            4,  /* a(4) */
            6,  /* a(5) */
            7,  /* a(6) */
            9,  /* a(7) */
            11, /* a(8) */
            13, /* a(9) */
            15, /* a(10) */
            17  /* a(11) */
        };
        return t[n];
    }

    /* n >= 12 */
    seq_t nn = (seq_t)n;               /* convert index to seq_t */
    seq_t res = (5 * nn - (nn & 1)) / 2 - 11;
    return res;
}

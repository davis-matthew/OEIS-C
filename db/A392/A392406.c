/*
 * A392406  a(n) is the maximum possible number of extensions of a sequence of
 *          any n integers, where an extension is the runs-resistance of the
 *          sequence including all previous extensions.
 * https://oeis.org/A392406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 3*n - 18 for n >= 20; precomputed values for 1 <= n < 20
 * Range:     u8 n <= 35, u16 n <= 35, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)0;
    if (n < 20u) {
        static const seq_t pre[20] = {
            0, /* dummy for index 0 */
            6, /* a(1) */
            5, /* a(2) */
            5, /* a(3) */
            7, /* a(4) */
            9, /* a(5) */
            13, /* a(6) */
            15, /* a(7) */
            17, /* a(8) */
            19, /* a(9) */
            21, /* a(10) */
            23, /* a(11) */
            25, /* a(12) */
            27, /* a(13) */
            29, /* a(14) */
            31, /* a(15) */
            33, /* a(16) */
            35, /* a(17) */
            37, /* a(18) */
            39  /* a(19) */
        };
        return pre[n];
    }
    seq_t n_s = (seq_t)n;
    return n_s * (seq_t)3 - (seq_t)18;
}

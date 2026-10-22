/*
 * A373929  Number of compositions of 7*n-3 into parts 1 and 7.
 * https://oeis.org/A373929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 21*a(n-2) + 35*a(n-3) - 35*a(n-4) + 21*a(n-5)
 *            - 7*a(n-6) + a(n-7) for n >= 8. Base values: a(1) = 1, a(2) = 6,
 *            a(3) = 28, a(4) = 133, a(5) = 651, a(6) = 3206, a(7) = 15771
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* sequence is undefined for n < 1 */

    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)6;
    if (n == 3) return (seq_t)28;
    if (n == 4) return (seq_t)133;
    if (n == 5) return (seq_t)651;
    if (n == 6) return (seq_t)3206;
    if (n == 7) return (seq_t)15771;

    /* Start with the seven known initial terms. */
    seq_t a1 = (seq_t)1;
    seq_t a2 = (seq_t)6;
    seq_t a3 = (seq_t)28;
    seq_t a4 = (seq_t)133;
    seq_t a5 = (seq_t)651;
    seq_t a6 = (seq_t)3206;
    seq_t a7 = (seq_t)15771;

    for (idx_t i = 8; i <= n; ++i) {
        seq_t v =
            ((seq_t)8   * a7) -
            ((seq_t)21  * a6) +
            ((seq_t)35  * a5) -
            ((seq_t)35  * a4) +
            ((seq_t)21  * a3) -
            ((seq_t)7   * a2) +
            a1;

        /* Shift the window for the next iteration. */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = a6;
        a6 = a7;
        a7 = v;
    }

    return a7;
}

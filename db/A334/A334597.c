/*
 * A334597  Dimensions of the finite-dimensional Lie algebras of type E_n
 *          (n=3,...,8).
 * https://oeis.org/A334597
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 11 for n=3; a(n) = n + 2*A272764(n) for 4 ≤ n ≤ 8, where
 *            A272764(n) is the number of positive roots of the E_n root
 *            system (10, 20, 36, 63, 120 for n = 4,5,6,7,8 respectively)
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 3) {
        return (seq_t)11;
    }
    if (n >= 4 && n <= 8) {
        /* Positive-root counts for E_n, n = 4..8 */
        static const seq_t roots[5] = {10, 20, 36, 63, 120};
        seq_t r = roots[(int)(n - 4)];
        /* a(n) = n + 2 * r */
        return ((seq_t)n) + (r << 1);
    }
    /* For n outside the defined range, return 0 (undefined). */
    return (seq_t)0;
}

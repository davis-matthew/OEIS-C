/*
 * A275793  The x members of the positive proper solutions (x = x1(n), y =
 *          y1(n)) of the first class for the Pell equation x^2 - 2*y^2 =
 *          +7^2.
 * https://oeis.org/A275793
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - a(n-2) (n ≥ 2), a(0)=9, a(1)=43
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)9;
    if (n == 1) return (seq_t)43;

    seq_t a_prev2 = (seq_t)9;   /* a(0) */
    seq_t a_prev1 = (seq_t)43;  /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t cur = 6 * a_prev1 - a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = cur;
    }
    return a_prev1;
}

/*
 * A377116  a(n) = coefficient of sqrt(6) in the expansion of (3 + sqrt(2) +
 *          sqrt(3))^n.
 * https://oeis.org/A377116
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*a(n-1) - 44*a(n-2) + 48*a(n-3) + 8*a(n-4); Initial
 *            values: a(0)=0, a(1)=0, a(2)=2, a(3)=18
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)18;

    /* Use a rolling window of the last four terms. */
    seq_t a0 = (seq_t)0;   /* a(n-4) */
    seq_t a1 = (seq_t)0;   /* a(n-3) */
    seq_t a2 = (seq_t)2;   /* a(n-2) */
    seq_t a3 = (seq_t)18;  /* a(n-1) */
    seq_t result = a3;

    for (idx_t i = 4; i <= n; ++i) {
        result = 12 * a3 - 44 * a2 + 48 * a1 + 8 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = result;
    }
    return result;
}

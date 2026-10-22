/*
 * A319512  a(n) = 7*a(n-1) - 14*a(n-2) + 7*a(n-3), a(0) = 1, a(1) = 3, a(2) =
 *          11.
 * https://oeis.org/A319512
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 14*a(n-2) + 7*a(n-3) a(0) = 1, a(1) = 3, a(2)
 *            = 11
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)11;

    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)3;   /* a(1) */
    seq_t a2 = (seq_t)11;  /* a(2) */
    for (idx_t i = 3; i <= n; ++i) {
        seq_t a3 = 7 * a2 - 14 * a1 + 7 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
    }
    return a2;
}

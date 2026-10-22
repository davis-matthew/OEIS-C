/*
 * A307469  a(n) = 2*a(n-1) + 6*a(n-2) for n >= 2, a(0) = 1, a(1) = 5.
 * https://oeis.org/A307469
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 6*a(n-2), a(0) = 1, a(1) = 5
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;

    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)5;   /* a(1) */
    seq_t an = a0;         /* placeholder for a(n) */

    for (idx_t i = 2; i <= n; ++i) {
        an = a1 * (seq_t)2 + a0 * (seq_t)6;
        a0 = a1;
        a1 = an;
    }
    return an;
}

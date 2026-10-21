/*
 * A255465  a(n) = A255464(2^n-1).
 * https://oeis.org/A255465
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 4*a(n-2) for n > 1; initial values: a(0) = 1,
 *            a(1) = 6
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0U) return (seq_t)1;
    if (n == 1U) return (seq_t)6;

    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)6;   /* a(1) */
    seq_t an;

    for (idx_t i = 2U; i <= n; ++i) {
        an = a1 * (seq_t)3 + a0 * (seq_t)4;
        a0 = a1;
        a1 = an;
    }
    return a1;
}

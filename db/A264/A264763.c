/*
 * A264763  a(0) = a(1) = 1; for n>1, a(n) = a(n-1) + (a(n-2) mod 5).
 * https://oeis.org/A264763
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + (a(n-2) mod 5), a(0) = a(1) = 1
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0 || n == 1)
        return (seq_t)1;

    seq_t a0 = (seq_t)1;  /* a(0) */
    seq_t a1 = (seq_t)1;  /* a(1) */
    seq_t an;

    for (idx_t i = 2; i <= n; ++i) {
        an = a1 + (a0 % (seq_t)5);
        a0 = a1;
        a1 = an;
    }
    return a1;
}

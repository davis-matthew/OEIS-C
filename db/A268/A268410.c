/*
 * A268410  a(n) = a(n - 1) + a(n - 2) + a(n - 3) for n>2, a(0)=5, a(1)=7,
 *          a(2)=9.
 * https://oeis.org/A268410
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3), a(0) = 5, a(1) = 7, a(2) = 9
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 34, u64 n <= 70, u128 n <= 143,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0) return (seq_t)5;
    if (n == 1) return (seq_t)7;
    if (n == 2) return (seq_t)9;

    /* iterative computation up to n */
    seq_t a0 = (seq_t)5;  /* a(0) */
    seq_t a1 = (seq_t)7;  /* a(1) */
    seq_t a2 = (seq_t)9;  /* a(2) */
    seq_t an = a2;

    for (idx_t i = 3; i <= n; ++i) {
        an = a0 + a1 + a2; /* a(i) */
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return a2;
}

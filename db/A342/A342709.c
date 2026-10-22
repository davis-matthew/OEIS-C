/*
 * A342709  12-gonal (dodecagonal) square numbers.
 * https://oeis.org/A342709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 48*a(n-1) - 48*a(n-2) + a(n-3); Initial values: a(1)=1,
 *            a(2)=64, a(3)=3025
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* below offset – defined behaviour */

    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)64;
    if (n == 3) return (seq_t)3025;

    seq_t a1 = (seq_t)1,        /* a(n-3) */
          a2 = (seq_t)64,       /* a(n-2) */
          a3 = (seq_t)3025;     /* a(n-1) */
    seq_t an;

    for (idx_t i = 4; i <= n; ++i) {
        an = (seq_t)48 * a3 - (seq_t)48 * a2 + a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return a3;
}

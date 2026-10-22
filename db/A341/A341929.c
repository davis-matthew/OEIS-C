/*
 * A341929  Bisection of the numerators of the convergents of cf
 *          (1,1,6,1,6,1,...,6,1).
 * https://oeis.org/A341929
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - a(n-2) initial values: a(0) = 1, a(1) = 2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    seq_t a0 = (seq_t)1;   /* a(n-2) */
    seq_t a1 = (seq_t)2;   /* a(n-1) */
    seq_t an;

    for (idx_t i = 2; i <= n; ++i) {
        an = (seq_t)8 * a1 - a0;   /* a(n) = 8*a(n-1) - a(n-2) */
        a0 = a1;
        a1 = an;
    }
    return a1;
}

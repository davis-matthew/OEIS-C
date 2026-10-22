/*
 * A342205  a(n) = T(n,n+1) where T(n,x) is a Chebyshev polynomial of the
 *          first kind.
 * https://oeis.org/A342205
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = T(n, n + 1) where T is the Chebyshev polynomial of the
 *            first kind
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t x = (seq_t)n + 1;
    seq_t T0 = (seq_t)1;
    seq_t T1 = x;

    if (n == 1)
        return T1;

    for (idx_t k = 2; k <= n; ++k) {
        seq_t Tnext = (seq_t)2 * x * T1 - T0;
        T0 = T1;
        T1 = Tnext;
    }
    return T1;
}

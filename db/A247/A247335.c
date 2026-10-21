/*
 * A247335  The curvature of touching circles inscribed in a special way in
 *          the larger segment of circle of radius 10/9 divided by a chord of
 *          length 4/3.
 * https://oeis.org/A247335
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 39*a(n-1) - 39*a(n-2) + a(n-3), a(0)=1, a(1)=10, a(2)=361
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)10;
    if (n == 2) return (seq_t)361;

    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)10;
    seq_t a2 = (seq_t)361;
    seq_t an = 0;

    for (idx_t i = 3; i <= n; ++i) {
        an = (seq_t)39 * a2 - (seq_t)39 * a1 + a0;
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return an;
}

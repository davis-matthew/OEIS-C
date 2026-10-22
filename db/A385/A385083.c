/*
 * A385083  Expansion of e.g.f. cosh(x)*(x + x^2/2).
 * https://oeis.org/A385083
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n if n is odd; a(n) = n/2*(n-1) if n is even (a(0)=0)
 * Range:     u8 n <= 23, u16 n <= 363, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sn = (seq_t)n;
    if (n == 0)
        return 0;
    if (n & 1ULL)
        return sn;
    return (sn * (sn - 1)) / 2;
}

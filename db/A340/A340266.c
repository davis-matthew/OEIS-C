/*
 * A340266  The number of degrees of freedom in a quadrilateral cell for a
 *          serendipity finite element space of order n.
 * https://oeis.org/A340266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+3)/2 + 3 for n > 1, a(1) = 4
 * Range:     u8 n <= 21, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) {
        return 4;
    }
    unsigned long long nn = (unsigned long long)n;
    unsigned long long val = nn * (nn + 3) / 2 + 3;
    return (seq_t)val;
}

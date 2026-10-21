/*
 * A255178  Second differences of eighth powers (A001016).
 * https://oeis.org/A255178
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1 a(n) = 2*(28*n^6 + 70*n^4 + 28*n^2 + 1) for n > 0 (OEIS
 *            A255178: Second differences of eighth powers)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 20, u64 n <= 831, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)1;
    }
    seq_t tn = (seq_t)n;
    seq_t n2 = tn * tn;
    seq_t n4 = n2 * n2;
    seq_t n6 = n4 * n2;
    seq_t res = 28 * n6;
    res += 70 * n4;
    res += 28 * n2;
    res += 1;
    res *= 2;
    return res;
}

/*
 * A256650  30-gonal pyramidal numbers: a(n) = n*(n+1)*(28*n-25)/6.
 * https://oeis.org/A256650
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(28*n-25)/6
 * Range:     u8 n <= 2, u16 n <= 13, u32 n <= 555, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    seq_t tn = (seq_t)n;
    seq_t t1 = tn + 1;
    seq_t t2 = (seq_t)(28 * tn - 25);
    seq_t prod = tn * t1;
    prod = prod * t2;
    return prod / 6;
}

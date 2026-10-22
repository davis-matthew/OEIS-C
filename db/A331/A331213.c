/*
 * A331213  a(n) = 1 + Sum_{i=1..n} (-1)^i * Product_{j=1..i} floor(n/j).
 * https://oeis.org/A331213
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 1 + Sum_{i=1..n} (-1)^i * Product_{j=1..i} floor(n/j)
 * Range:     i8 n <= 9, i16 n <= 15, i32 n <= 30, i64 n <= 59, i128 n <= 115,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    seq_t sum = 0;
    seq_t prod = 1;
    for (idx_t i = 1; i <= n; ++i) {
        prod *= (seq_t)(n / i);
        if (i & 1)
            sum -= prod;
        else
            sum += prod;
    }
    return sum + (seq_t)1;
}

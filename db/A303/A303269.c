/*
 * A303269  Sum of squares of odd digits minus sum of squares of even digits
 *          of n.
 * https://oeis.org/A303269
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = sum of squares of odd digits of n minus sum of squares of
 *            even digits of n
 * Range:     i8 n <= 78, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t sqr[10] = { 0, 1, 4, 9, 16, 25, 36, 49, 64, 81 };
    seq_t sum = 0;
    for (idx_t x = n; x > 0; x /= 10) {
        unsigned int d = x % 10u;
        if (d & 1)
            sum += sqr[d];
        else
            sum -= sqr[d];
    }
    return sum;
}

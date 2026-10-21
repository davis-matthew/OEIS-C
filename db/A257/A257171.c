/*
 * A257171  Sum of numbers on n-th segment of Ulam's spiral.
 * https://oeis.org/A257171
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; a(1) = 5; a(n) = (n^3 + 5 n^2 + 14 n + 16) / 8 for n
 *            even (n ≥ 2) a(n) = (n^3 + 4 n^2 + 11 n + 8 ) / 8 for n odd (n ≥
 *            3)
 * Range:     u8 n <= 11, u16 n <= 79, u32 n <= 2316, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;

    seq_t N = (seq_t)n;

    if ((n & 1) == 0) {   /* even n */
        return (N * N * N + 5 * N * N + 14 * N + 16) / 8;
    } else {              /* odd n */
        return (N * N * N + 4 * N * N + 11 * N + 8) / 8;
    }
}

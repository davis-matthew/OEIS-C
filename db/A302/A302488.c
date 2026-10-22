/*
 * A302488  Total domination number of the n X n grid graph.
 * https://oeis.org/A302488
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((-1)^n + 2*n*(n + 2) + 4*sin(n*Pi/2) - 1)/8
 * Range:     u8 n <= 31, u16 n <= 511, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent periodic form: a(4*m + r) = (2*m + 1)*(2*m + r), 0 <= r < 4 */

seq_t a(idx_t n)
{
    idx_t m = n / 4;          /* floor division */
    idx_t r = n % 4;          /* remainder 0..3 */

    seq_t term1 = (seq_t)(2 * m) + 1;   /* 2*m + 1 */
    seq_t term2 = (seq_t)(2 * m) + (seq_t)r;   /* 2*m + r */

    return term1 * term2;
}

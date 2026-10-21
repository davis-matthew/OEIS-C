/*
 * A272010  First differences of number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 413", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A272010
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 7 For n > 0 let k = floor(n/2). Then a(n) = 16*k*k + 24*k
 *            + 3 if n is even, -(16*k*k + 24*k + 3) if n is odd
 * Range:     i8 n <= 5, i16 n <= 89, i32 n <= 127, i64 n <= 127,
 *            i128 n <= 127, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)7;

    idx_t k = n >> 1;              /* floor(n/2) */
    seq_t kk = (seq_t)k;
    seq_t base = (seq_t)16 * kk * kk + (seq_t)24 * kk + (seq_t)3;

    return (n & 1) ? -base : base;
}

/*
 * A268201  a(n) = 4*n^3 - 6*n^2 + 3*n - 1.
 * https://oeis.org/A268201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n^3 - 6*n^2 + 3*n - 1
 * Range:     u8 n <= 4, u16 n <= 25, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* The sequence is indexed from 1; for n==0 return 0 (out of range). */
    if (n == 0)
        return 0;
    seq_t nn = n;
    seq_t n2 = nn * nn;          /* n^2 */
    seq_t n3 = n2 * nn;          /* n^3 */
    /* Compute the two parts of the expression to avoid unsigned underflow. */
    seq_t part1 = 4 * n3 + 3 * nn; /* 4n³ + 3n  (≥ part2 for all n≥1) */
    seq_t part2 = 6 * n2 + 1;      /* 6n² + 1      */
    return part1 - part2;
}

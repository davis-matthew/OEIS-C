/*
 * A289181  Number of 6-cycles in the n X n knight graph.
 * https://oeis.org/A289181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4 * (34*n^2 - 259*n + 484) for n > 5 with manual values:
 *            a(1)=0, a(2)=0, a(3)=0, a(4)=20, a(5)=164
 * Range:     u8 n <= 5, u16 n <= 25, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle small n explicitly */
    if (n == 1) return 0;
    if (n == 2) return 0;
    if (n == 3) return 0;
    if (n == 4) return 20;
    if (n == 5) return 164;

    /* For n > 5 use the polynomial formula.
       Computation is done in seq_t to avoid overflow
       on the target type when the result fits. */
    seq_t N = (seq_t)n;
    seq_t val = 34 * N * N - 259 * N + 484;
    return 4 * val;
}

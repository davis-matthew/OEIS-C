/*
 * A245336  Sum of digits of n written in fractional base 8/7.
 * https://oeis.org/A245336
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = A007953(A024649(n))
 * Range:     u8 n <= 1061, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;   /* copy n into seq_t to avoid overflow on arithmetic */
    seq_t sum = 0;

    /* The digits d_i of the fractional base 8/7 representation are
       obtained by the recursion:
           d_i = N_i mod 8
           N_{i+1} = (N_i - d_i) * 7 / 8
       until N becomes 0. */
    while (N) {
        seq_t d = N % (seq_t)8;
        sum += d;
        N = (N - d) * (seq_t)7 / (seq_t)8;
    }

    return sum;
}

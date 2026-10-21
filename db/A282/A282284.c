/*
 * A282284  Least common multiple of 3*n+1 and 3*n-1.
 * https://oeis.org/A282284
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1 For n>0 even: a(n) = 9*n^2 - 1 For n>0 odd : a(n) =
 *            (9*n^2 - 1)/2 (Same as lcm(3*n+1, 3*n-1) )
 * Range:     u8 n <= 5, u16 n <= 85, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)1;
    seq_t nsq = (seq_t)n * (seq_t)n;    /* n^2 */
    seq_t nine_nsq_minus_one = 9 * nsq - 1; /* 9*n^2 - 1 */
    if ((n & 1) == 0)                  /* n even */
        return nine_nsq_minus_one;
    else                                /* n odd */
        return nine_nsq_minus_one / 2;
}

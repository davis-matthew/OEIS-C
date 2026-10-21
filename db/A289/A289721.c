/*
 * A289721  Let a(0)=1. Then a(n) = sums of consecutive strings of positive
 *          integers of length 3*n, starting with the integer 2.
 * https://oeis.org/A289721
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; for n > 0, a(n) = 9 * n * (n*n + 1) / 2
 * Range:     u8 n <= 2, u16 n <= 19, u32 n <= 916, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t tn  = (seq_t)n;
    seq_t sq  = tn * tn;      /* n^2 */
    seq_t sum = sq + (seq_t)1;   /* n^2 + 1 */
    seq_t tmp = sum * (seq_t)9;  /* 9 * (n^2 + 1) */
    seq_t prod = tmp * tn;       /* 9 * n * (n^2 + 1) */
    return prod / (seq_t)2;      /* divide by 2 */
}

/*
 * A247322  Number of paths from (0,0) to the line x = n, each consisting of
 *          segments given by the vectors (1,1), (1,2), (1,-1), with vertices
 *          (i,k) satisfying 0 <= k <= 3.
 * https://oeis.org/A247322
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 33, u64 n <= 66, u128 n <= 133,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Number of paths using steps (1,1), (1,2), (1,-1) staying 0<=y<=3
   Formula (linear recurrence): a(n) = 3*a(n-2) + 2*a(n-3) - a(n-4)
   for n >= 4, with a(0)=1, a(1)=2, a(2)=5, a(3)=9 */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)9;

    /* Use sliding window of last four values:
       x0 = a(i-4), x1 = a(i-3), x2 = a(i-2), x3 = a(i-1)  */
    seq_t x0 = (seq_t)1;   /* a(0) */
    seq_t x1 = (seq_t)2;   /* a(1) */
    seq_t x2 = (seq_t)5;   /* a(2) */
    seq_t x3 = (seq_t)9;   /* a(3) */

    seq_t ai = 0;
    for (idx_t i = 4; i <= n; ++i) {
        ai = x2 * (seq_t)3 + x1 * (seq_t)2 - x0;
        /* shift window for next iteration */
        x0 = x1;
        x1 = x2;
        x2 = x3;
        x3 = ai;
    }
    return ai;
}

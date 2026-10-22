/*
 * A368524  a(n) = Sum_{k=1..n} k^2 * n^(n-k).
 * https://oeis.org/A368524
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} k^2 * n^(n-k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Small cases */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    seq_t sum   = 0;
    seq_t power = 1;                 /* power = n^(n-k) starts with n^(n-n)=n^0 = 1 */

    /* Iterate k from n down to 1, accumulating k^2 * power */
    for (idx_t k = n; k > 0; --k) {
        seq_t kk   = (seq_t)k;       /* convert idx_t to seq_t */
        seq_t term = kk * kk * power;
        sum += term;

        /* Update power for the next term (k-1) */
        if (k > 1) {
            power *= (seq_t)n;
        }
    }
    return sum;
}

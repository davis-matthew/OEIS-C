/*
 * A299646  a(n) = Sum_{k = n..2*n+1} k^2.
 * https://oeis.org/A299646
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k = n..2*n+1} k^2
 * Range:     u8 n <= 3, u16 n <= 29, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* limit = 2*n + 1, computed safely */
    idx_t limit = n + n + 1;
    for (idx_t k = n; k <= limit; ++k) {
        seq_t ks = (seq_t)k * (seq_t)k;   /* k^2 */
        sum += ks;                        /* accumulate */
    }
    return sum;
}

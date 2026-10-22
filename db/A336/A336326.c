/*
 * A336326  a(n) is the least k > 0 such that A000217(k) is divisible by n+k,
 *          except a(2) = 0.
 * https://oeis.org/A336326
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k > 0 such that A000217(k) is divisible by n + k,
 *            with a(2)=0
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case where no k exists */
    if (n == 2) return 0;

    seq_t t = 0;            /* running triangular number T_k = k(k+1)/2 */
    for (idx_t k = 1; ; ++k) {
        t += (seq_t)k;                /* T_k = T_{k-1} + k */
        seq_t denom = (seq_t)k + (seq_t)n;
        if (t % denom == 0) return (seq_t)k;
    }
}

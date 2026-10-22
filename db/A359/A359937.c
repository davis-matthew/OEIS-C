/*
 * A359937  a(n) = Sum_{d|n, d-1 is square} d.
 * https://oeis.org/A359937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, d-1 is a perfect square} d
 * Range:     u8 n <= 84, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* iterate over all numbers k such that d = k^2 + 1 divides n */
    idx_t k = 0;
    seq_t sum = 0;
    while (1) {
        /* d must be <= n; compute as seq_t then cast back to idx_t for modulo */
        seq_t d_seq = (seq_t)k * (seq_t)k + 1;
        if (d_seq > n) break;          /* d exceeds n, stop */
        idx_t d = (idx_t)d_seq;        /* safe cast, d <= n */
        if (n % d == 0)                /* d divides n? */
            sum += d_seq;
        ++k;
    }
    return sum;
}

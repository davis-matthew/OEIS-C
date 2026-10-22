/*
 * A309176  a(n) = n^2 * (n + 1)/2 - Sum_{k=1..n} sigma_2(k).
 * https://oeis.org/A309176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (n mod k) * k
 * Range:     u8 n <= 14, u16 n <= 87, u32 n <= 3510, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t nn = (seq_t)n;                 /* cast index to solution type */
    for (idx_t k = 1; k <= n; ++k) {
        seq_t div = (seq_t)k;           /* divisor in solution type */
        seq_t r = nn % div;             /* n mod k */
        sum += r * div;                 /* add (n mod k) * k */
    }
    return sum;
}

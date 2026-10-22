/*
 * A339370  a(n) = Sum_{k=1..floor(n/2)} (n-k) * floor((n-k)/k).
 * https://oeis.org/A339370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..floor(n/2)} (n-k) * floor((n-k)/k)
 * Range:     u8 n <= 12, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t half = n / 2;            /* floor(n/2) */
    for (idx_t k = 1; k <= half; ++k)
    {
        idx_t rem = n - k;         /* n - k  >= 1 */
        idx_t t   = rem / k;       /* floor((n-k)/k) */
        sum += (seq_t)rem * (seq_t)t;
    }
    return sum;
}

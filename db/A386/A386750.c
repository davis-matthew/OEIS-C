/*
 * A386750  a(n) = n*sigma_6(n).
 * https://oeis.org/A386750
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = n * sigma_6(n) = n * sum_{d|n} d^6
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 23, u64 n <= 563, u128 n <= 3000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t sigma = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d != 0)            /* not a divisor, skip */
            continue;

        seq_t d6 = (seq_t)d * d * d * d * d * d;   /* d^6 */
        idx_t other = n / d;

        if (other == d) {          /* perfect square divisor */
            sigma += d6;
        } else {                   /* pair of distinct divisors */
            seq_t o6 = (seq_t)other * other * other * other * other * other; /* other^6 */
            sigma += d6 + o6;
        }
    }

    return (seq_t)n * sigma;
}

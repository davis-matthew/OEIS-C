/*
 * A247840  a(n) = Sum_{k=2..n} 6^k.
 * https://oeis.org/A247840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=2..n} 6^k
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    seq_t sum   = 0;
    seq_t power = 36;          /* 6^2 */

    for (idx_t k = 2; k <= n; ++k) {
        sum += power;
        /* Multiply by 6 for the next term; the extra multiplication on
           the final iteration does not affect the result. */
        power *= 6;
    }
    return sum;
}

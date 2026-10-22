/*
 * A366983  a(n) = Sum_{k=1..n} (k+1) * floor(n/k).
 * https://oeis.org/A366983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} (k+1) * floor(n/k)
 * Range:     u8 n <= 15, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handles n == 0 gracefully, though the sequence is defined for n >= 1 */
    if (n == 0)
        return 0;

    seq_t acc = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        /* floor division is inherent in idx_t / idx_t */
        idx_t quot = n / k;
        acc += (seq_t)(k + 1) * (seq_t)quot;
    }
    return acc;
}

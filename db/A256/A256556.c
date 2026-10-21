/*
 * A256556  If n = 10k+m (0 <= m <= 9) then a(n) = n*k^m.
 * https://oeis.org/A256556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = n * (⌊n/10⌋)^(n mod 10) for n ≥ 10, a(n) = 0 for n < 10
 * Range:     u8 n <= 23, u16 n <= 36, u32 n <= 88, u64 n <= 678,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* values for n = 1…9 are defined as 0 */
    if (n < 10)
        return 0;

    /* k = floor(n/10), m = n % 10 */
    seq_t k = n / 10;
    idx_t m = n % 10;

    /* Compute k^m (m ≤ 9, trivial loop) */
    seq_t powk = 1;
    for (idx_t i = 0; i < m; ++i)
        powk *= k;

    /* Final result */
    return (seq_t)n * powk;
}

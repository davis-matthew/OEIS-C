/*
 * A338233  Number of numbers less than n whose square does not divide n.
 * https://oeis.org/A338233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1) - #{m : 1 ≤ m < n and m² | n}
 * Range:     u8 n <= 257, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    idx_t cnt = 0;
    for (idx_t m = 1; m < n; ++m) {
        idx_t sq = m * m;          /* m², fits in 64‑bit for all n ≤ 2⁶⁴ */
        if (sq > n)
            break;                 /* m² > n → no larger m will satisfy m² ≤ n */
        if (n % sq == 0)
            ++cnt;                 /* m² divides n */
    }
    return (seq_t)(n - 1) - (seq_t)cnt;
}

/*
 * A251091  a(n) = n^2 / gcd(n+2, 4).
 * https://oeis.org/A251091
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = n^2 / gcd(n+2, 4)
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute gcd(n+2,4) by examining n mod 4. */
    idx_t m = n + 2;          /* m runs 0..UINT64_MAX+1, but n is within bounds */
    seq_t d;
    if ((m & 3ULL) == 0)      /* m % 4 == 0 */
        d = 4;
    else if ((m & 1ULL) == 0) /* m % 2 == 0 (but not %4) */
        d = 2;
    else
        d = 1;                /* odd case */

    /* Compute n^2 / d */
    seq_t n_sq = (seq_t)n * ((seq_t)n);
    return n_sq / d;
}

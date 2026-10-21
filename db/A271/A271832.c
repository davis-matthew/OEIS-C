/*
 * A271832  Period 12 zigzag sequence: repeat [0,1,2,3,4,5,6,5,4,3,2,1].
 * https://oeis.org/A271832
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = abs(n - 12*round(n/12)). Equivalent periodic description:
 *            a(n) = a(n-12) for n≥12, with the initial block a(0..11) =
 *            {0,1,2,3,4,5,6,5,4,3,2,1}
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute n modulo 12 and map the values 0..11 to the
       zig‑zag pattern.  No overflow or special cases are
       required because the results are always between 0 and 6. */
    idx_t mod = n % 12;          /* 0 ≤ mod < 12 */
    if (mod <= 6)
        return (seq_t)mod;
    return (seq_t)(12 - mod);
}

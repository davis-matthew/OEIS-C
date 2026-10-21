/*
 * A289111  a(n) = (2^49 - 2)*n/3 + 444813635231.
 * https://oeis.org/A289111
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2^49 - 2)*n/3 + 444813635231
 * Range:     u8 none, u16 none, u32 none, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t base = 444813635231ULL;          /* 444813635231 */
    const seq_t mult = 187649984473770ULL;        /* (2^49 - 2)/3 = 187649984473770 */
    return base + mult * (seq_t)n;
}

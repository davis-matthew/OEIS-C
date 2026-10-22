/*
 * A304205  Numbers k such that 24*k + 6 is congruent to 0 (mod 49).
 * https://oeis.org/A304205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 49*n - 37
 * Range:     u8 n <= 5, u16 n <= 1338, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is 1‑based; the formula holds for all n >= 1 */
    return ((seq_t)n * (seq_t)49) - (seq_t)37;
}

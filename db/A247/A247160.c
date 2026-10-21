/*
 * A247160  Dynamic Betting Game D(n,4,3).
 * https://oeis.org/A247160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(16*n/15) (for n >= 1)
 * Range:     u8 n <= 239, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence definition is a simple floor of a linear function.
       We cast n to seq_t first to keep the multiplication exact in
       the chosen integer type. */
    return ((seq_t)n * 16) / 15;
}

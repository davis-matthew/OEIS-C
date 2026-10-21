/*
 * A258588  Minimal most likely sum for a roll of n 10-sided dice.
 * https://oeis.org/A258588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(11*n/2) for n > 1; a(1) = 1
 * Range:     u8 n <= 46, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) {
        /* a(1) = 1 is explicitly defined in the OEIS entry. */
        return 1;
    }
    /* Convert the index to seq_t before any arithmetic. */
    seq_t val = ((seq_t)n * 11) / 2;
    return val;
}

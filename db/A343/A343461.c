/*
 * A343461  a(n) is the maximal number of regular n-gons that can be arranged
 *          around a vertex without overlapping.
 * https://oeis.org/A343461
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(2*n/(n-2))
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 3.  For smaller n the value
       does not matter; returning 0 is harmless. */
    if (n < 3)
        return (seq_t)0;

    /* Convert to seq_t before performing arithmetic to avoid any
       overflow on the index type.  The division truncates toward
       zero, which is equivalent to floor for positive operands. */
    return ((seq_t)2 * (seq_t)n) / (((seq_t)n) - (seq_t)2);
}

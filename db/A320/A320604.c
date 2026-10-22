/*
 * A320604  Chromatic number of the n-polygon diagonal intersection graph.
 * https://oeis.org/A320604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (10 - (-1)^n + cos(n*Pi/3) - cos(2*n*Pi/3))/3
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent to the periodic pattern 4,3,3,3,4,3 repeated every 6. */
seq_t a(idx_t n)
{
    /* n >= 1 is guaranteed by the problem statement. */
    static const seq_t pattern[6] = {4, 3, 3, 3, 4, 3};
    /* Compute index in the repeating block. */
    idx_t i = (n - 1) % 6;
    return pattern[i];
}

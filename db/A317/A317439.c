/*
 * A317439  Numbers missing from A317437.
 * https://oeis.org/A317439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n - 1 for n <= 11360; a(n) = n + 11360 for n > 11360
 * Range:     u8 n <= 128, u16 n <= 9408, u32 n <= 9408, u64 n <= 9408,
 *            u128 n <= 9408, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const idx_t M = 11360;        /* cutoff index */
    if (n <= M)
        return (seq_t)2 * (seq_t)n - 1; /* odd numbers 1,3,5,... */
    else
        return (seq_t)n + (seq_t)M;     /* all numbers > 22720 */
}

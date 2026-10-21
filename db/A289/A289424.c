/*
 * A289424  a(n) = length of longest circuit code K(n,3).
 * https://oeis.org/A289424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(1)=0; a(n)=2*n for 2≤n≤5; a(6)=16; a(n)=12*n-60 for n≥7
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)0;
    if (n <= 5)
        return (seq_t)(2 * n);
    if (n == 6)
        return (seq_t)16;
    return (seq_t)(12 * n - 60);
}

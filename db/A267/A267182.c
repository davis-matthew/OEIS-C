/*
 * A267182  Row 2 of the square array in A267181.
 * https://oeis.org/A267182
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1, 2, (n/2)-1 for even n>0, (n+3)/2 for odd n>0
 * Range:     u8 n <= 508, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if ((n & 1) == 0)               /* even n */
        return (seq_t)(n / 2) - (seq_t)1;
    else                            /* odd n */
        return ((seq_t)(n + 3)) / (seq_t)2;
}

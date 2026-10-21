/*
 * A294271  Sum of the fourth powers of the parts in the partitions of n into
 *          two parts.
 * https://oeis.org/A294271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/480)*n*(-16 + 160*n² + 15*(-15 + (-1)^n)*n³ + 96*n⁴)
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 117, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* offset 1: a(1)=0 */

seq_t a(idx_t n)
{
    /* n is assumed >=1 (offset 1) */
    /* Use a 128‑bit integer for intermediate calculations so that
     * the result is exact for all n where it fits in seq_t. */
    __int128 N   = ( __int128 )n;   /* n */
    __int128 n2  = N * N;           /* n² */
    __int128 n3  = n2 * N;          /* n³ */
    __int128 n4  = n3 * N;          /* n⁴ */

    int parity = (n & 1) ? -1 : 1;   /* (-1)^n */

    /* inner part: -16 + 160*n² + 15*(-15 + (-1)^n)*n³ + 96*n⁴ */
    __int128 inner = -16
                     + 160 * n2
                     + 15 * ((-15 + parity)) * n3
                     + 96 * n4;

    __int128 numerator = N * inner;   /* n * inner */
    __int128 result128 = numerator / 480; /* exact division */

    return (seq_t) result128;
}

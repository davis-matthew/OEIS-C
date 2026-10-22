/*
 * A325168  Number of integer partitions of n with origin-to-boundary
 *          graph-distance equal to 2.
 * https://oeis.org/A325168
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=a(1)=a(2)=0; a(3)=1; a(4)=3; For n>=5: if n is even,
 *            a(n)=2*n-4; if n is odd, a(n)=2*n-5 (This follows from the
 *            recurrence a(n)=a(n-1)+a(n-2)-a(n-3) for n>7.)
 * Range:     u8 n <= 129, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle small indices explicitly */
    if (n <= 2U)
        return 0;
    if (n == 3U)
        return 1;
    if (n == 4U)
        return 3;

    /* For n >= 5 use the closed formulas */
    seq_t nn = (seq_t)n;              /* cast to seq_t to avoid overflow in 64‑bit builds */
    if (n & 1ULL)                     /* odd */
        return nn * 2u - 5u;
    else                               /* even */
        return nn * 2u - 4u;
}

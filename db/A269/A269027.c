/*
 * A269027  Parity of the number of 1's in A039724(n).
 * https://oeis.org/A269027
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(4k) = a(k); a(4k+1) = 1 - a(k); a(4k+2) = 1 - a(k+1); a(4k+3)
 *            = a(k+1); Base: a(0)=0, a(1)=1
 * Range:     u8 n <= 2047, u16 n <= 2047, u32 n <= 2047, u64 n <= 2047,
 *            u128 n <= 2047, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)n;          /* a(0)=0, a(1)=1 */

    idx_t k   = n >> 2;           /* k = ⌊n/4⌋ */
    switch (n & 3)                /* n % 4 */
    {
        case 0: return a(k);               /* a(4k) = a(k) */
        case 1: return a(k) ^ (seq_t)1;    /* a(4k+1) = 1 - a(k) */
        case 2: return a(k+1) ^ (seq_t)1;  /* a(4k+2) = 1 - a(k+1) */
        case 3: return a(k+1);             /* a(4k+3) = a(k+1) */
        default: return 0; /* unreachable */
    }
}

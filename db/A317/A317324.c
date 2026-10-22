/*
 * A317324  Multiples of 24 and odd numbers interleaved.
 * https://oeis.org/A317324
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(2n) = 24*n, a(2n+1) = 2*n+1
 * Range:     u8 n <= 21, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is an unsigned 64‑bit index.  Use integer division to obtain
       floor(n/2).  All arithmetic is done in seq_t to match the
       requested output type. */
    if ((n & 1) == 0)          /* even index: a(2k) = 24*k */
        return (seq_t)(n / 2) * (seq_t)24;
    else                       /* odd index : a(2k+1) = 2*k+1 */
        return (seq_t)(n / 2) * (seq_t)2 + (seq_t)1;
}

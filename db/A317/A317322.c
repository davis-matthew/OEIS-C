/*
 * A317322  Multiples of 22 and odd numbers interleaved.
 * https://oeis.org/A317322
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(2n) = 22*n, a(2n+1) = 2*n+1
 * Range:     u8 n <= 23, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n & 1ULL) == 0ULL)               /* even index: a(2k) = 22*k */
        return (seq_t)22 * (seq_t)(n / 2);
    else                                  /* odd index: a(2k+1) = 2*k+1 = n */
        return (seq_t)n;
}

/*
 * A317326  Multiples of 26 and odd numbers interleaved.
 * https://oeis.org/A317326
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(2*n) = 26*n, a(2*n+1) = 2*n + 1
 * Range:     u8 n <= 19, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* k = floor(n/2) is the integer used in the formulas */
    seq_t k = (seq_t)(n >> 1);
    if ((n & 1ULL) == 0)          /* even index: a(2*k) = 26*k */
        return (seq_t)26 * k;
    else                          /* odd index: a(2*k+1) = 2*k + 1 */
        return (seq_t)2 * k + 1;
}

/*
 * A397808  Decimal representation of the Collatz iteration encoding of n.
 * https://oeis.org/A397808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 0; for n > 1, a(n) = 2*a(n/2)+1 if n even, otherwise a(n)
 *            = 2*a(3*n+1)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 26, u64 n <= 26, u128 n <= 312,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 0;
    if ((n & 1ULL) == 0)
        return (a(n >> 1) << 1) | 1;
    return a(3*n + 1) << 1;
}

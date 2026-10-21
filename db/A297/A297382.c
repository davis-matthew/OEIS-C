/*
 * A297382  Denominator of -A023900(n)/2.
 * https://oeis.org/A297382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = 2 if n is a power of 2, otherwise 1
 * Range:     u8 n <= 9974, u16 n <= 9974, u32 n <= 9974, u64 n <= 9974,
 *            u128 n <= 9974, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For safety, treat n==0 (outside the offset) as 1 */
    if (n == 0)
        return (seq_t)1;
    return ((n & (n - 1)) == 0) ? (seq_t)2 : (seq_t)1;
}

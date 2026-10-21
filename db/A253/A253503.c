/*
 * A253503  Number of (n+2) X (1+2) 0..1 arrays with every 2 X 2 and 3 X 3
 *          subblock diagonal maximum minus antidiagonal minimum nondecreasing
 *          horizontally and vertically.
 * https://oeis.org/A253503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 20*n^2 + 68*n + 44
 * Range:     u8 n <= 1, u16 n <= 55, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert n to seq_t once to avoid repeated casts */
    seq_t sn = (seq_t)n;
    /* Compute n^2.  This intermediate value is guaranteed to be
       less than or equal to 20*n^2 ≤ a(n) for positive n, so it
       cannot overflow before the final result for any n that fits
       into the public type seq_t. */
    seq_t t = sn * sn;
    /* 20 * n^2 */
    t = t * 20;
    /* Add 68 * n */
    t = t + sn * 68;
    /* Add the constant term 44 */
    t = t + 44;
    return t;
}

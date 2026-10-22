/*
 * A371689  Main diagonal of A365495: parity of the n-th term in the
 *          trajectory of n under the A014682 map.
 * https://oeis.org/A371689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = parity of the (n-1)-st iterate of n under the accelerated
 *            Collatz map: f(m) = m/2 if m even, else (3*m+1)/2
 * Range:     u8 n <= 40, u16 n <= 8130, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle trivial case */
    if (n == 1) return 1;

    seq_t x = (seq_t)n;
    for (idx_t i = 1; i < n; ++i)   /* iterate n-1 times */
    {
        if ((x & 1) == 0)           /* m even        */
            x = x >> 1;             /* m/2           */
        else                        /* m odd         */
            x = ((x * 3) + 1) >> 1; /* (3*m+1)/2     */
    }
    return x & 1;                   /* parity of final term */
}

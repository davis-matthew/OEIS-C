/*
 * A298822  Number of minimum edge covers in the n-dipyramidal graph.
 * https://oeis.org/A298822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(2*k) = 2*k^2, a(2*k-1) = (2*k-1)*(2*k^2-1)
 * Range:     u8 n <= 8, u16 n <= 50, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Ensure n >= 1; behaviour for n = 0 is undefined but return 0 for safety */
    if (n == 0u) return (seq_t)0;

    /* Use the parity of n */
    if ((n & 1u) == 0u)           /* n is even: n = 2*k */
    {
        seq_t k = (seq_t)(n >> 1);    /* k = n/2 */
        return (seq_t)2 * k * k;      /* 2*k^2 */
    }
    else                            /* n is odd: n = 2*k-1 */
    {
        seq_t k = (seq_t)((n + 1u) >> 1);   /* k = (n+1)/2 */
        seq_t t = (seq_t)2 * k * k - (seq_t)1; /* 2*k^2 - 1 */
        return (seq_t)n * t;             /* n * (2*k^2-1) */
    }
}

/*
 * A337173  a(n) = Sum_{k=1..floor(n/2)} k^2 * (n-k)^2.
 * https://oeis.org/A337173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..floor(n/2)} k^2 * (n-k)^2
 * Range:     u8 n <= 6, u16 n <= 20, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t half = n / 2;            /* floor(n/2) */
    for (idx_t k = 1; k <= half; ++k)
    {
        seq_t ksq = (seq_t)k * k;                  /* k^2 */
        seq_t nk  = (seq_t)n - (seq_t)k;          /* n-k */
        seq_t nksq = nk * nk;                     /* (n-k)^2 */
        sum += ksq * nksq;                        /* k^2*(n-k)^2 */
    }
    return sum;
}

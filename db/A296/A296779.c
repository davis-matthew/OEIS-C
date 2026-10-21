/*
 * A296779  Detour index of the n X n grid graph.
 * https://oeis.org/A296779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2n-1) = 32*n^3*(n-1)^3, a(2n) = 4*n^2*(8*n^4 - 5*n^2 + 1)
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 45, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The formula applies for all n ≥ 1. */
    if ((n & 1) == 0)          /* even n = 2k */
    {
        seq_t k  = (seq_t)(n / 2);
        seq_t k2 = k * k;       /* k^2 */
        seq_t k4 = k2 * k2;     /* k^4 */
        seq_t term = 8 * k4 - 5 * k2 + (seq_t)1;  /* 8k^4 - 5k^2 + 1 */
        return 4 * k2 * term;  /* 4k^2 * term */
    }
    else                        /* odd n = 2k-1 */
    {
        seq_t k  = (seq_t)((n + 1) / 2); /* k = (n+1)/2  */
        seq_t k1 = k - 1;                /* k-1     */
        seq_t k2 = k * k;                /* k^2     */
        seq_t k3 = k2 * k;               /* k^3     */
        seq_t k1_2 = k1 * k1;            /* (k-1)^2 */
        seq_t k1_3 = k1_2 * k1;          /* (k-1)^3 */
        return 32 * k3 * k1_3;           /* 32*k^3*(k-1)^3 */
    }
}

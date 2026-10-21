/*
 * A296819  Maximum detour index of any bipartite graph on n nodes.
 * https://oeis.org/A296819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2*n-1) = 4*(n-1)^3, a(2*n) = n*(4*n^2 - 5*n + 2)
 * Range:     u8 n <= 8, u16 n <= 51, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;                     /* undefined for n < 1, but guard anyway */
    }

    if (n & 1U) { /* odd n: n = 2*k - 1 */
        seq_t k = ((seq_t)n + 1) / 2; /* k = (n+1)/2 */
        k = k - 1;                   /* k := k-1 */
        seq_t k2 = k * k;
        seq_t k3 = k2 * k;
        return 4 * k3;
    } else { /* even n: n = 2*k */
        seq_t k = (seq_t)n / 2;        /* k = n/2 */
        seq_t t = 4 * k * k;           /* 4*k^2 */
        t = t - 5 * k;                 /* -5*k */
        t = t + 2;                     /* +2 */
        return k * t;
    }
}

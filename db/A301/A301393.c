/*
 * A301393  a(n) = Product_{k=1..L} hypergeom([-n, -n], [1], k) with L = 4.
 * https://oeis.org/A301393
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=1..4} hypergeom([-n,-n],[1],k)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    for (idx_t k = 1; k <= 4; ++k)
    {
        seq_t sum = 0;
        seq_t C = 1;
        seq_t powk = 1;

        /* m = 0 term (C = 1, powk = 1) */
        sum += 1;

        for (idx_t m = 1; m <= n; ++m)
        {
            C = C * (seq_t)(n - m + 1) / (seq_t)m;    /* recurrence for C(n,m) */
            powk = powk * (seq_t)k;                   /* k^m */
            seq_t term = C * C;                       /* C(n,m)^2 */
            term *= powk;                             /* * k^m */
            sum += term;
        }

        result *= sum;
    }
    return result;
}
